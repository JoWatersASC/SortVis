#include"application.h"

//Forward Declarations
namespace MySrt {
    static ImVec2 getWinDim(size_t numWindows, ImVec2 contentDim);
}
//*******************************************************************************************************\\
                                        Application Variables
//*******************************************************************************************************\\

namespace MySrt {
    static const ImGuiWindowFlags winFlags_ = (
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove       |
        ImGuiWindowFlags_NoNav        |
        ImGuiWindowFlags_NoCollapse   |
        ImGuiWindowFlags_NoSavedSettings
        );
    static const ImGuiSliderFlags sliderFlags = (
        ImGuiSliderFlags_AlwaysClamp |
        ImGuiSliderFlags_Logarithmic
        );

    ImVec2 dspSize;

    static int numItems = 10; //variable to be passed to windows
    static int numItems_ = 10; //variable to be edited with slider (ONLY EDIT IN WINDOW VECTOR MOD FUNCTIONS)

    bool renderWinLists;
    ImVec2 winRatio;
    ImVec2 winDim;
    ImVec2 winContentDim;

    typedef std::map<std::string, SortingWindow*>::iterator SWL_itr;
    static std::map<std::string, SortingWindow*>* SWL;
    static SWL_itr swlEnd;

    //Random number generator
    static std::random_device rand_dev;
    static std::mt19937 generate(rand_dev());

    Uint32 lastTime, currentTime;
    //Verifies that End() has been called after Start()
    bool cleaned;
    std::thread sortThread;
}

//*******************************************************************************************************\\
                                    Main Application Functions
//*******************************************************************************************************\\
    
namespace MySrt {
    //Place before main loop
    void Start() {
        cleaned = false;
        winRatio = { 400.0f / 1280.0f, 275.0f / 725.0f };
        winDim = { 400, 275 };
        dspSize = ImGui::GetIO().DisplaySize;

        windows_open["Selection Sort"] = true;

        SortingWindow::SortingWindowList = new std::map<std::string, SortingWindow*>();
        SortingWindow::SortingWindowList->operator[]("Selection Sort") =
            new SortingWindow({ 40, 180 }, "Selection Sort", &windows_open["Selection Sort"]);

        SWL = SortingWindow::SortingWindowList;

        swlEnd = SWL->end();

        Reset(true);

        SWL->operator[]("Selection Sort")->printList();
        lastTime = SDL_GetTicks();
    }


    void Update(SDL_Window* window) {
        currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
    }
    void Draw(SDL_Window* window, ImGuiIO& io) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        dspSize = ImGui::GetIO().DisplaySize;
        dspSize.x -= 4; // leaves 2 pixels on either side
        dspSize.y -= 22; //leaves 2 pixels above(mainMenuBar is 18 pixels) and below
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        RenderMainMenuBar();

        ImGui::SetNextWindowPos({ 2, 20 });
        ImGui::SetNextWindowSize(dspSize);
        ImGui::Begin("Content Window", NULL, winFlags_ | ImGuiWindowFlags_NoBringToFrontOnFocus);

        RenderInputs();
        RenderWindows();

        ImGui::End();
        ImGui::Render();

        glViewport(0, 0, (size_t)io.DisplaySize.x, (size_t)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    //Place inside main loop
    void Run() {
        dspSize = ImGui::GetIO().DisplaySize;
        dspSize.x -= 4; // leaves 2 pixels on either side
        dspSize.y -= 22; //leaves 2 pixels above(mainMenuBar is 18 pixels) and below

        RenderMainMenuBar();

        ImGui::SetNextWindowPos({ 2, 20 });
        ImGui::SetNextWindowSize(dspSize);
        ImGui::Begin("Content Window", NULL, winFlags_ | ImGuiWindowFlags_NoBringToFrontOnFocus);

        RenderInputs();
        RenderWindows();

        ImGui::End();
    }

    //Place after main loop
    void End() {
        SWL_itr it = SWL->begin();
        size_t size = SWL->size();
        size_t count = 0;

        while (count < size && it != swlEnd) {
            delete it->second;
            count++;
            it++;
        }
        SortingWindow::SortingWindowList->clear();

        delete SortingWindow::SortingWindowList;
        cleaned = true;
    }
}

//*******************************************************************************************************\\
                                    Main Menu Render Functions
//*******************************************************************************************************\\
    
namespace MySrt {
    void RenderMainMenuBar() {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                RenderMainMenuOptions();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("My Links")) {
                RenderMainMenuAbout();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
    void RenderMainMenuOptions() {
        if (ImGui::BeginMenu("Add Sorting Function"))
        {
            for (auto it : sort_funcs) {
                std::string sortFuncItem = it.first;

                if (SWL->count(sortFuncItem)) continue;

                if (ImGui::MenuItem(sortFuncItem.c_str()))
                {
                    windows_open[sortFuncItem] = true;
                    SortingWindow::SortingWindowList->operator[](sortFuncItem) =
                        new SortingWindow({ 40 + (SWL->size() % 3) * (winDim.x + 20), 
                            (SWL->size() > 3 ? 0.2f * winDim.y : 1.1f * winDim.y) },
                            sortFuncItem.c_str(), 
                            &windows_open[sortFuncItem]
                        );
                }
            }

            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Reset")) {}
    }
    void RenderMainMenuAbout() {
        if (ImGui::BeginMenu("About Me"))
        {
            if (ImGui::MenuItem("GitHub"))
            {
                open_url("https://github.com/JoWatersASC");
            }
            if (ImGui::MenuItem("LinkedIn"))
            {
                open_url("https://www.linkedin.com/in/joshua-b-waters/");
            }
            ImGui::EndMenu();
        }
    }
}

//*******************************************************************************************************\\
                                    Content Area Render Functions
//*******************************************************************************************************\\

namespace MySrt {
    void RenderInputs() {
        ImGui::SliderInt("Value", &numItems_, 2, 50000, "%d", sliderFlags); ImGui::SameLine();

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            const char* helpText = "\"Ctrl + click\" to edit manually";
            ImGui::TextUnformatted(helpText);

            ImGui::EndTooltip();
        } ImGui::SameLine();

        if (ImGui::Button("Confirm")) {
            numItems = numItems_;
            Reset(true);
        }

        if (ImGui::Button("Reset Windows")) {
            Reset(false);
        }ImGui::SameLine();
        if (ImGui::Button("Sort")) {
            if(sortThread.joinable()) sortThread.join();
            sortThread = std::thread(Sort);
        }
    }

    void RenderWindows() {
        ImGuiWindowFlags winFlags = winFlags_ - ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;

        size_t winIndex = 0;

        for (auto [winName, isOpen] : windows_open) {
            auto currWindow = (*SWL)[winName];

            if (!isOpen || !currWindow) {
                continue;
            }

            ImVec2 pos( 20 + (winIndex % 3) * (winDim.x + 20),
                (winIndex < 3 ? winDim.y * winRatio.y : 1.1f * (winDim.y * winRatio.y) + winDim.y));

            ImVec2 dim(dspSize * winRatio);

            winDim = dim;

            currWindow->position() = pos;
            currWindow->dimension() = dim;

            if (!currWindow->Render(winFlags, renderWinLists)) {
                return;
            }

            winIndex++;
        }
    }
}

//*******************************************************************************************************\\
                                        Event Handler Functions
//*******************************************************************************************************\\

namespace MySrt{
    void Sort(){
        std::map<std::string, std::thread> threads;

        for (auto [winName, isOpen] : windows_open) {
            if (isOpen) {
                SortingWindow* currSW = SWL->operator[](winName);
                threads[winName] = std::thread(&SortingWindow::sortList, currSW);
            }
        }

        for (auto [winName, isOpen] : windows_open) {
            if (isOpen) {
                threads[winName].join();
            }
        }
    }

    void Reset(bool setNewVect) {
        renderWinLists = true;
        if (setNewVect) {
            std::vector<int> newVect;
            for (size_t i = 0; i < numItems; i++) newVect.push_back(generate() % (3 * numItems) + 1);
            
            SortingWindow::winStartList = newVect;
        }

        for (auto [winName, isOpen] : windows_open) {
            SortingWindow* currSW = SWL->operator[](winName);

            if (currSW && isOpen) {
                currSW->setList();
            }
        }
    }
}

//Helper functions
namespace MySrt{
    /*void sortList(SortingWindow& currWin) {
        std::function<void(std::vector<int>&)> currSortFunc = sort_funcs.at(currWin.sortFuncString);
        currSortFunc(currWin.list);
    }*/
}