#include"sorting_window.h"

namespace MySrt
{
    SortingWindow::SortingWindow(position p, dimensions d) : pos(p), dim(d){}
    
	void SortingWindow::Render(){}
	bool colliding(const SortingWindow&, const SortingWindow&){ return false; }
}