#include <cstdio>
#include <chrono>

#include "StackVM.hpp"


int main(){
	// int n = 100'000'000;
	// float res = 4.0f;
	// int v = 3;
	// for(int i=0; i<n; ++i){
	// 	res -= 4.0f / v;
	// 	res += 4.0f / (v+2);
	// 	v += 4;
	// }
	//
	// rewrite loop
	//
	// do{
	//  ...;
	// 	--n;
	// }while(n > 0);
	StackVM vm({
		// int n = 100'000'000;
		PUSH, 100'000'000,
		STORE, 0,
		// float res = 4.0f;
		PUSH, IMM(4.0f),
		STORE, 1,
		// int v = 3;
		PUSH, IMM(3.0f),
		STORE, 2,
		// loop, address 12
		// res = (res - (4.0f / v)) + (4.0f / (v+2));
		LOAD, 1,
		PUSH, IMM(4.0f),
		LOAD, 2,
		DIVFP,
		SUBFP,
		PUSH, IMM(4.0f),
		LOAD, 2,
		PUSH, IMM(2.0f),
		ADDFP,
		DIVFP,
		ADDFP,
		STORE, 1,
		// v += 4
		LOAD, 2,
		PUSH, IMM(4.0f),
		ADDFP,
		STORE, 2,
		// n--
		LOAD, 0,
		PUSH, 1,
		SUB,
		STORE, 0,
		// n > 0
		LOAD, 0,
		PUSH, 0,
		ISGT,
		JIF, 12,

		// return res
		LOAD, 1,

		HALT
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

	return 0;
}
