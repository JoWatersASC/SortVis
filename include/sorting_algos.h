#ifndef SORTING_ALGOS_H
#define SORTING_ALGOS_H

#include<thread>
#include<mutex>
#include<memory>
#include<stack>
#include<map>
#include<functional>

#include"sortvis_internal.h"

//----------------------
//Function Declarations
//----------------------
namespace MySrt {

	template<class T>
	void bubbleSort(std::vector<T>&);
	static std::mutex mtx;

	template<class T>
	void selectionSort(std::vector<T>&);

	template<class T>
	void insertionSort(std::vector<T>&);

	template<class T>
	void mergeSortR(std::vector<T>&);
	template<class T>
	void mergeSortI(std::vector<T>&);

	template<class T>
	void heapSort(std::vector<T>&);

	template<class T>
	void quickSortR(std::vector<T>&); //Recursive Quick Sort
	template<class T>
	void quickSortI(std::vector<T>&); //Iterative Quick Sort

	// Merge sort helper functions:
	// mergeI - merges sublists(iterative merge sort)
	// mergeS - Splits list into sublists, and merges sorted sublists(recursive merge sort)
	// merge - Fulfills role of merging lists passed through merge sort algo(recursive merge sort)
	template<class T>
	void mergeI(std::vector<T>&, int, int, int);
	template<class T>
	void mergeS(std::vector<T>&, int, int);
	template<class T>
	void merge(std::vector<T>&, int, int, int);

	// Quick sort helper function:
	// quickS - Recursively creates sublists to be partitioned & sorted, allows quickSortR to only take vector
	// partition - Sorts elements around pivot
	template<class T>
	void quickS(std::vector<T>&, int, int);
	template<class T>
	int partition(std::vector<T>&, int, int);

	//Heap sort helper function:
	//heapify - creates max heap of list and maintains order
	template<class T>
	void heapify(std::vector<T>&, int&);
	template<class T>
	void reheap(std::vector<T>&, int&, int);

	//Threading helper function, proportional to vector size
	static void waitTime(size_t s = 0);
	static void waitSwap();
	static void waitCompare();

	static bool waitOn = true;
}


//---------------------
//Function Definitions
//---------------------
namespace MySrt
{
	template<class T>
	void bubbleSort(std::vector<T>& vect) {
		const int size = vect.size();
		double count;

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size - i - 1; j++) {
				waitTime();
				if (vect[j] > vect[j + 1]) {
					std::swap(vect[j], vect[j + 1]);
					waitTime();
				}
			}
		}
	}

	template<class T>
	void selectionSort(std::vector<T>& vect) {
		const int size = vect.size();

		waitTime();
		for (int i = 0; i < size - 1; i++) {
			int min = i;
			waitTime();

			waitTime();
			for (int j = i; j < size; j++)
			{
				waitTime();
				if (vect[j] < vect[min]) {
					min = j;
					waitTime();
				}
			}

			waitTime();
			if (min != i) {
				waitTime();
				std::swap(vect[i], vect[min]);
			}
		}
	}

	template<class T>
	void insertionSort(std::vector<T>& vect) {
		const int size = vect.size();
		double count;

		for (int i = 0; i < size; i++) {
			T temp = vect[i];
			int j = i;
			waitTime();
			waitTime();

			waitTime();
			while (j > 0 && vect[j - 1] > temp) {
				vect[j] = vect[j - 1];
				waitTime();
				waitTime();
				j--;
			}
			vect[j] = temp;
			waitTime();
		}
	}

	template<class T>
	void mergeSortR(std::vector<T>& vect) {
		mergeS(vect, 0, vect.size() - 1);
	}

	template<class T>
	void mergeSortI(std::vector<T>& vect) {
		int size = vect.size();
		if (size <= 1) return;

		for (int sublistSize = 1; sublistSize <= size - 1; sublistSize *= 2) {
			for (int sublistIndex = 0; sublistIndex < size - 1; sublistIndex += 2 * sublistSize) {
				int mid = std::min(sublistSize + sublistIndex - 1, size - 1);
				int end = std::min(sublistIndex + 2 * sublistSize - 1, size - 1);
				waitTime();
				waitTime();

				mergeI(vect, sublistIndex, mid, end);
			}
		}

	}

	template<class T>
	void heapSort(std::vector<T>& vect) {
		//std::lock_guard<std::mutex> guard(mtx);
		int size = vect.size();

		heapify(vect, size);

		int i = size;
		waitTime();
		while (i > 1) {
			waitTime();
			i--;
			std::swap(vect[i], vect[0]);
			waitTime();
			reheap(vect, i, 0);
		}
	}

	template<class T>
	void quickSortI(std::vector<T>& vect) {

		std::stack<std::pair<int, int>> stk;
		stk.push({ 0, vect.size() - 1 });
		waitTime();

		while (!stk.empty()) {
			int lo = stk.top().first;
			int hi = stk.top().second;
			waitTime();
			waitTime();

			int left = lo + 1;
			int right = hi;
			int pivot = vect[lo];
			waitTime();
			waitTime();
			waitTime();

			stk.pop();

			waitTime();
			while (left < right) {
				waitTime();
				while (left < right && vect[left] <= pivot) {
					waitTime();
					left++;
				}
				waitTime();
				while (right > left && vect[right] > pivot) {
					waitTime();
					right--;
				}
				waitTime();
				if (left < right) {
					std::swap(vect[left], vect[right]);
					waitTime();
					left++;
					right--;
				}
			}

			waitTime();
			if (left == right && vect[left] > pivot) {
				right--;
			}
			std::swap(vect[lo], vect[right]);
			waitTime();


			waitTime();
			if (lo < right - 1) {
				stk.push({ lo, right - 1 });
				waitTime();
			}

			waitTime(); 
			if (left < hi) {
				stk.push({ left, hi });
				waitTime();
			}
		}
	}

	template<class T>
	void quickSortR(std::vector<T>& vect) {
		quickS(vect, 0, vect.size() - 1);
	}
};


//--------------------------------------
// Helper functions
//--------------------------------------
namespace MySrt {
	//--------------------------
	//Mergesort helper functions
	//--------------------------
	template<class T>
	void mergeI(std::vector<T>& vect, int l, int m, int r) {
		int i = 0, j = 0, k = l;
		waitTime();
		waitTime();
		waitTime();

		std::vector<T> llist(vect.begin() + l, vect.begin() + m + 1);
		std::vector<T> rlist(vect.begin() + m + 1, vect.begin() + r + 1);
		int llistSize = llist.size();
		int rlistSize = rlist.size();
		waitTime();
		waitTime();
		waitTime();
		waitTime();

		waitTime();
		while (i < llistSize && j < rlistSize) {
			waitTime();
			if (llist[i] < rlist[j]) {
				vect[k] = llist[i];
				waitTime();
				i++;
			}
			else {
				vect[k] = rlist[j];
				waitTime();
				j++;
			}
			k++;
		}

		waitTime();
		while (i < llistSize) {
			waitTime();
			vect[k] = llist[i];
			waitTime();
			k++;
			i++;
		}

		waitTime();
		while (j < rlistSize) {
			waitTime();
			vect[k] = rlist[j];
			waitTime();
			k++;
			j++;
		}
	}
	template<class T>
	void merge(std::vector<T>& vect, int l, int r, int mid) {
		if (l == r) return;

		int i = 0, j = 0, k = l;
		std::vector<T> left(vect.begin() + l, vect.begin() + mid + 1);
		std::vector<T> right(vect.begin() + mid + 1, vect.begin() + r + 1);

		while (i < left.size() && j < right.size()) {
			waitTime();
			if (left[i] <= right[j]) {
				vect[k] = left[i];
				waitTime();
				i++;
			}
			else {
				vect[k] = right[j];
				waitTime();
				j++;
			}
			k++;
		}

		waitTime();
		while (i < left.size()) {
			waitTime();
			vect[k] = left[i];
			waitTime();
			i++;
			k++;
		}

		waitTime();
		while (j < right.size()) {
			waitTime();
			vect[k] = right[j];
			waitTime();
			j++;
			k++;
		}
	}

	template<class T>
	void mergeS(std::vector<T>& vect, int l, int r) {
		if (l >= r) return;

		int mid = l + (r - l) / 2;
		waitTime();
		mergeS(vect, l, mid);
		mergeS(vect, mid + 1, r);

		merge(vect, l, r, mid);
		//waitTime(vect);
	}


	//--------------------------
	//Quicksort helper functions
	//--------------------------
	template<class T>
	void quickS(std::vector<T>& vect, int l, int r) {
		if (l >= r) return;

		std::unique_ptr<int> part(new int(partition(vect, l, r)));

		quickS(vect, l, *part - 1);
		quickS(vect, *part + 1, r);
	}
	template<class T>
	int partition(std::vector<T>& vect, int l, int r) {
		std::unique_ptr<int> mid(new int(l + (r - l) / 2));
		waitTime();

		std::swap(vect[*mid], vect[l]);
		waitTime();

		int pivot = vect[l];
		waitTime();
		std::unique_ptr<int> i(new int(r + 1));
		waitTime();

		waitTime();
		while (r > l) {
			waitTime();
			if (vect[r] > pivot) {
				(*i)--;
				std::swap(vect[r], vect[*i]);
				waitTime();
			}

			r--;
		}

		std::swap(vect[(*i) - 1], vect[l]);
		waitTime();
		return (*i) - 1;
	}

	template<class T>
	void heapify(std::vector<T>& vect, int& size) {
		int start = vect.size() / 2;
		waitTime();

		waitTime();
		while (start > 0) {
			start--;
			reheap(vect, size, start);
		}
	}
	template<class T>
	void reheap(std::vector<T>& vect, int& end, int root) {
		while (root * 2 + 1 < end) {
			int child = root * 2 + 1;

			if (child + 1 < end && vect[child] < vect[child + 1])
				child++;

			waitTime();
			if (vect[root] < vect[child]) {
				std::swap(vect[root], vect[child]);
				waitTime();
				root = child;
			}
			else {
				return;
			}
		}
	}

	static void waitTime(size_t s) {
		if(!waitOn) return;
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(100ns);
	}
	//static void waitSwap() {
	//	if (!waitOn) return;
	//	using namespace std::chrono_literals;
	//	std::this_thread::sleep_for(350ns);
	//}
	//static void waitCompare() {
	//	if (!waitOn) return;
	//	using namespace std::chrono_literals;
	//	std::this_thread::sleep_for(100ns);
	//}
	//static void waitAssign() {
	//	if (!waitOn) return;
	//	using namespace std::chrono_literals;
	//	std::this_thread::sleep_for(150ns);
	//}
}

//--------------------------
//Array of sorting functions
//--------------------------
namespace MySrt {
	//DOES NOT WORK IN LINUX
	//template<class T>
	//extern const std::map<std::string, std::function<void(std::vector<T>&)>> sort_funcs;
	extern const std::map<std::string, std::function<void(std::vector<int>&)>> sort_funcs;
}

#endif