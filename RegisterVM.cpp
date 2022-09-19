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

void RegisterVM::exec_mov(opcode instr){
	registerFile[instr.lhs] = getRightOperand(instr);
}
TEST_CASE("testing mov"){
	RegisterVM vm({
		{MOV, R(0), 88},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 88);
	vm.reset();
	CHECK(vm.run_goto() == 88);
}

void RegisterVM::exec_add(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) + getRightOperand(instr);
}
TEST_CASE("testing add"){
	RegisterVM vm({
		{MOV, R(0), 88},
		{ADD, R(0), 12},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 100);
	vm.reset();
	CHECK(vm.run_goto() == 100);
}

void RegisterVM::exec_sub(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) - getRightOperand(instr);
}
TEST_CASE("testing sub"){
	RegisterVM vm({
		{MOV, R(0), 88},
		{SUB, R(0), 12},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 76);
	vm.reset();
	CHECK(vm.run_goto() == 76);
}

void RegisterVM::exec_mul(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) * getRightOperand(instr);
}
TEST_CASE("testing mul"){
	RegisterVM vm({
		{MOV, R(0), 88},
		{MUL, R(0), 10},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 880);
	vm.reset();
	CHECK(vm.run_goto() == 880);
}

void RegisterVM::exec_div(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) / getRightOperand(instr);
}
TEST_CASE("testing div"){
	RegisterVM vm({
		{MOV, R(0), 12},
		{DIV, R(0), 3},
		{HALT, R(0), 0}
	});
	CHECK(vm.run_switch() == 4);
	vm.reset();
	CHECK(vm.run_goto() == 4);
}

void RegisterVM::exec_addfp(opcode instr){
	float lhs = std::bit_cast<float>(getLeftOperand(instr));
	float rhs = std::bit_cast<float>(getRightOperand(instr));
	registerFile[instr.lhs] = std::bit_cast<uint32_t>(lhs + rhs);
}
TEST_CASE("testing addfp"){
	RegisterVM vm({
		{MOV, R(0), 1.4f},
		{ADDFP, R(0), 2.6f},
		{HALT, R(0), 0}
	});
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(4.0f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(4.0f));
}

void RegisterVM::exec_subfp(opcode instr){
	float lhs = std::bit_cast<float>(getLeftOperand(instr));
	float rhs = std::bit_cast<float>(getRightOperand(instr));
	registerFile[instr.lhs] = std::bit_cast<uint32_t>(lhs - rhs);
}
TEST_CASE("testing subfp"){
	RegisterVM vm({
		{MOV, R(0), 1.4f},
		{SUBFP, R(0), 0.5f},
		{HALT, R(0), 0}
	});
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(0.9f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(0.9f));
}

void RegisterVM::exec_mulfp(opcode instr){
	float lhs = std::bit_cast<float>(getLeftOperand(instr));
	float rhs = std::bit_cast<float>(getRightOperand(instr));
	registerFile[instr.lhs] = std::bit_cast<uint32_t>(lhs * rhs);
}
TEST_CASE("testing mulfp"){
	RegisterVM vm({
		{MOV, R(0), 1.4f},
		{MULFP, R(0), 2.0f},
		{HALT, R(0), 0}
	});
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(2.8f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(2.8f));
}

void RegisterVM::exec_divfp(opcode instr){
	float lhs = std::bit_cast<float>(getLeftOperand(instr));
	float rhs = std::bit_cast<float>(getRightOperand(instr));
	registerFile[instr.lhs] = std::bit_cast<uint32_t>(lhs / rhs);
}
TEST_CASE("testing divfp"){
	RegisterVM vm({
		{MOV, R(0), 1.4f},
		{DIVFP, R(0), 0.7f},
		{HALT, R(0), 0}
	});
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(2.0f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(2.0f));
}

//TODO: add test cases
void RegisterVM::exec_cmpeq(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) == getRightOperand(instr);
}
void RegisterVM::exec_cmpgt(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) > getRightOperand(instr);
}
void RegisterVM::exec_cmpge(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) >= getRightOperand(instr);
}
void RegisterVM::exec_cmplt(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) < getRightOperand(instr);
}
void RegisterVM::exec_cmple(opcode instr){
	registerFile[instr.lhs] = getLeftOperand(instr) <= getRightOperand(instr);
}

void RegisterVM::exec_jmp(opcode instr){
	IP = getLeftOperand(instr);
}
void RegisterVM::exec_jif(opcode instr){
	if(getLeftOperand(instr)){
		IP = getRightOperand(instr);
	}
}


uint32_t RegisterVM::run_switch(){
	while(!halted){
		opcode instr = instructions[IP];
		switch(instr.op){
			case MOV: exec_mov(instr); break;
			case ADD: exec_add(instr); break;
			case SUB: exec_sub(instr); break;
			case MUL: exec_mul(instr); break;
			case DIV: exec_div(instr); break;

			// floating-point arithmetic
			case ADDFP: exec_addfp(instr); break;
			case SUBFP: exec_subfp(instr); break;
			case MULFP: exec_mulfp(instr); break;
			case DIVFP: exec_divfp(instr); break;

			//TODO: bool ops

			// comparisons using normal registers, not extra flags
			case CMPEQ: exec_cmpeq(instr); break;
			case CMPGT: exec_cmpgt(instr); break;
			case CMPGE: exec_cmpge(instr); break;
			case CMPLT: exec_cmplt(instr); break;
			case CMPLE: exec_cmple(instr); break;

			case JMP: exec_jmp(instr); break;
			case JIF: exec_jif(instr); break;

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

uint32_t RegisterVM::run_goto(){
	static void *dispatch_table[] = {
		&&do_mov, 
		&&do_add, &&do_sub, &&do_mul, &&do_div,
		&&do_addfp, &&do_subfp, &&do_mulfp, &&do_divfp,
		&&do_cmpeq, &&do_cmpgt, &&do_cmpge, &&do_cmplt, &&do_cmple,
		&&do_jmp, &&do_jif,
		&&do_halt
	};
#define DISPATCH goto *dispatch_table[instr.op]

	opcode instr=instructions[IP];
	DISPATCH;
	while(1){
		do_mov:   exec_mov(instr);   ++IP; instr=instructions[IP]; DISPATCH;
		do_add:   exec_add(instr);   ++IP; instr=instructions[IP]; DISPATCH;
		do_sub:   exec_sub(instr);   ++IP; instr=instructions[IP]; DISPATCH;
		do_mul:   exec_mul(instr);   ++IP; instr=instructions[IP]; DISPATCH;
		do_div:   exec_div(instr);   ++IP; instr=instructions[IP]; DISPATCH;
		do_addfp: exec_addfp(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_subfp: exec_subfp(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_mulfp: exec_mulfp(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_divfp: exec_divfp(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_cmpeq: exec_cmpeq(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_cmpgt: exec_cmpgt(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_cmpge: exec_cmpge(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_cmplt: exec_cmplt(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_cmple: exec_cmple(instr); ++IP; instr=instructions[IP]; DISPATCH;
		do_jmp:   exec_jmp(instr);   ++IP; instr=instructions[IP]; DISPATCH;
		do_jif:   exec_jif(instr);   ++IP; instr=instructions[IP]; DISPATCH;
		do_halt: {
			halted = true;
			break;
		}
	}
	return registerFile[0];
}
