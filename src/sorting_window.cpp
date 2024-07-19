#include"sorting_window.h"
#include<stdexcept>

//*******************************************************************************************************\\
                                        Public Functions
//*******************************************************************************************************\\

namespace MySrt
{
	//static member declarations
	std::vector<int>					   SortingWindow::winStartList;
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
		
		ImGui::Begin(sortFunc, open, winFlags - ImGuiWindowFlags_NoResize);

		if(!RenderMenuBar()) 
			return false;

		//ImGui::BeginChild("Content Area", ImGui::GetWindowSize() - ImVec2(40, 20));
		RenderWinList();
		//ImGui::EndChild();

		ImGui::End();

		return true;
	}

	void SortingWindow::setList() { setList(winStartList); }
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

//*******************************************************************************************************\\
                                        Private Render Functions
//*******************************************************************************************************\\

namespace MySrt {
	bool SortingWindow::RenderMenuBar() {
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
		return true;
	}

	bool SortingWindow::RenderWinList() {
		int size = list.size();

		ImVec2 barPos;
		ImVec2 barDim; //barDim.y is max height of bar
		ImVec2 barOffset; // offset of window decorations and padding

		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winMin = ImGui::GetWindowContentRegionMin(); // upper left corner of content region relative to window
		ImVec2 winMax = ImGui::GetWindowContentRegionMax(); //lower right corner of content region relative to window
		ImVec2 winDim = ImGui::GetContentRegionAvail(); // available size of window - decorations

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 crs = ImGui::GetContentRegionAvail();

		barOffset = ImVec2(0.01f * winMax.x, winMax.y - winDim.y); // 1% of width and available content region offset from top
		barPos = winPos + barOffset;
		barDim = (winMax - winMin) * ImVec2(0.05f, 0.1f);

		draw_list->AddRectFilled(barPos, barPos + barDim, ImGui::GetColorU32(IM_COL32(0, 255, 0, 255)), 0, ImDrawFlags_RoundCornersNone);

		return true;
	}
}

//*******************************************************************************************************\\
                                        Helper Functions
//*******************************************************************************************************\\

namespace MySrt {
	void open_url(const std::string& url) {
#if defined(_WIN64)
		system(("start " + url).c_str());
#elif defined(__linux__)
		system(("xdg-open " + url).c_str());
#endif
	}

	void SortingWindow::sortList() {
		const std::function<void(std::vector<int>&)>* currSortFunc = &sort_funcs.at(sortFuncString);
		currSortFunc->operator()(list);
	}

	void SortingWindow::printList() {
		printf("[");
		for (auto num : list) {
			printf("%d, ", num);
		}
		printf("\b\b]\n");
	}
}

//experimental imvec2 algebra operators
ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
	return { a.x + b.x, a.y + b.y };
}
ImVec2 operator-(const ImVec2& a, const ImVec2& b) {
	return { a.x - b.x, a.y - b.y };
}
ImVec2 operator*(const ImVec2& vec, const ImVec2& factor) {
	return	{ vec.x * factor.x, vec.y * factor.y };
}
ImVec2 operator*(const ImVec2& vec, int a) {
	return	vec * ImVec2( a, a );
}