#include "StackVM.hpp"

#include <doctest/doctest.h>


void StackVM::exec_push(){
	stack.push_back(instructions[IP+1]);
	IP += 2;
}
TEST_CASE("testing push"){
	StackVM vm { PUSH, 88, HALT };
	CHECK(vm.run_switch() == 88);
	vm.reset();
	CHECK(vm.run_goto() == 88);
	vm.reset();
	CHECK(vm.run_direct() == 88);
}

void StackVM::exec_add(){
	//int op2 = stack.back(); stack.pop_back();
	//int op1 = stack.back(); stack.pop_back();
	//stack.push_back(op1 + op2);
	int op = stack.back(); stack.pop_back();
	stack.back() += op;
	++IP;
}
TEST_CASE("testing add"){
	StackVM vm { PUSH, 1, PUSH, 2, ADD, HALT };
	CHECK(vm.run_switch() == 3);
	vm.reset();
	CHECK(vm.run_goto() == 3);
	vm.reset();
	CHECK(vm.run_direct() == 3);
}

void StackVM::exec_sub(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 - op2);
	++IP;
}
TEST_CASE("testing sub"){
	StackVM vm { PUSH, 42, PUSH, 23, SUB, HALT };
	CHECK(vm.run_switch() == 19);
	vm.reset();
	CHECK(vm.run_goto() == 19);
	vm.reset();
	CHECK(vm.run_direct() == 19);
}

void StackVM::exec_mul(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 * op2);
	++IP;
}
TEST_CASE("testing mul"){
	StackVM vm { PUSH, 11, PUSH, 4, MUL, HALT };
	CHECK(vm.run_switch() == 44);
	vm.reset();
	CHECK(vm.run_goto() == 44);
	vm.reset();
	CHECK(vm.run_direct() == 44);
}

void StackVM::exec_div(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 / op2);
	++IP;
}
TEST_CASE("testing div"){
	StackVM vm { PUSH, 12, PUSH, 4, DIV, HALT };
	CHECK(vm.run_switch() == 3);
	vm.reset();
	CHECK(vm.run_goto() == 3);
	vm.reset();
	CHECK(vm.run_direct() == 3);
}

void StackVM::exec_addfp(){
	float op2 = std::bit_cast<float>(stack.back()); stack.pop_back();
	float op1 = std::bit_cast<float>(stack.back()); stack.pop_back();
	stack.push_back(std::bit_cast<int>(op1 + op2));
	++IP;
}
TEST_CASE("testing addfp"){
	StackVM vm { PUSH, IMM(1.4f), PUSH, IMM(2.6f), ADDFP, HALT };
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(4.0f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(4.0f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_direct()) == doctest::Approx(4.0f));
}

void StackVM::exec_subfp(){
	float op2 = std::bit_cast<float>(stack.back()); stack.pop_back();
	float op1 = std::bit_cast<float>(stack.back()); stack.pop_back();
	stack.push_back(std::bit_cast<int>(op1 - op2));
	++IP;
}
TEST_CASE("testing subfp"){
	StackVM vm { PUSH, IMM(1.4f), PUSH, IMM(0.5f), SUBFP, HALT };
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(0.9f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(0.9f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_direct()) == doctest::Approx(0.9f));
}

void StackVM::exec_mulfp(){
	float op2 = std::bit_cast<float>(stack.back()); stack.pop_back();
	float op1 = std::bit_cast<float>(stack.back()); stack.pop_back();
	stack.push_back(std::bit_cast<int>(op1 * op2));
	++IP;
}
TEST_CASE("testing mulfp"){
	StackVM vm { PUSH, IMM(1.4f), PUSH, IMM(2.0f), MULFP, HALT };
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(2.8f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(2.8f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_direct()) == doctest::Approx(2.8f));
}

void StackVM::exec_divfp(){
	float op2 = std::bit_cast<float>(stack.back()); stack.pop_back();
	float op1 = std::bit_cast<float>(stack.back()); stack.pop_back();
	stack.push_back(std::bit_cast<int>(op1 / op2));
	++IP;
}
TEST_CASE("testing divfp"){
	StackVM vm { PUSH, IMM(1.4f), PUSH, IMM(0.7f), DIVFP, HALT };
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(2.0f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(2.0f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_direct()) == doctest::Approx(2.0f));
}

void StackVM::exec_not(){
	int op = stack.back(); stack.pop_back();
	stack.push_back(!op);
	++IP;
}
TEST_CASE("testing not"){
	{
		StackVM vm { PUSH, 0, NOT, HALT };
		CHECK(vm.run_switch() == 1);
		vm.reset();
		CHECK(vm.run_goto() == 1);
		vm.reset();
		CHECK(vm.run_direct() == 1);
	}
	{
		StackVM vm { PUSH, 1, NOT, HALT };
		CHECK(vm.run_switch() == 0);
		vm.reset();
		CHECK(vm.run_goto() == 0);
		vm.reset();
		CHECK(vm.run_direct() == 0);
	}
}

void StackVM::exec_and(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 && op2);
	++IP;
}
//TODO: more tests

void StackVM::exec_or(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 || op2);
	++IP;
}

void StackVM::exec_iseq(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 == op2);
	++IP;
}

void StackVM::exec_isgt(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 > op2);
	++IP;
}

void StackVM::exec_isge(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 >= op2);
	++IP;
}

void StackVM::exec_islt(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 < op2);
	++IP;
}

void StackVM::exec_isle(){
	int op2 = stack.back(); stack.pop_back();
	int op1 = stack.back(); stack.pop_back();
	stack.push_back(op1 <= op2);
	++IP;
}

void StackVM::exec_jmp(){
	IP = instructions[IP+1];
}

void StackVM::exec_jif(){
	int op = stack.back(); stack.pop_back();
	if(op){
		IP = instructions[IP+1];
	}else{
		IP += 2;
	}
}

void StackVM::exec_load(){
	int idx = instructions[IP+1];
	// load local variable and push on stack
	stack.push_back(locals[idx]);
	IP += 2;
}

void StackVM::exec_store(){
	int op = stack.back(); stack.pop_back();
	int idx = instructions[IP+1];
	// store top of stack to local variable
	locals[idx] = op;
	IP += 2;
}

void StackVM::exec_inc(){
	stack.back() += 1;
	++IP;
}

void StackVM::exec_dec(){
	stack.back() -= 1;
	++IP;
}

void StackVM::exec_jnz(){
	int op = stack.back(); // does not pop
	if(op != 0){
		IP = instructions[IP+1];
	}else{
		IP += 2;
	}
}

TEST_CASE("load and store"){
	StackVM vm { PUSH, IMM(1.4f), STORE, 0, LOAD, 0, HALT };
	CHECK(std::bit_cast<float>(vm.run_switch()) == doctest::Approx(1.4f));
	vm.reset();
	CHECK(std::bit_cast<float>(vm.run_goto()) == doctest::Approx(1.4f));
}


int StackVM::run_switch(){
	while(!halted){
		switch(instructions[IP]){
			case PUSH:  exec_push();  break;
			case ADD:   exec_add();   break;
			case SUB:   exec_sub();   break;
			case MUL:   exec_mul();   break;
			case DIV:   exec_div();   break;
			case ADDFP: exec_addfp(); break;
			case SUBFP: exec_subfp(); break;
			case MULFP: exec_mulfp(); break;
			case DIVFP: exec_divfp(); break;
			case NOT:   exec_not();   break;
			case AND:   exec_and();   break;
			case OR:    exec_or();    break;
			case ISEQ:  exec_iseq();  break;
			case ISGT:  exec_isgt();  break;
			case ISGE:  exec_isge();  break;
			case ISLT:  exec_islt();  break;
			case ISLE:  exec_isle();  break;
			case JMP:   exec_jmp();   break;
			case JIF:   exec_jif();   break;
			case LOAD:  exec_load();  break;
			case STORE: exec_store(); break;
			case INC:   exec_inc();   break;
			case DEC:   exec_dec();   break;
			case JNZ:   exec_jnz();   break;
			case HALT: {
				halted = true;
				break;
			}

			default:
				__builtin_trap(); // unsupported instruction
		}
	}
	return stack.back();
}

int StackVM::run_goto(){
	static void *dispatch_table[] = {
		&&do_push, 
		&&do_add, &&do_sub, &&do_mul, &&do_div,
		&&do_addfp, &&do_subfp, &&do_mulfp, &&do_divfp,
		&&do_not, &&do_and, &&do_or,
		&&do_iseq, &&do_isgt, &&do_isge, &&do_islt, &&do_isle,
		&&do_jmp, &&do_jif,
		&&do_load, &&do_store,
		&&do_inc, &&do_dec,
		&&do_jnz,
		&&do_halt
	};
#define DISPATCH goto *dispatch_table[instructions[IP]]

	DISPATCH;
	while(1){
		do_push:  exec_push();  DISPATCH;
		do_add:   exec_add();   DISPATCH;
		do_sub:   exec_sub();   DISPATCH;
		do_mul:   exec_mul();   DISPATCH;
		do_div:   exec_div();   DISPATCH;
		do_addfp: exec_addfp(); DISPATCH;
		do_subfp: exec_subfp(); DISPATCH;
		do_mulfp: exec_mulfp(); DISPATCH;
		do_divfp: exec_divfp(); DISPATCH;
		do_not:   exec_not();   DISPATCH;
		do_and:   exec_and();   DISPATCH;
		do_or:    exec_or();    DISPATCH;
		do_iseq:  exec_iseq();  DISPATCH;
		do_isgt:  exec_isgt();  DISPATCH;
		do_isge:  exec_isge();  DISPATCH;
		do_islt:  exec_islt();  DISPATCH;
		do_isle:  exec_isle();  DISPATCH;
		do_jmp:   exec_jmp();   DISPATCH;
		do_jif:   exec_jif();   DISPATCH;
		do_load:  exec_load();  DISPATCH;
		do_store: exec_store(); DISPATCH;
		do_inc:   exec_inc();   DISPATCH;
		do_dec:   exec_dec();   DISPATCH;
		do_jnz:   exec_jnz();   DISPATCH;
		do_halt: {
			halted = true;
			break;
		}
	}
#undef DISPATCH
	return stack.back();
}

int StackVM::run_direct(){
	using Instr = void*;
	static Instr dispatch_table[] = {
		&&do_push, 
		&&do_add, &&do_sub, &&do_mul, &&do_div,
		&&do_addfp, &&do_subfp, &&do_mulfp, &&do_divfp,
		&&do_not, &&do_and, &&do_or,
		&&do_iseq, &&do_isgt, &&do_isge, &&do_islt, &&do_isle,
		&&do_jmp, &&do_jif,
		&&do_load, &&do_store,
		&&do_inc, &&do_dec,
		&&do_jnz,
		&&do_halt
	};

	Instr *program = new Instr[instructions.size()];
	Instr *dip = program;

#define CPY(x) program[IP] = dispatch_table[x]
#define CPYPARAM() ++IP; program[IP] = (void*)instructions[IP]
	while(!halted){
		switch(instructions[IP]){
			case PUSH:  CPY(PUSH);  CPYPARAM(); break;
			case ADD:   CPY(ADD);               break;
			case SUB:   CPY(SUB);               break;
			case MUL:   CPY(MUL);               break;
			case DIV:   CPY(DIV);               break;
			case ADDFP: CPY(ADDFP);             break;
			case SUBFP: CPY(SUBFP);             break;
			case MULFP: CPY(MULFP);             break;
			case DIVFP: CPY(DIVFP);             break;
			case NOT:   CPY(NOT);               break;
			case AND:   CPY(AND);               break;
			case OR:    CPY(OR);                break;
			case ISEQ:  CPY(ISEQ);              break;
			case ISGT:  CPY(ISGT);              break;
			case ISGE:  CPY(ISGE);              break;
			case ISLT:  CPY(ISLT);              break;
			case ISLE:  CPY(ISLE);              break;
			case JMP:   CPY(JMP);   CPYPARAM(); break;
			case JIF:   CPY(JIF);   CPYPARAM(); break;
			case LOAD:  CPY(LOAD);  CPYPARAM(); break;
			case STORE: CPY(STORE); CPYPARAM(); break;
			case INC:   CPY(INC);               break;
			case DEC:   CPY(DEC);               break;
			case JNZ:   CPY(JNZ);   CPYPARAM(); break;
			case HALT:  CPY(HALT); halted=true; break;

			default:
				__builtin_trap(); // unsupported instruction
		}
		++IP;
	}
#undef CPY
#undef CPYPARAM

#define DISPATCH goto **dip
#define OP int op=stack.back(); stack.pop_back()
#define PARAM *(int*)(dip+1)
#define OPFP(operation) \
	float op2=std::bit_cast<float>(stack.back()); \
	stack.pop_back(); \
	float op1=std::bit_cast<float>(stack.back()); \
	stack.back() = std::bit_cast<int>(op1 operation op2);

	DISPATCH;
	while(1){
		do_push:  { stack.push_back(PARAM); dip+=2; DISPATCH; }
		do_add:   { OP; stack.back() += op; ++dip; DISPATCH; }
		do_sub:   { OP; stack.back() -= op; ++dip; DISPATCH; }
		do_mul:   { OP; stack.back() *= op; ++dip; DISPATCH; }
		do_div:   { OP; stack.back() /= op; ++dip; DISPATCH; }
		do_addfp: { OPFP(+); ++dip; DISPATCH; }
		do_subfp: { OPFP(-); ++dip; DISPATCH; }
		do_mulfp: { OPFP(*); ++dip; DISPATCH; }
		do_divfp: { OPFP(/); ++dip; DISPATCH; }
		do_not:   { stack.back() = !stack.back(); ++dip; DISPATCH; }
		do_and:   { OP; stack.back() = stack.back() && op; ++dip; DISPATCH; }
		do_or:    { OP; stack.back() = stack.back() || op; ++dip; DISPATCH; }
		do_iseq:  { OP; stack.back() = stack.back() == op; ++dip; DISPATCH; }
		do_isgt:  { OP; stack.back() = stack.back() >  op; ++dip; DISPATCH; }
		do_isge:  { OP; stack.back() = stack.back() >= op; ++dip; DISPATCH; }
		do_islt:  { OP; stack.back() = stack.back() <  op; ++dip; DISPATCH; }
		do_isle:  { OP; stack.back() = stack.back() <= op; ++dip; DISPATCH; }
		do_jmp:   { dip = program + PARAM; DISPATCH; }
		do_jif:   { OP; if(op) dip = program + PARAM; else dip+=2;  DISPATCH; }
		do_load:  { stack.push_back(locals[PARAM]); dip+=2; DISPATCH; }
		do_store: { OP; locals[PARAM] = op; dip+=2; DISPATCH; }
		do_inc:   { stack.back() += 1; ++dip; DISPATCH; }
		do_dec:   { stack.back() -= 1; ++dip; DISPATCH; }
		do_jnz:   { if(stack.back() != 0) dip = program + PARAM; else dip+=2; DISPATCH; }
		do_halt: {
			halted = true;
			break;
		}
	}
#undef DISPATCH
#undef OP
#undef PARAM
#undef OPFP

	delete[] program;
	return stack.back();
}
