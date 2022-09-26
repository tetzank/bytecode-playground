#pragma once

#include <vector>
#include <bit>


enum bytecode : int {
	PUSH,
	ADD, SUB, MUL, DIV,
	ADDFP, SUBFP, MULFP, DIVFP,
	NOT, AND, OR,
	ISEQ, ISGT, ISGE, ISLT, ISLE,
	JMP, JIF,
	LOAD, STORE,
	INC, DEC,
	JNZ, // jump if non-zero, does not pop operand
	HALT
};

inline int IMM(int   immediate){ return immediate; }
inline int IMM(float immediate){ return std::bit_cast<int>(immediate); }

class StackVM {
private:
	std::vector<int> instructions;
	//std::vector<int> stack;
	struct Stack {
		int data[128]; // fixed-size, requires check during assembling of bytecode
		int idx = 0;
		void push_back(int value){
			data[idx] = value;
			++idx;
		}
		int &back(){
			return data[idx-1];
		}
		void pop_back(){
			--idx;
		}
	};
	Stack stack;
	int locals[1024] = {}; //FIXME: fixed-size
	unsigned IP=0;
	bool halted=false;


	inline void exec_push();

	inline void exec_add();
	inline void exec_sub();
	inline void exec_mul();
	inline void exec_div();

	inline void exec_addfp();
	inline void exec_subfp();
	inline void exec_mulfp();
	inline void exec_divfp();

	inline void exec_not();
	inline void exec_and();
	inline void exec_or();

	inline void exec_iseq();
	inline void exec_isgt();
	inline void exec_isge();
	inline void exec_islt();
	inline void exec_isle();

	inline void exec_jmp();
	inline void exec_jif();

	inline void exec_load();
	inline void exec_store();

	inline void exec_inc();
	inline void exec_dec();
	inline void exec_jnz();

public:
	StackVM(std::initializer_list<int> &&instructions)
		: instructions(instructions) {}

	int run_switch();
	int run_goto();
	int run_direct();

	void reset(){
		IP = 0;
		halted = false;
	}
};
