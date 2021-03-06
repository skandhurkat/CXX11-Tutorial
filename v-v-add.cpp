#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>
#include <stdexcept>

void v_v_add_serial(
	std::vector<int>& result,
	std::vector<int>& a,
	std::vector<int>& b) {

	if(a.size() not_eq b.size())
		throw std::range_error("a and b don't match in size");

	result.resize(a.size());

	for(size_t i = 0; i < a.size(); i++) {
		result[i] = a[i] + b[i];
	}
}

void v_v_add_parallel_frag(
	std::vector<int>& result,
	std::vector<int>& a,
	std::vector<int>& b,
	size_t thread_id,
	size_t total_size,
	size_t num_threads)
{
	size_t stride = total_size / num_threads;
	size_t start_index = thread_id * stride;
	size_t end_index   = (thread_id + 1) * stride;
	for(size_t i = start_index; i < end_index and i < total_size; i++) {
		result[i] = a[i] + b[i];
	}
}

void v_v_add_parallel(
	std::vector<int>& result,
	std::vector<int>& a,
	std::vector<int>& b)
{

	if(a.size() not_eq b.size())
		throw std::range_error("a and b don't match in size");

	result.resize(a.size());

	std::vector<std::thread> threads;

	size_t num_threads = std::thread::hardware_concurrency();

	for(size_t i = 0; i < num_threads; i++) {
		threads.emplace_back(
			v_v_add_parallel_frag,
			std::ref(result),
			std::ref(a),
			std::ref(b),
			i,
			a.size(),
			num_threads);
	}

	for(auto& t:threads) {
		t.join();
	}
}

int main(void) {

	static const unsigned int VECTOR_SIZE = 10000;

	std::vector<int> a(VECTOR_SIZE);
	std::vector<int> b(VECTOR_SIZE);
	std::vector<int> c(VECTOR_SIZE);
	std::vector<int> d(VECTOR_SIZE);

	for(size_t i = 0; i < VECTOR_SIZE; i++) {
		a[i] = b[i] = i;
	}

	auto start = std::chrono::steady_clock::now();
	v_v_add_serial(c, a, b);
	auto stop = std::chrono::steady_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
				stop - start);
	std::cout << "Elapsed (serial): " << elapsed.count()
		<< std::endl;

	start = std::chrono::steady_clock::now();
	v_v_add_parallel(d, a, b);
	stop = std::chrono::steady_clock::now();

	elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
				stop - start);
	std::cout << "Elapsed (parallel): " << elapsed.count()
		<< std::endl;

	for(size_t i = 0; i < VECTOR_SIZE; i++) {
		if(c[i] not_eq d[i])
			std::cout << "Mismatch at i=" << i << std::endl;
	}

return EXIT_SUCCESS;
}
