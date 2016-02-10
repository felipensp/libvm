#include "vm.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

int main(int argc, char **argv)
{
	int val1 = atoi(argv[1]), val2 = atoi(argv[2]);
	vm_env env;

	vm_init(&env);

	vm_add_inst(&env, (vm_inst){
		.opcode = OP_PLUS,
		.op1 	= {CPOOL, vm_add_const(&env, INT, &val1)},
		.op2 	= {CPOOL, vm_add_const(&env, INT, &val2)},
		.result = vm_get_temp(&env)
	});

	vm_add_inst(&env, (vm_inst){OP_PRINT, {TEMP, vm_get_last_temp(&env)}});
	vm_add_inst(&env, (vm_inst){OP_HALT});

	vm_run(&env);

	return 0;
}
