#include<vector>

namespace mySort
{
template<typename T>
void insertionSort(std::vector<T> vect);

template<typename T>
void selectionSort(std::vector<T> vect);

template<typename T>
void bubbleSort(std::vector<T> vect);

template<typename T>
void quickSort(std::vector<T> vect);

template<typename T>
void mergeSort(std::vector<T> vect);

template<typename T>
void heapSort(std::vector<T> vect);
}