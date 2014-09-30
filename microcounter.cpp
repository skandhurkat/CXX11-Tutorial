#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>

void increment(
	unsigned int& counter,
	std::mutex& counter_lock) {

	counter_lock.lock();
	counter++;
	counter_lock.unlock();
}

int main(void) {

	unsigned int counter = 0;
	std::mutex counter_lock;

	std::vector<std::thread> threads;

	for(unsigned int i = 0; i < 8; i++) {
		threads.emplace_back(
			increment,
			std::ref(counter),
			std::ref(counter_lock));
	}

	for(auto& t: threads) {
		t.join();
	}

	std::cout << counter << std::endl;
}
