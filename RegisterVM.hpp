#pragma once

#include <cstdint>
#include <vector>


enum ops : uint32_t {
	MOV,
	ADD, SUB, MUL, DIV,
	ADDFP, SUBFP, MULFP, DIVFP,
	NOT, AND, OR,
	CMPEQ, CMPGT, CMPGE, CMPLT, CMPLE,
	JMP, JIF,
	HALT
};

#if 0
// bitfield
struct opcode {
	uint8_t op     :  8;
	unsigned lflag :  1;
	unsigned lhs   : 11;
	unsinged rflag :  1;
	unsigned rhs   : 11;
};
#endif

inline uint16_t R(int idx){
	return idx;
}

inline uint16_t IMM(uint16_t value){
	return (1 << 11) | (value & 0b111'1111'1111);
}

inline uint32_t OP(enum ops op, uint16_t dst, uint16_t src){
	return (op << 24) | (dst << 12) | src;
}

class RegisterVM {
private:
	std::vector<uint32_t> instructions;
	uint32_t registerFile[1024]; // fixed-size
	unsigned IP=0;
	bool halted=false;

	static inline uint32_t getOperation(uint32_t instr);
	static inline uint32_t getDst(uint32_t instr);
	       inline uint32_t getLeftOperand(uint32_t instr) const;
	       inline uint32_t getRightOperand(uint32_t instr) const;

public:
	RegisterVM(std::initializer_list<uint32_t> &&instructions)
		: instructions(instructions) {}

	uint32_t run_switch();
	
};
