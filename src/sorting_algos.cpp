#include"sorting_algos.h"

namespace MySrt
{
	//DOES NOT WORK IN LINUX
	//template<class T>
	//const std::map<std::string, std::function<void(std::vector<T>&)>> sort_funcs = {
	//		{"Bubble Sort", &bubbleSort},
	//		{"Insertion Sort", &insertionSort},
	//		{"Selection Sort", &selectionSort},
	//		{"Recursive Merge Sort", &mergeSortR},
	//		{"Iterative Merge Sort", &mergeSortI},
	//		{"Recursive Quick Sort", &quickSortR},
	//		{"Iterative Quick Sort", &quickSortI},
	//		{"Heap Sort", &heapSort<T>}
	//};

	const std::map<std::string, std::function<void(std::vector<int>&)>> sort_funcs = {
		{"Bubble Sort", &bubbleSort<int>},
		{"Insertion Sort", &insertionSort<int>},
		{"Selection Sort", &selectionSort<int>},
		{"Recursive Merge Sort", &mergeSortR<int>},
		{"Iterative Merge Sort", &mergeSortI<int>},
		{"Recursive Quick Sort", &quickSortR<int>},
		{"Iterative Quick Sort", &quickSortI<int>},
		{"Heap Sort", &heapSort<int>}
	};

	//DOES NOT WORK IN LINUX
	//template<> const std::map<std::string, std::function<void(std::vector<float>&)>> sort_funcs<float> = {
	//	{"Bubble Sort", &bubbleSort<float>},
	//	{ "Insertion Sort", &insertionSort<float> },
	//	{ "Selection Sort", &selectionSort<float> },
	//	{ "Recursive Merge Sort", &mergeSortR<float> },
	//	{ "Iterative Merge Sort", &mergeSortI<float> },
	//	{ "Recursive Quick Sort", &quickSortR<float> },
	//	{ "Iterative Quick Sort", &quickSortI<float> },
	//	{ "Heap Sort", &heapSort<float> }
	//};
};