#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

unsigned int counter = 0;
std::mutex counter_lock;

void increment(void) {
	counter_lock.lock();
	counter++;
	counter_lock.unlock();
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
