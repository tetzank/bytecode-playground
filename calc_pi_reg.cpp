#include <cstdio>
#include <chrono>

#include "RegisterVM.hpp"


int main(){
	RegisterVM vm({
		{MOV, R(0), 4.0f}, // res = 4.0f
		{MOV, R(1), 100'000'000}, // n = 100'000'000
		{MOV, R(2), 3.0f}, // v = 3

		// loop, address 3
		{MOV,   R(3), 4.0f},
		{DIVFP, R(3), R(2)}, // 4.0f / v
		{SUBFP, R(0), R(3)}, // res -= (4.0f / v)
		{ADDFP, R(2), 2.0f}, // v += 2
		{MOV,   R(3), 4.0f},
		{DIVFP, R(3), R(2)},
		{ADDFP, R(0), R(3)}, // res += 4.0f / (v+2)

		{ADDFP, R(2), 2.0f}, // v += 2

		{SUB, R(1), 1}, // --n

		{CMPGT, R(1), 0},
		{JIF, R(-1), 2},

		{HALT, R(0), 0}
	});

	puts("switch statement");
	for(int i=0; i<5; ++i){
		auto t_start = std::chrono::high_resolution_clock::now();
		float res = std::bit_cast<float>(vm.run_switch());
		auto t_end = std::chrono::high_resolution_clock::now();
		printf("Wall clock time passed: %10.2f ms - result: %f\n",
			std::chrono::duration<double, std::milli>(t_end-t_start).count(),
			res
		);
		vm.reset();
	}

	puts("goto with manual jump table");
	for(int i=0; i<5; ++i){
		auto t_start = std::chrono::high_resolution_clock::now();
		float res = std::bit_cast<float>(vm.run_goto());
		auto t_end = std::chrono::high_resolution_clock::now();
		printf("Wall clock time passed: %10.2f ms - result: %f\n",
			std::chrono::duration<double, std::milli>(t_end-t_start).count(),
			res
		);
		vm.reset();
	}
}
