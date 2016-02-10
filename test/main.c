#include "vm.h"

int main(int argc, char **argv)
{
	int id1, id2;
	int val1 = 1, val2 = 2;
	vm_env env;

	vm_init(&env);
	id1 = vm_add_constant(&env, INT, &val1);
	id2 = vm_add_constant(&env, INT, &val2);

	vm_add_inst(&env, (vm_inst){
		.opcode = OP_PLUS,
		.op1 = {CPOOL, id1},
		.op2 = {CPOOL, id2}
	});

	vm_add_inst(&env, (vm_inst){ .opcode = OP_HALT });

	vm_run(&env);

	return 0;
}
