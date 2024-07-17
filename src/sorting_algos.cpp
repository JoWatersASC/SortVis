#include"sorting_algos.h"

namespace MySrt
{
	template<class T>
	std::map<std::string, std::function<void(std::vector<T>&)>> sort_funcs = {
			{"Bubble Sort", &bubbleSort<T>},
			{"Insertion Sort", &insertionSort<T>},
			{"Selection Sort", &selectionSort<T>},
			{"Recursive Merge Sort", &mergeSortR<T>},
			{"Iterative Merge Sort", &mergeSortI<T>},
			{"Recursive Quick Sort", &quickSortR<T>},
			{"Iterative Quick Sort", &quickSortI<T>},
			{"Heap Sort", &heapSort<T>}
	};

	template std::map<std::string, std::function<void(std::vector<int>&)>> sort_funcs<int>;
	template std::map<std::string, std::function<void(std::vector<std::string>&)>> sort_funcs<std::string>;
	template std::map<std::string, std::function<void(std::vector<float>&)>> sort_funcs<float>;
};