#include <random>

#include"SortingVisualizer.h"

int main(int argc, char** argv) {
	auto f = std::async(std::launch::async, MySrt::forever);
	MySrt::RunVisualizer();

	return 0;
}