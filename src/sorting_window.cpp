#include"sorting_window.h"
#include<stdexcept>

namespace MySrt
{
	std::map<std::string, SortingWindow*>* SortingWindow::SortingWindowList;

    SortingWindow::SortingWindow(ImVec2 p) : pos(p){
		*open = true;
		if (!SortingWindowList || SortingWindowList->size() > 0)
			throw;

		sortFunc = "Bubble Sort";
		sortFuncString = sortFunc;

		(*SortingWindowList)[sortFunc] = this;
	}
	SortingWindow::SortingWindow(ImVec2 p, char* str) : pos(p) , sortFunc(str), sortFuncString(str) {
		*open = true;

		if (!SortingWindowList || SortingWindowList->size() == 0) {
			(*SortingWindowList)[std::string(sortFunc)] = this;
		}else if (!SortingWindowList->count(str) && sort_funcs.count(str) && SortingWindowList->size() < sort_funcs.size()) {
			(*SortingWindowList)[std::string(sortFunc)] = this;
		}
		else {
			if(SortingWindowList->count(str))
				throw std::runtime_error("Window of " + std::string(str) + " already exists");
			if (!sort_funcs.count(str))
				throw std::runtime_error(std::string(str) + " not in sorting function list");
			if (SortingWindowList->size() >= sort_funcs.size())
				throw std::runtime_error("Maximum number of windows created");
		}
	}
	SortingWindow::SortingWindow(ImVec2 p, char* str, bool *isOpen) : pos(p), sortFunc(str), sortFuncString(str) {
		open = isOpen;

		if (SortingWindowList->size() == 0) {
			SortingWindowList->operator[](std::string(sortFunc)) = this;
		}
		else if (!SortingWindowList->count(str) && sort_funcs.count(str) && SortingWindowList->size() < sort_funcs.size()) {
			SortingWindowList->operator[](std::string(sortFunc)) = this;
		}
		else {
			if (SortingWindowList->count(str))
				throw std::runtime_error("Window of " + std::string(str) + " already exists");
			if (!sort_funcs.count(str))
				throw std::runtime_error(std::string(str) + " not in sorting function list");
			if (SortingWindowList->size() >= sort_funcs.size())
				throw std::runtime_error("Maximum number of windows created");
		}
	}
    
	void SortingWindow::Render(const ImVec2 dim)
	{
		ImGui::SetNextWindowSize(dim);
		ImGui::SetNextWindowPos(pos);
		ImGui::Begin(sortFunc, open);
		ImGui::End();
	}

	bool SortingWindow::Render(const ImVec2 dim, const ImGuiWindowFlags winFlags)
	{
		if (!*open) return false;

		ImGui::SetNextWindowSize(dim);
		ImGui::SetNextWindowPos(pos);
		
		ImGui::Begin(sortFunc, open, winFlags);

		printf("%s\n", sortFunc);
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Options")) 
				{
					if (ImGui::MenuItem("Sorting Function")) 
					{
						// Handle opening a file
					}
					if (ImGui::MenuItem("Close")) 
					{
						*open = false;
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Sorting Algorithm")) 
				{
					for (auto it : sort_funcs) {
						std::string sortFuncItem = it.first;

						if (!SortingWindowList->count(sortFuncItem)) {
							if (ImGui::MenuItem(sortFuncItem.c_str())) 
							{
								SortingWindowList->erase(sortFuncString);

								sortFuncString = sortFuncItem;
								sortFunc = sortFuncString.c_str();

								SortingWindowList->operator[](sortFuncString) = this;

								ImGui::EndMenu();
								ImGui::EndMenuBar();
								ImGui::End();
								return false;
							}
						}
					}

					//if (ImGui::MenuItem("Insertion Sort"))	{ sortFunc = "Insertion Sort"; }
					//if (ImGui::MenuItem("Selection Sort"))	{ sortFunc = "Selection Sort"; }
					//if (ImGui::MenuItem("Bubble Sort"))		{ sortFunc = "Bubble Sort"; }
					//if (ImGui::MenuItem("Merge Sort"))		{ sortFunc = "Merge Sort"; }
					//if (ImGui::MenuItem("Quick Sort"))		{ sortFunc = "Quick Sort"; }
					//if (ImGui::MenuItem("Heap Sort"))		{ sortFunc = "Heap Sort"; }

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
		}

		ImGui::End();

		return true;
	}

	ImVec2& SortingWindow::position() {
		return pos;
	}
	bool SortingWindow::isOpen() {
		return open;
	}
	bool colliding(const SortingWindow& a, const SortingWindow& b){ return a.sortFunc == b.sortFunc; }
}