#pragma once

#include <cstdint>
#include <vector>
#include <bit>


enum ops : uint32_t {
	MOV,
	ADD, SUB, MUL, DIV,
	ADDFP, SUBFP, MULFP, DIVFP,
	//NOT, AND, OR,
	CMPEQ, CMPGT, CMPGE, CMPLT, CMPLE,
	JMP, JIF,
	HALT
};


// register index
struct R {
	int index;

	explicit R(int index) : index(index) {}
};

// encoding for the instructions, using bitfields for simplicity
struct opcode {
	unsigned op    :  8; // instruction id
	unsigned lhs   : 23; // also destination, hence always a register
	unsigned isimm :  1; // flag indicating if immediate or not
	unsigned rhs   : 32; // register or immediate (int or float)

	opcode(enum ops op, R dst, R src)
		: op(op), lhs(dst.index), isimm(0), rhs(src.index) {}
	opcode(enum ops op, R dst, int32_t imm)
		: op(op), lhs(dst.index), isimm(1), rhs(imm) {}
	opcode(enum ops op, R dst, float imm)
		: op(op), lhs(dst.index), isimm(1), rhs(std::bit_cast<uint32_t>(imm)) {}
};
static_assert(sizeof(opcode) == sizeof(uint64_t), "wrong size of opcode");


class RegisterVM {
private:
	std::vector<opcode> instructions;
	uint32_t registerFile[1024]; // fixed-size
	unsigned IP=0;
	bool cmpFlag=false;
	bool halted=false;

	inline uint32_t getLeftOperand(opcode instr) const;
	inline uint32_t getRightOperand(opcode instr) const;

	inline void exec_mov(opcode instr);

	inline void exec_add(opcode instr);
	inline void exec_sub(opcode instr);
	inline void exec_mul(opcode instr);
	inline void exec_div(opcode instr);

	inline void exec_addfp(opcode instr);
	inline void exec_subfp(opcode instr);
	inline void exec_mulfp(opcode instr);
	inline void exec_divfp(opcode instr);

	inline void exec_cmpeq(opcode instr);
	inline void exec_cmpgt(opcode instr);
	inline void exec_cmpge(opcode instr);
	inline void exec_cmplt(opcode instr);
	inline void exec_cmple(opcode instr);

	inline void exec_jmp(opcode instr);
	inline void exec_jif(opcode instr);

public:
	RegisterVM(std::initializer_list<opcode> &&instructions)
		: instructions(instructions) {}

	uint32_t run_switch();
	uint32_t run_goto();

	void reset(){
		IP = 0;
		halted = false;
	}
};
