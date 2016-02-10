#include <stdio.h>
#include "vm.h"

void vm_init(vm_env *env)
{
	env->cpool_count = 0;
	env->insts_count = 0;
}

int vm_add_constant(vm_env *env, int type, void *value)
{
	switch (type) {
	case INT:
		env->cpool[env->cpool_count] = (vm_cpool_entry) { .type = type, .value.vint = *(int*)value };
		break;
	case STRING:
		env->cpool[env->cpool_count] = (vm_cpool_entry) { .type = type, .value.vstr = (char*)value };
		break;
	}

	return env->cpool_count++;
}

int vm_add_inst(vm_env *env, vm_inst inst)
{
	env->insts[env->insts_count] = inst;

	return env->insts_count++;
}

int vm_get_op_value(const vm_env *env, const vm_operand *op)
{
	switch (op->type) {
		case CPOOL:
			return env->cpool[op->value.id].value.vint;
			break;
	}
	return -1;
}


void vm_run(vm_env *env)
{
	int pc = 0;
	OPCODES;

	OP(S_SCOPE):
	DISPATCH;

	OP(E_SCOPE):
	DISPATCH;

	OP(PLUS):
		printf("%d\n", vm_get_op_value(env, &OPCODE.op1) + vm_get_op_value(env, &OPCODE.op2));
	DISPATCH;

	OP(HALT): goto exit;

	END_OPCODES;
exit:
	return;
}
