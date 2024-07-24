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

    SortingWindow::SortingWindow(ImVec2 p) : pos(p) {
		*open = true;
		state = window_state::UNSORTED;

		if (!SortingWindowList || SortingWindowList->size() > 0)
			throw;

		sortFunc = "Bubble Sort";
		sortFuncString = sortFunc;

		(*SortingWindowList)[sortFunc] = this;
	}
	SortingWindow::SortingWindow(ImVec2 p, char* str) : pos(p) , sortFunc(str), sortFuncString(str) {
		*open = true;
		state = window_state::UNSORTED;

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
		state = window_state::UNSORTED;

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

	bool SortingWindow::Render(const ImGuiWindowFlags winFlags, bool& renderList)
	{
		if (!*open) {
			SortingWindowList->erase(sortFuncString);
			return false;
		}

		ImGui::SetNextWindowSize(dim);
		ImGui::SetNextWindowPos(pos);
		
		ImGui::Begin(sortFunc, open, winFlags);

		if(!RenderMenuBar()) 
			return false;

		RenderWinList();

		if (state != window_state::UNSORTED)
			RenderDuration();

		ImGui::End();

		return true;
	}

	ImVec2& SortingWindow::position() {
		return pos;
	}
	ImVec2& SortingWindow::dimension() {
		return dim;
	}
	std::string SortingWindow::getName() { return sortFuncString; }
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
			if (ImGui::BeginMenu("Choose Sorting Algorithm"))
			{
				for (auto it : sort_funcs) {
					std::string sortFuncItem = it.first;

					if (!SortingWindowList->count(sortFuncItem)) {
						if (ImGui::MenuItem(sortFuncItem.c_str()))
						{
							SortingWindowList->erase(sortFuncString);
							windows_open[sortFuncString] = false;
							windows_open.erase(sortFuncString);
							
							sortFuncString = sortFuncItem;
							sortFunc = sortFuncString.c_str();

							windows_open[sortFuncString] = true;


							SortingWindowList->operator[](sortFuncString) = this;

							//printf("%s\n", sortFuncString.c_str());

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
		size_t size = list.size();
		if (!size) return size;

		ImVec2 barPos;
		ImVec2 barDim; //barDim.y is max height of bar
		ImVec2 barOffset; // offset of window decorations and padding

		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		ImVec2 contentPadding(0, 0.7f * (dim.y - contentRegion.y)); //available content region offset from top
		ImVec2 contentPos = pos + contentPadding;
		ImVec2 contentDim = dim - contentPadding;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		//barPos = winPos + barOffset;
		float listMax = getMax(list);
		float listMin = getMin(list);
		

		barPos = contentPos + (contentDim * ImVec2(0.05f, 0.0f));
		barDim = ImVec2(); // barDim.y is minimum size of a bar

		for (int i = 0; i < list.size(); i++) {
			barPos = barPos + barDim * ImVec2(1.0f, 0);
			barDim = contentDim * ImVec2(0.9f / (float)list.size(), ((float)list[i] / listMax) * 0.95f);

			draw_list->AddRectFilled(barPos, barPos + barDim, ImGui::GetColorU32(IM_COL32(0, 255, 0, 255)), 0, ImDrawFlags_RoundCornersNone);

			////printf("\nItem Ratio: %0.4f\n", itemRatio);
			//std::cout << barPos.x << ", " << "==>" << (float)list[i] << ", " << barDim.y << std::endl;
			//std::cout << barPos.x << ", " << barPos.y << "==>" << contentPos.y << ", " << contentDim.y << std::endl;
		}

		return true;
	}

	bool SortingWindow::RenderDuration() {
		if (state == window_state::UNSORTED) return false;
		std::string durationText = "0";
		std::string suffix = "ns";

		if (duration.count() >= 1000000) {
			std::chrono::duration<double> duration_s(duration);

			durationText = std::to_string(duration_s.count());
			durationText.erase(durationText.end() - 3, durationText.end());
			suffix = "s";
		}
		else if(duration.count() >= 1000) {
			std::chrono::duration<double, std::milli> duration_ms(duration);

			durationText = std::to_string(duration_ms.count());
			durationText.erase(durationText.end() - 3, durationText.end());
			suffix = "ms";
		}
		else if(duration.count() >= 1){
			durationText = std::to_string(duration.count());
			durationText.erase(durationText.end() - 3, durationText.end());
			suffix = "us";
		}

		if (state == window_state::SORTING) {
			durationText = "Execution Time: " + durationText + suffix;
		}
		else if (state == window_state::SORTED) {
			durationText = sortFuncString + " sorted " + std::to_string(list.size()) + " elements in " + durationText + suffix;
		}

		auto winDim = ImGui::GetWindowSize();
		auto textWidth = ImGui::CalcTextSize(durationText.c_str()).x;

		ImGui::SetCursorPosX((winDim.x - textWidth) * 0.5f);
		ImGui::SetCursorPosY(winDim.y * 0.95f);

		ImGui::Text(durationText.c_str());

		return true;
	}
}

//*******************************************************************************************************\\
                                        List Functions
//*******************************************************************************************************\\

namespace MySrt {
	void SortingWindow::setList() { setList(winStartList); }
	void SortingWindow::setList(const std::vector<int>& vect) { list = vect; }

	
	void SortingWindow::sortList() {
		state = window_state::SORTING;
		duration = std::chrono::duration<double, std::micro>::zero();

		std::thread timer([this]() {
			auto start = HRC::now();
			while (state != window_state::SORTED) {
				duration = HRC::now() - start;
			}
		});

		const std::function<void(std::vector<int>&)>* currSortFunc = &sort_funcs.at(sortFuncString);
		currSortFunc->operator()(list);

		state = window_state::SORTED;

		if(timer.joinable()) timer.join();
	}

	void SortingWindow::printList() {
		print(list);
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
ImVec2 operator*(const ImVec2& vec, float factor) {
	return	vec * ImVec2( factor, factor );
}
ImVec2 operator/(const ImVec2& vec, const ImVec2& divisor) {
	return { vec.x / divisor.x, vec.y / divisor.y };
}
ImVec2 operator/(const ImVec2& vec, float divisor) {
	return vec / ImVec2(divisor, divisor);
}