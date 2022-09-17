#include "RegisterVM.hpp"

#include <doctest/doctest.h>


uint32_t RegisterVM::getOperation(uint32_t instr){
	return instr >> 24;
}
uint32_t RegisterVM::getDst(uint32_t instr){
	return (instr >> 12) & 0xff;
}

uint32_t RegisterVM::getLeftOperand(uint32_t instr) const {
	// always register, no immediate possible
	return registerFile[getDst(instr)];
}
uint32_t RegisterVM::getRightOperand(uint32_t instr) const {
	if(instr & 0b1000'0000'0000){
		// immediate
		return instr & 0b111'1111'1111;
	}else{
		// register, load value
		return registerFile[instr & 0xff];
	}
}

TEST_CASE("testing mov"){
	RegisterVM vm({
		OP(MOV, R(0), IMM(88)),
		OP(HALT, 0, 0)
	});
	CHECK(vm.run_switch() == 88);
}

TEST_CASE("testing add"){
	RegisterVM vm({
		OP(MOV, R(0), IMM(88)),
		OP(ADD, R(0), IMM(12)),
		OP(HALT, 0, 0)
	});
	CHECK(vm.run_switch() == 100);
}

TEST_CASE("testing sub"){
	RegisterVM vm({
		OP(MOV, R(0), IMM(88)),
		OP(SUB, R(0), IMM(12)),
		OP(HALT, 0, 0)
	});
	CHECK(vm.run_switch() == 76);
}

TEST_CASE("testing mul"){
	RegisterVM vm({
		OP(MOV, R(0), IMM(88)),
		OP(MUL, R(0), IMM(10)),
		OP(HALT, 0, 0)
	});
	CHECK(vm.run_switch() == 880);
}

TEST_CASE("testing div"){
	RegisterVM vm({
		OP(MOV, R(0), IMM(12)),
		OP(DIV, R(0), IMM(3)),
		OP(HALT, 0, 0)
	});
	CHECK(vm.run_switch() == 4);
}


uint32_t RegisterVM::run_switch(){
	while(!halted){
		uint32_t instr = instructions[IP];
		switch(getOperation(instr)){
			case MOV: {
				registerFile[getDst(instr)] = getRightOperand(instr);
				break;
			}
			case ADD: {
				registerFile[getDst(instr)] = getLeftOperand(instr) + getRightOperand(instr);
				break;
			}
			case SUB: {
				registerFile[getDst(instr)] = getLeftOperand(instr) - getRightOperand(instr);
				break;
			}
			case MUL: {
				registerFile[getDst(instr)] = getLeftOperand(instr) * getRightOperand(instr);
				break;
			}
			case DIV: {
				registerFile[getDst(instr)] = getLeftOperand(instr) / getRightOperand(instr);
				break;
			}

			//TODO: FP arithmetic
			//TODO: bool ops
			//TODO: comparisons

			case JMP: {
				IP = getLeftOperand(instr);
				break;
			}
			case JIF: {
				if(getLeftOperand(instr)){
					IP = getRightOperand(instr);
				}
				break;
			}

			case HALT: {
				halted = true;
				break;
			}

			default:
				__builtin_trap(); // unsupported instruction
		}
		++IP;
	}
	return registerFile[0];
}
