#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "StackVM.hpp"


TEST_CASE("nested expression"){
	// (1 + 2 * 3) / 7 = 1
	StackVM vm({
		PUSH, 1,
		PUSH, 2,
		PUSH, 3,
		MUL,
		ADD,
		PUSH, 7,
		DIV,
		HALT
	});

	CHECK(vm.run_switch() == 1);
	vm.reset();
	CHECK(vm.run_goto() == 1);
}

TEST_CASE("loop"){
	// int a=6, b=4, total=0;
    // while (b >= 1) {
    //     total += a;
    //     --b;
	// }
	// basically total = a * b = 6 * 4 = 24
	StackVM vm({
		// a = 6;
		PUSH, 6,
		STORE, 0,
		// b = 4
		PUSH, 4,
		STORE, 1,
		// total = 0
		PUSH, 0,
		STORE, 2,

		// while condition
		// address 12
		LOAD, 1, // b
		PUSH, 1,
		ISGE,    // b >= 1
		NOT,     // b < 1
		JIF, 36, // jump to HALT label at the end

		// loop body
		// total = a + total
		LOAD, 0, // a
		LOAD, 2, // total
		ADD,     // a + total
		STORE, 2,
		// --b
		LOAD, 1, // b
		PUSH, 1,
		SUB,     // b - 1
		STORE, 1,

		JMP, 12, // jump to loop condition

		// address 36
		LOAD, 2,
		HALT
	});

	CHECK(vm.run_switch() == 24);
	vm.reset();
	CHECK(vm.run_goto() == 24);
}

TEST_CASE("loop2"){
	// same loop as above just written different in the available instr
	StackVM vm({
		// a = 6;
		PUSH, 6,
		STORE, 0,
		// b = 4
		PUSH, 4,
		STORE, 1,
		// total = 0
		PUSH, 0,
		STORE, 2,

		JMP, 28, // jump to loop condition at the end of loop

		// loop body
		// address 14
		// total = a + total
		LOAD, 0,
		LOAD, 2,
		ADD,
		STORE, 2,
		// --b
		LOAD, 1,
		PUSH, 1,
		SUB,
		STORE, 1,

		// condition
		// address 28
		LOAD, 1,
		PUSH, 1,
		ISGE,    // b >= 1
		JIF, 14, // 14 is beginning of loop

		LOAD, 2,
		HALT
	});

	CHECK(vm.run_switch() == 24);
	vm.reset();
	CHECK(vm.run_goto() == 24);
}
