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
	SortingWindow::SortingWindow(ImVec2 p, const char* str, bool *isOpen) : pos(p),  sortFuncString(str) {
		open = isOpen;
		sortFunc = &sortFuncString[0];

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
		if (!*open) {
			SortingWindowList->erase(sortFuncString);
			return false;
		}

		ImGui::SetNextWindowSize(dim);
		ImGui::SetNextWindowPos(pos);
		
		ImGui::Begin(sortFunc, open, winFlags);

		//Menu Bar
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Options")) 
				{
					if (ImGui::MenuItem("link")) 
					{
						open_url("https://www.google.com");
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
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
		}

		ImGui::End();

		return true;
	}

	void SortingWindow::setList(const std::vector<int>& vect) {
		list = vect;
	}

	ImVec2& SortingWindow::position() {
		return pos;
	}
	bool SortingWindow::isOpen() {
		return open;
	}
	bool colliding(const SortingWindow& a, const SortingWindow& b){ return a.sortFunc == b.sortFunc; }
}

namespace MySrt {
	void open_url(const std::string& url) {
	#if defined(_WIN64)
			system(("start " + url).c_str());
	#elif defined(__linux__)
			system(("xdg-open " + url).c_str());
	#endif
	}
}