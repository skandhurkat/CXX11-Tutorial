#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <atomic>

void increment(std::atomic<unsigned int>& counter) {

	counter++;
}

int main(void) {

	std::atomic<unsigned int> counter(0);

	std::vector<std::thread> threads;

	for(unsigned int i = 0; i < 8; i++) {
		threads.emplace_back(
			increment,
			std::ref(counter));
	}

	for(auto& t: threads) {
		t.join();
	}

	std::cout << counter << std::endl;
}
