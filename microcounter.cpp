#include <iostream>
#include <thread>

unsigned int counter = 0;

void increment(void) {
	counter++;
}

int main(void) {
	for(unsigned int i = 0; i < 8; i++) {
		increment();
	}

	std::cout << counter << std::endl;
}
