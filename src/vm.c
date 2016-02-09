#include <stdio.h>
#include "vm.h"

void vm_run(vm_inst *inst)
{
	int pc = 0;
	OPCODES;

	OP(S_SCOPE):
		printf("start scope\n");
	DISPATCH;

	OP(E_SCOPE):
		printf("end scope\n");
	DISPATCH;

	OP(HALT): goto exit;

	END_OPCODES;
exit:
	return;
}
