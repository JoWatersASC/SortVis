#include"application.h"

namespace MySrt{
    const ImGuiWindowFlags winFlags_ = (
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_MenuBar
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
        winDim = { 300, 250 };

        SortingWindow::SortingWindowList = new std::map<std::string, SortingWindow*>();

        windows_closed["Insertion Sort"] = true;
        windows_closed["Selection Sort"] = true;

        SortingWindow::SortingWindowList->operator[]("Insertion Sort") =
            new SortingWindow({40, 180}, "Insertion Sort", &windows_closed["Insertion Sort"]);

        SortingWindow::SortingWindowList->operator[]("Selection Sort") =
            new SortingWindow({ 380, 180 }, "Selection Sort", &windows_closed["Selection Sort"]);

        SWL = SortingWindow::SortingWindowList;

        swlEnd = SWL->end();
    }

    //Place inside main loop
    void Run(){
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImGui::SetNextWindowSize(displaySize);
        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::Begin("Application Window", NULL, winFlags_ | ImGuiWindowFlags_NoBringToFrontOnFocus);

        if (ImGui::BeginMainMenuBar()) 
        {
            if (ImGui::BeginMenu("Options")) {
                if (ImGui::MenuItem("Sorting Function")) {
                    // Handle opening a file
                }
                if (ImGui::MenuItem("Close")) {
                    //*open = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        {
            ImGuiWindowFlags winFlags = winFlags_ - ImGuiWindowFlags_NoTitleBar;

            SWL_itr it = SWL->begin();
            while (it != swlEnd) {
                if (it->second->Render(winDim, winFlags))
                    it++;
                else
                    break;
            }
        }

        ImGui::End();
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
}