#include"sorting_algos.h"

namespace MySrt
{
	std::map<std::string, void (*)(std::vector<int>&)> sort_funcs = {
			{"Bubble Sort", &bubbleSort},
			{"Insertion Sort", &insertionSort},
			{"Selection Sort", &selectionSort},
			{"Recursive Merge Sort", &mergeSortR},
			{"Iterative Merge Sort", &mergeSortI},
			{"Recursive Quick Sort", &quickSortR},
			{"Iterative Quick Sort", &quickSortI},
			{"Heap Sort", &heapSort}
	};
};