#include <iostream>
#include <thread>
#include <vector>

unsigned int counter = 0;

void increment(void) {
	counter++;
}

int main(void) {

	std::vector<std::thread> threads;

	for(unsigned int i = 0; i < 8; i++) {
		threads.emplace_back(increment);
	}

	for(auto& t: threads) {
		t.join();
	}

	std::cout << counter << std::endl;
}
