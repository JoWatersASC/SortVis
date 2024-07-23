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
	void mergeI(std::vector<T>&, size_t, size_t, size_t);
	template<class T>
	void mergeS(std::vector<T>&, size_t, size_t);
	template<class T>
	void merge(std::vector<T>&, size_t, size_t, size_t);

	// Quick sort helper function:
	// quickS - Recursively creates sublists to be partitioned & sorted, allows quickSortR to only take vector
	// partition - Sorts elements around pivot
	template<class T>
	void quickS(std::vector<T>&, size_t, size_t);
	template<class T>
	size_t partition(std::vector<T>&, size_t, size_t);

	//Heap sort helper function:
	//heapify - creates max heap of list and maintains order
	template<class T>
	void heapify(std::vector<T>& vect, size_t, size_t);

	//Threading helper function, proportional to vector size
	template<class T>
	static void waitTime(std::vector<T>& vect);
}


//---------------------
//Function Definitions
//---------------------
namespace MySrt
{
	template<class T>
	void bubbleSort(std::vector<T>& vect) {
		const size_t size = vect.size();
		double count;

		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size - i - 1; j++) {
				if (vect[j] > vect[j + 1]) {
					std::swap(vect[j], vect[j + 1]);
					waitTime(vect);
				}
			}
		}
	}

	template<class T>
	void selectionSort(std::vector<T>& vect) {
		const size_t size = vect.size();
		double count;

		for (size_t i = 0; i < size - 1; i++) {
			size_t min = i;

			for (size_t j = i; j < size; j++)
			{
				if (vect[j] < vect[min]) {
					min = j;
				}
			}

			std::swap(vect[i], vect[min]);
			waitTime(vect);
		}
	}

	template<class T>
	void insertionSort(std::vector<T>& vect) {
		const size_t size = vect.size();
		double count;

		for (size_t i = 0; i < size - 1; i++) {
			size_t j = i + 1;

			while (j > 0 && vect[j] < vect[j - 1]) {
				std::swap(vect[j], vect[j - 1]);
				j--;
			}
			waitTime(vect);
		}
	}

	template<class T>
	void mergeSortR(std::vector<T>& vect) {
		mergeS(vect, 0, vect.size() - 1);
	}

	template<class T>
	void mergeSortI(std::vector<T>& vect) {
		size_t size = vect.size();
		if (size <= 1) return;

		for (size_t sublistSize = 1; sublistSize <= size - 1; sublistSize *= 2) {
			for (size_t sublistIndex = 0; sublistIndex < size - 1; sublistIndex += 2 * sublistSize) {
				size_t mid = std::min(sublistSize + sublistIndex - 1, size - 1);
				size_t end = std::min(sublistIndex + 2 * sublistSize - 1, size - 1);

				mergeI(vect, sublistIndex, mid, end);
				waitTime(vect);
			}
		}

	}

	template<class T>
	void heapSort(std::vector<T>& vect) {
		//std::lock_guard<std::mutex> guard(mtx);
		const size_t size = vect.size();

		for (int i = size / 2 - 1; i >= 0; i--) {
			heapify(vect, size, i);
		}

		for (int i = size - 1; i > 0; i--) {
			std::swap(vect[i], vect[0]);
			waitTime(vect);

			heapify(vect, i, 0);
		}
	}

	template<class T>
	void quickSortI(std::vector<T>& vect) {

		std::stack<std::pair<size_t, size_t>> stk;
		stk.push({ 0, vect.size() - 1 });

		while (!stk.empty()) {
			size_t lo = stk.top().first;
			size_t hi = stk.top().second;

			size_t left = lo + 1;
			size_t right = hi;
			size_t pivot = vect[lo];

			stk.pop();

			while (left < right) {
				while (left < right && vect[left] <= pivot)
					left++;
				while (right > left && vect[right] > pivot)
					right--;
				if (left < right) {
					std::swap(vect[left], vect[right]);
					left++;
					right--;
				}
			}

			if (left == right && vect[left] > pivot) right--;
			std::swap(vect[lo], vect[right]);

			if (lo < right - 1)
				stk.push({ lo, right - 1 });
			if (left < hi)
				stk.push({ left, hi });
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
	void mergeI(std::vector<T>& vect, size_t l, size_t m, size_t r) {
		size_t i = 0, j = 0, k = l;

		std::vector<T> llist(vect.begin() + l, vect.begin() + m + 1);
		std::vector<T> rlist(vect.begin() + m + 1, vect.begin() + r + 1);
		size_t llistSize = llist.size();
		size_t rlistSize = rlist.size();

		while (i < llistSize && j < rlistSize) {
			if (llist[i] < rlist[j]) {
				vect[k] = llist[i];
				i++;
			}
			else {
				vect[k] = rlist[j];
				j++;
			}
			k++;
		}
		while (i < llistSize) {
			vect[k] = llist[i];
			k++;
			i++;
		}
		while (j < rlistSize) {
			vect[k] = rlist[j];
			k++;
			j++;
		}
	}
	template<class T>
	void merge(std::vector<T>& vect, size_t l, size_t r, size_t mid) {
		if (l == r) return;

		size_t i = 0, j = 0, k = l;
		std::vector<T> left(vect.begin() + l, vect.begin() + mid + 1);
		std::vector<T> right(vect.begin() + mid + 1, vect.begin() + r + 1);

		while (i < left.size() && j < right.size()) {
			if (left[i] <= right[j]) {
				vect[k] = left[i];
				i++;
			}
			else {
				vect[k] = right[j];
				j++;
			}
			k++;
		}

		while (i < left.size()) {
			vect[k] = left[i];
			i++;
			k++;
		}

		while (j < right.size()) {
			vect[k] = right[j];
			j++;
			k++;
		}
	}

	template<class T>
	void mergeS(std::vector<T>& vect, size_t l, size_t r) {
		if (l >= r) return;

		size_t mid = l + (r - l) / 2;
		mergeS(vect, l, mid);
		mergeS(vect, mid + 1, r);

		merge(vect, l, r, mid);
		waitTime(vect);
	}


	//--------------------------
	//Quicksort helper functions
	//--------------------------
	template<class T>
	void quickS(std::vector<T>& vect, size_t l, size_t r) {
		if (l >= r) return;

		std::unique_ptr<size_t> part(new size_t(partition(vect, l, r)));

		waitTime(vect);

		quickS(vect, l, *part - 1);
		quickS(vect, *part + 1, r);
	}
	template<class T>
	size_t partition(std::vector<T>& vect, size_t l, size_t r) {
		std::unique_ptr<size_t> mid(new size_t(l + (r - l) / 2));
		std::swap(vect[*mid], vect[l]);
		size_t pivot = vect[l];
		std::unique_ptr<size_t> i(new size_t(r + 1));

		while (r > l) {
			if (vect[r] > pivot) {
				(*i)--;
				std::swap(vect[r], vect[*i]);
				waitTime(vect);
			}

			r--;
		}

		std::swap(vect[(*i) - 1], vect[l]);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return (*i) - 1;
	}

	template<class T>
	void heapify(std::vector<T>& vect, size_t size, size_t maxItemIndex) {
		size_t left  = maxItemIndex * 2 + 1;
		size_t right = maxItemIndex * 2 + 2;
		size_t newMaxItemIndex = maxItemIndex;
		
		if (left < size &&  vect[left]  > vect[newMaxItemIndex]) {
			newMaxItemIndex = left;
		}
		if (right < size && vect[right] > vect[newMaxItemIndex]) {
			newMaxItemIndex = right;
		}
		if (newMaxItemIndex != maxItemIndex) {
			std::swap(vect[maxItemIndex], vect[newMaxItemIndex]);
			//waitTime(vect);

			heapify(vect, size, newMaxItemIndex);
		}
	}

	template<class T>
	static void waitTime(std::vector<T>& vect) {
		//return;
		using namespace std::chrono_literals;
		size_t size = vect.size();

		if (size <= 5) {
			std::this_thread::sleep_for(500ms);
		}
		else if (size <= 20) {
			std::this_thread::sleep_for(200ms);
		}
		else if (size <= 10000) {
			std::this_thread::sleep_for(2ms);
		}
		else {
			std::this_thread::sleep_for(500us);
		}
	}
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