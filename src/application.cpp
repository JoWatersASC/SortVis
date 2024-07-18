#include"application.h"
#include"imgui_internal.h" //for debugging purposes, remove for build

namespace MySrt{
    const ImGuiWindowFlags winFlags_ = (
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    std::map<std::string, bool> windows_closed;
    ImVec2 winDim;
    bool cleaned;

    typedef std::map<std::string, SortingWindow*>::iterator SWL_itr;
    std::map<std::string, SortingWindow*>* SWL;
    SWL_itr swlEnd;

    //Place before main loop
    void Start() {
        cleaned = false;
        winDim = { 350, 275 };

        SortingWindow::SortingWindowList = new std::map<std::string, SortingWindow*>();

        windows_closed["Selection Sort"] = true;

        SortingWindow::SortingWindowList->operator[]("Selection Sort") =
            new SortingWindow({40, 180}, "Selection Sort", &windows_closed["Selection Sort"]);

        SWL = SortingWindow::SortingWindowList;

        swlEnd = SWL->end();
    }

    //Place inside main loop
    void Run(){
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

        RenderInputs();
        RenderWindows();
    }

    //Place after main loop
    void End() {
        SWL_itr it = SWL->begin();
        int size = SWL->size();
        int count = 0;

        while(count < size && it != swlEnd){
            delete it->second;
            count++;
            it++;
        }
        SortingWindow::SortingWindowList->clear();

        delete SortingWindow::SortingWindowList;
        cleaned = true;
    }

    void RenderMainMenuBar() {

    }
    void RenderMainMenuOptions() {
        if (ImGui::BeginMenu("Add Sorting Function"))
        {
            for (auto it : sort_funcs) {
                std::string sortFuncItem = it.first;

                if (SWL->count(sortFuncItem)) continue;

                if (ImGui::MenuItem(sortFuncItem.c_str()))
                {
                    windows_closed[sortFuncItem] = true;
                    SortingWindow::SortingWindowList->operator[](sortFuncItem) =
                        new SortingWindow({ 40 + SWL->size() * (winDim.x + 20), 180 },
                            sortFuncItem.c_str(), &windows_closed[sortFuncItem]);
                }
            }

            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Reset")){}
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

    void Sort(){}
}