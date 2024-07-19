#include<thread>
#include"application.h"

//Forward Declarations
namespace MySrt {
    static ImVec2 getWinDim(int numWindows, ImVec2 contentDim);
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

    //Holds window values - which windows are open(by sorting algo) and dimensions of window 
    static std::map<std::string, bool> windows_open;
    ImVec2 winDim;

    typedef std::map<std::string, SortingWindow*>::iterator SWL_itr;
    static std::map<std::string, SortingWindow*>* SWL;
    static SWL_itr swlEnd;

    //Random number generator
    static std::random_device rand_dev;
    static std::mt19937 generate(rand_dev());

    //Verifies that End() has been called after Start()
    bool cleaned;
}

//*******************************************************************************************************\\
                                    Main Application Functions
//*******************************************************************************************************\\
    
namespace MySrt {
    //Place before main loop
    void Start() {
        cleaned = false;
        winDim = { 350, 275 };
        dspSize = ImGui::GetIO().DisplaySize;

        windows_open["Selection Sort"] = true;

        SortingWindow::SortingWindowList = new std::map<std::string, SortingWindow*>();
        SortingWindow::SortingWindowList->operator[]("Selection Sort") =
            new SortingWindow({ 40, 180 }, "Selection Sort", &windows_open["Selection Sort"]);

        SWL = SortingWindow::SortingWindowList;

        swlEnd = SWL->end();

        Reset(true);

        SWL->operator[]("Selection Sort")->printList();
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
        int size = SWL->size();
        int count = 0;

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
                        new SortingWindow({ 40 + SWL->size() * (winDim.x + 20), 180 },
                            sortFuncItem.c_str(), &windows_open[sortFuncItem]);
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
        if (ImGui::Button("Sort")) Sort();
    }

    void RenderWindows() {
        ImGuiWindowFlags winFlags = winFlags_ - ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;

        SWL_itr it = SWL->begin();

        int winIndex = 0;
        while (it != swlEnd) {
            auto currWindow = it->second;

            ImVec2 pos(40 + winIndex * (20 + winDim.x), 180);
            currWindow->position() = pos;
            if (currWindow->Render(winDim, winFlags))
                it++;
            else
                break;

            winIndex++;
        }
    }
}

//*******************************************************************************************************\\
                                        Event Handler Functions
//*******************************************************************************************************\\

namespace MySrt{
    void Sort(){
        for (auto [winName, isOpen] : windows_open) {
            if (isOpen) {
                SortingWindow* currSW = SWL->operator[](winName);
                currSW->sortList();
            }
        }
    }

    void Reset(bool setNewVect) {
        if (setNewVect) {
            std::vector<int> newVect;
            for (int i = 0; i < numItems; i++) newVect.push_back(generate() % (3 * numItems));
            
            SortingWindow::winStartList = newVect;
        }

        for (auto [winName, isOpen] : windows_open) {
            SortingWindow* currSW= SWL->operator[](winName);

            if (isOpen) {
                currSW->setList();
                currSW->printList();
            }
        }
    }

    static void SortThreads() {

    }
}

//Helper functions
namespace MySrt{
    static ImVec2 getWinDim(int numWindows, ImVec2 contentDim) {

    }
}