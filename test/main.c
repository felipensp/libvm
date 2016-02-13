#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

/* ADD instruction implementation */
void add_impl(VM_HANDLER_ARGS)
{
	if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
		VM_INT(result) = VM_INT(op1) + VM_INT(op2);
	} else {
		printf("Error: operation not implemented!");
	}
}

/* SUB instruction implementation */
void sub_impl(VM_HANDLER_ARGS)
{
	if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
		VM_INT(result) = VM_INT(op1) - VM_INT(op2);
	} else {
		printf("Error: operation not implemented!");
	}
}

/* MULT instruction implementation */
void mult_impl(VM_HANDLER_ARGS)
{
	if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
		VM_INT(result) = VM_INT(op1) * VM_INT(op2);
	} else {
		printf("Error: operation not implemented!");
	}
}

/* DIV instruction implementation */
void div_impl(VM_HANDLER_ARGS)
{
	if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
		VM_INT(result) = VM_INT(op1) / VM_INT(op2);
	} else {
		printf("Error: operation not implemented!");
	}
}

/* PRINT instruction implementation */
void print_impl(VM_HANDLER_ARGS)
{
	switch (VM_T(op1)) {
	case INT:	printf("%d\n", op1->value.vint); break;
	case STR: 	printf("%s\n", op1->value.vstr); break;
	case DBL:	printf("%.f\n", op1->value.vdouble); break;
	default:	break;
	}
}

int main(int argc, char **argv)
{
	int i;
	vm_env env;

	vm_init(&env);

	compile_file(&env, argv[1]);

	vm_add_inst(&env, (vm_inst){OP_HALT});

	for (i = 0; i < env.insts_count; ++i) {
		switch (env.insts[i].opcode) {
		case OP_PRINT: 	env.insts[i].handler = print_impl; 	break;
		case OP_ADD:  	env.insts[i].handler = add_impl;  	break;
		case OP_SUB:  	env.insts[i].handler = sub_impl;  	break;
		case OP_MULT:  	env.insts[i].handler = mult_impl;  	break;
		case OP_DIV:  	env.insts[i].handler = div_impl;  	break;
		default: break;
		}
	}

	vm_run(&env);

	return 0;
}
