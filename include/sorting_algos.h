#ifndef SORTING_ALGOS_H
#define SORTING_ALGOS_H

#include<stdlib.h>
#include<vector>
#include<stack>
#include<memory>
#include<map>
#include<functional>
#include<string>

//----------------------
//Function Declarations
//----------------------
namespace MySrt {

	template<class T>
	void bubbleSort(std::vector<T>&);

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
	void mergeS(std::vector<T>&, int, int, double&);
	template<class T>
	void merge(std::vector<T>&, int, int, int);
	template<class T>
	void merge(std::vector<T>&, int, int, int, double&);

	// Quick sort helper function:
	// quickS - Recursively creates sublists to be partitioned & sorted, allows quickSortR to only take vector
	// partition - Sorts elements around pivot
	template<class T>
	void quickS(std::vector<T>&, int, int);
	template<class T>
	void quickS(std::vector<T>&, int, int, double&);
	template<class T>
	int partition(std::vector<T>&, int, int);
	template<class T>
	int partition(std::vector<T>&, int, int, double&);

	//Heap sort helper function:
	//heapify - creates max heap of list and maintains order
	template<class T>
	void heapify(std::vector<T>& vect, int size, int maxIndex);
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
				if (vect[j] > vect[j + 1]) {
					std::swap(vect[j], vect[j + 1]);
				}
			}
		}
	}

	template<class T>
	void selectionSort(std::vector<T>& vect) {
		const int size = vect.size();
		double count;

		for (int i = 0; i < size - 1; i++) {
			int min = i;

			for (int j = i; j < size; j++)
			{
				if (vect[j] < vect[min]) {
					min = j;
				}
			}

			std::swap(vect[i], vect[min]);
		}
	}

	template<class T>
	void insertionSort(std::vector<T>& vect) {
		const int size = vect.size();
		double count;

		for (int i = 0; i < size - 1; i++) {
			int j = i + 1;

			while (j > 0 && vect[j] < vect[j - 1]) {
				std::swap(vect[j], vect[j - 1]);
				j--;
			}
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

		for (int sublistSize = 1; sublistSize < size - 1; sublistSize *= 2) {
			for (int sublistIndex = 0; sublistIndex < size - 1; sublistIndex += sublistSize) {
				int mid = std::min(sublistSize + sublistIndex - 1, size - 1);
				int end = std::min(sublistIndex + 2 * sublistSize - 1, size - 1);

				mergeI(vect, sublistIndex, mid, end);
			}
		}

	}

	template<class T>
	void heapSort(std::vector<T>& vect) {
		const int size = vect.size();

		for (int i = size / 2 - 1; i >= 0; i--) {
			heapify(vect, size, i);
		}

		for (int i = size - 1; i > 0; i--) {
			std::swap(vect[i], vect[0]);
			heapify(vect, i, 0);
		}
	}

	template<class T>
	void quickSortI(std::vector<T>& vect) {

		std::stack<std::pair<int, int>> stk;
		stk.push({ 0, vect.size() - 1 });

		while (!stk.empty()) {
			int lo = stk.top().first;
			int hi = stk.top().second;

			int left = lo + 1;
			int right = hi;
			int pivot = vect[lo];

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
		if (false) {
			double count = 0;
			quickS(vect, 0, vect.size() - 1, count);
		}
		quickS(vect, 0, vect.size() - 1);
	}
};


//--------------------------------------
// Helper functions without progress Bar
//--------------------------------------
namespace MySrt {
	//--------------------------
	//Mergesort helper functions
	//--------------------------
	template<class T>
	void mergeI(std::vector<T>& vect, int l, int m, int r) {
		int i = 0, j = 0, k = l;

		std::vector<T> llist(vect.begin() + l, vect.begin() + m);
		std::vector<T> rlist(vect.begin() + m, vect.begin() + r);
		int llistSize = llist.size();
		int rlistSize = rlist.size();

		while (i < llistSize && j < rlistSize) {
			if (llist[i] < rlist[j]) {
				vect[k++] = llist[i++];
			}
			else {
				vect[k++] = rlist[j++];
			}
		}
		while (i < llistSize) {
			vect[k++] = llist[i++];
		}
		while (j < rlistSize) {
			vect[k++] = rlist[j++];
		}
	}
	template<class T>
	void merge(std::vector<T>& vect, int l, int r, int mid) {
		if (l == r) return;

		int i = 0, j = 0, k = l;
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
	void mergeS(std::vector<T>& vect, int l, int r) {
		if (l >= r) return;

		int mid = l + (r - l) / 2;
		mergeS(vect, l, mid);
		mergeS(vect, mid + 1, r);

		merge(vect, l, r, mid);
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
		std::swap(vect[*mid], vect[l]);
		int pivot = vect[l];
		std::unique_ptr<int> i(new int(r + 1));

		while (r > l) {
			if (vect[r] > pivot) {
				(*i)--;
				std::swap(vect[r], vect[*i]);
			}

			r--;
		}

		std::swap(vect[(*i) - 1], vect[l]);
		return (*i) - 1;
	}
}



//--------------------------------------
// Helper functions with progress Bar
//--------------------------------------
namespace MySrt {
	//--------------------------
	//Mergesort helper functions
	//--------------------------
	template<class T>
	void merge(std::vector<T>& vect, int l, int r, int mid, double& count) {
		if (l == r) return;

		int i = 0, j = 0, k = l;
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
	void mergeS(std::vector<T>& vect, int l, int r, double& count) {
		if (l >= r) return;

		int mid = l + (r - l) / 2;
		mergeS(vect, l, mid);
		mergeS(vect, mid + 1, r);

		merge(vect, l, r, mid);
	}


	//--------------------------
	//Quicksort helper functions
	//--------------------------
	template<class T>
	void quickS(std::vector<T>& vect, int l, int r, double& count) {
		if (l >= r) return;

		int part = partition(vect, l, r);

		quickS(vect, l, part - 1);
		quickS(vect, part + 1, r);
	}
	template<class T>
	int partition(std::vector<T>& vect, int l, int r, double& count) {
		int pivot = vect[l];
		int i = r + 1;

		while (r > l) {
			if (vect[r] > pivot) {
				i--;
				std::swap(vect[r], vect[i]);
			}

			r--;
		}

		std::swap(vect[i - 1], vect[l]);
		return i - 1;
	}

	//---------------------------
	//Heap Sort helper functions
	//---------------------------
	template<class T>
	void heapify(std::vector<T>& vect, int size, int maxIndex) {
		int max = maxIndex;
		int lChildIndex = maxIndex * 2 + 1;
		int rChildIndex = maxIndex * 2 + 2;

		if (lChildIndex < size && vect[lChildIndex] > vect[max]) {
			max = lChildIndex;
		}
		if (rChildIndex < size && vect[rChildIndex] > vect[max]) {
			max = rChildIndex;
		}

		if (max != maxIndex) {
			std::swap(vect[max], vect[maxIndex]);
			heapify(vect, size, max);
		}
	}
}


//--------------------------
//Array of sorting functions
//--------------------------
namespace MySrt {
	//template<class T>
	extern std::map<std::string, void (*)(std::vector<int>&)> sort_funcs;
}

#endif