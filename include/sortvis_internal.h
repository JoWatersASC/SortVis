#include <iostream>
#include <vector>
#include <cstdlib>

//Forward Declarations
template<class T>
inline std::ostream& print(const std::vector<T>&, std::ostream& out = std::cout);

template<class T>
inline T getMax(const std::vector<T>&);
template<class T>
inline T getMin(const std::vector<T>&);

inline void open_url(const std::string& url);


//Definitions
template<class T>
inline std::ostream& print(const std::vector<T>& vect, std::ostream& out){
    out << "[";
    for(T item : vect) out << item << ", ";
    out << "\b\b]";

    return out;
}

template<class T>
inline T getMax(const std::vector<T>& vect) {
    size_t size = vect.size();
    if (size == 0) throw;
    if (size == 1) return vect[0];

    T maximum = vect[0];

    for (int i = 0; i < size; i++) {
        if (vect[i] > maximum) maximum = vect[i];
    }

    return maximum;
}

template<class T>
inline T getMin(const std::vector<T>& vect) {
    size_t size = vect.size();
    if (size == 0) throw;
    if (size == 1) return vect[0];

    T minimum = vect[0];

    for (int i = 0; i < size; i++) {
        if (vect[i] < minimum) minimum = vect[i];
    }

    return minimum;
}


inline void open_url(const std::string& url) {
#if defined(_WIN64)
		system(("start " + url).c_str());
#elif defined(__linux__)
		system(("xdg-open " + url).c_str());
#endif
}