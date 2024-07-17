#ifndef SORTING_WINDOW_H
#define SORTING_WINDOW_H

#include <chrono>

#include"sorting_algos.h"

namespace MySrt
{
	typedef std::chrono::high_resolution_clock HRC;

	struct dimensions { int x; int y; };
	struct position   { int x; int y; };

	class SortingWindow {
	private:
		std::vector<int> list;
		position pos;
		dimensions dim;
		HRC timer;

	public:
		SortingWindow(position, dimensions);
		void Render();
		bool friend colliding(const SortingWindow&, const SortingWindow&);

		static std::vector<SortingWindow>* SortingWindowList;
	};
}

#endif