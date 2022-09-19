#include "RegisterVM.hpp"

#include <doctest/doctest.h>


uint32_t RegisterVM::getLeftOperand(opcode instr) const {
	// always register, no immediate possible
	return registerFile[instr.lhs];
}
uint32_t RegisterVM::getRightOperand(opcode instr) const {
	if(instr.isimm){
		// immediate
		return instr.rhs;
	}else{
		// register, load value
		return registerFile[instr.rhs];
	}
}

TEST_CASE("testing mov"){
	RegisterVM vm({
		{MOV, R(0), 88},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 88);
}

TEST_CASE("testing add"){
	RegisterVM vm({
		{MOV, R(0), 88},
		{ADD, R(0), 12},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 100);
}

TEST_CASE("testing sub"){
	RegisterVM vm({
		{MOV, R(0), 88},
		{SUB, R(0), 12},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 76);
}

TEST_CASE("testing mul"){
	RegisterVM vm({
		{MOV, R(0), 88},
		{MUL, R(0), 10},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 880);
}

TEST_CASE("testing div"){
	RegisterVM vm({
		{MOV, R(0), 12},
		{DIV, R(0), 3},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 4);
}

TEST_CASE("testing addfp"){
	RegisterVM vm({
		{MOV, R(0), 1.4f},
		{ADDFP, R(0), 2.6f},
		{HALT, R(0), 0}
	});
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(4.0f));
}

TEST_CASE("testing subfp"){
	RegisterVM vm({
		{MOV, R(0), 1.4f},
		{SUBFP, R(0), 0.5f},
		{HALT, R(0), 0}
	});
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(0.9f));
}

TEST_CASE("testing mulfp"){
	RegisterVM vm({
		{MOV, R(0), 1.4f},
		{MULFP, R(0), 2.0f},
		{HALT, R(0), 0}
	});
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(2.8f));
}

TEST_CASE("testing divfp"){
	RegisterVM vm({
		{MOV, R(0), 1.4f},
		{DIVFP, R(0), 0.7f},
		{HALT, R(0), 0}
	});
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(2.0f));
}


uint32_t RegisterVM::run_switch(){
	while(!halted){
		opcode instr = instructions[IP];
		switch(instr.op){
			case MOV: {
				registerFile[instr.lhs] = getRightOperand(instr);
				break;
			}
			case ADD: {
				registerFile[instr.lhs] = getLeftOperand(instr) + getRightOperand(instr);
				break;
			}
			case SUB: {
				registerFile[instr.lhs] = getLeftOperand(instr) - getRightOperand(instr);
				break;
			}
			case MUL: {
				registerFile[instr.lhs] = getLeftOperand(instr) * getRightOperand(instr);
				break;
			}
			case DIV: {
				registerFile[instr.lhs] = getLeftOperand(instr) / getRightOperand(instr);
				break;
			}

			// floating-point arithmetic
			case ADDFP: {
				float lhs = std::bit_cast<float>(getLeftOperand(instr));
				float rhs = std::bit_cast<float>(getRightOperand(instr));
				registerFile[instr.lhs] = std::bit_cast<uint32_t>(lhs + rhs);
				break;
			}
			case SUBFP: {
				float lhs = std::bit_cast<float>(getLeftOperand(instr));
				float rhs = std::bit_cast<float>(getRightOperand(instr));
				registerFile[instr.lhs] = std::bit_cast<uint32_t>(lhs - rhs);
				break;
			}
			case MULFP: {
				float lhs = std::bit_cast<float>(getLeftOperand(instr));
				float rhs = std::bit_cast<float>(getRightOperand(instr));
				registerFile[instr.lhs] = std::bit_cast<uint32_t>(lhs * rhs);
				break;
			}
			case DIVFP: {
				float lhs = std::bit_cast<float>(getLeftOperand(instr));
				float rhs = std::bit_cast<float>(getRightOperand(instr));
				registerFile[instr.lhs] = std::bit_cast<uint32_t>(lhs / rhs);
				break;
			}

			//TODO: bool ops

			// comparisons using normal registers, not extra flags
			case CMPEQ: {
				registerFile[instr.lhs] = getLeftOperand(instr) == getRightOperand(instr);
				break;
			}
			case CMPGT: {
				registerFile[instr.lhs] = getLeftOperand(instr) > getRightOperand(instr);
				break;
			}
			case CMPGE: {
				registerFile[instr.lhs] = getLeftOperand(instr) >= getRightOperand(instr);
				break;
			}
			case CMPLT: {
				registerFile[instr.lhs] = getLeftOperand(instr) < getRightOperand(instr);
				break;
			}
			case CMPLE: {
				registerFile[instr.lhs] = getLeftOperand(instr) <= getRightOperand(instr);
				break;
			}

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
