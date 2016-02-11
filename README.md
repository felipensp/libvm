# libvm
Register-based VM as C library


####Current instruction set:####

| Opcode  | Description |
| ------------- | ------------- |
| OP_PLUS | Sums two operands |
| OP_PRINT | Prints something |
| OP_JMP | Jumps to somewhere |
| OP_HALT | Ends VM execution |

####Example####

```c
#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

/* PLUS instruction implementation */
void plus_impl(VM_HANDLER_ARGS)
{
	if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
		VM_INT(result) = VM_INT(op1) + VM_INT(op2);
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
	int val1 = atoi(argv[1]), val2 = atoi(argv[2]);
	vm_env env;

	vm_init(&env);

	vm_add_inst(&env, (vm_inst){
		.opcode  = OP_PLUS,
		.op1 	 = {CONST, vm_add_const(&env, INT, &val1)},
		.op2 	 = {CONST, vm_add_const(&env, INT, &val2)},
		.result  = vm_get_temp(&env),
		.handler = plus_impl
	});

	vm_add_inst(&env, (vm_inst){
		.opcode  = OP_PRINT,
		.op1 	 = {TEMP, vm_get_last_temp(&env)},
		.handler = print_impl
	});

	vm_add_inst(&env, (vm_inst){OP_HALT});

	vm_run(&env);
}
```
