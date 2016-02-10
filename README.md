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
	int id1, id2, val1 = 1, val2 = 2, result;
	vm_env env;

	vm_init(&env);
	id1 = vm_add_const(&env, INT, &val1);
	id2 = vm_add_const(&env, INT, &val2);

	result = vm_get_temp(&env);

	vm_add_inst(&env, (vm_inst){
		.opcode = OP_PLUS,
		.op1 	= {CPOOL, id1},
		.op2 	= {CPOOL, id2},
		.result = result
	});

	vm_add_inst(&env, (vm_inst) { OP_PRINT, {TEMP, result}});
	vm_add_inst(&env, (vm_inst){ OP_HALT });

	vm_run(&env);

	return 0;
}
```
