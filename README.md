# libvm
Register-based VM as C library


####Current instruction set:####

| Opcode  | Description |
| ------------- | ------------- |
| OP_S_SCOPE | Starts a new scope context |
| OP_E_SCOPE | Ends current scope context |
| OP_HALT | Ends VM execution |

####Example####

```c
#include "vm.h"

int main(int argc, char **argv)
{
	int id1, id2;
	int val1 = 1, val2 = 2;
	vm_env env;
	vm_inst insts[5];

	vm_init(&env);
	id1 = vm_add_constant(&env, INT, &val1);
	id2 = vm_add_constant(&env, INT, &val2);

	insts[0] = (vm_inst){
		.opcode = OP_PLUS,
		.op1 = {CPOOL, id1},
		.op2 = {CPOOL, id2}
	};

	insts[1] = (vm_inst){ .opcode = OP_HALT };

	vm_run(&env, insts);

	return 0;
}
```
