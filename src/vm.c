#include <stdio.h>
#include <string.h>
#include "vm.h"

void vm_init(vm_env *env)
{
	memset(env, 0, sizeof(vm_env));
}

int vm_add_const(vm_env *env, int type, void *value)
{
	switch (type) {
	case INT:
		env->cpool[env->cpool_count] = (vm_value) {
			.type = type,
			.value.vint = *(int*)value
		};
		break;
	case STRING:
		env->cpool[env->cpool_count] = (vm_value) {
			.type = type,
			.value.vstr = (char*)value
		};
		break;
	}

	return env->cpool_count++;
}

int vm_add_inst(vm_env *env, vm_inst inst)
{
	env->insts[env->insts_count] = inst;

	return env->insts_count++;
}

int vm_get_temp(vm_env *env)
{
	return env->temps_count++;
}

vm_value *vm_get_temp_value(vm_env *env, int id)
{
	return &env->temps[id];
}

int vm_get_op_value(const vm_env *env, const vm_operand *op)
{
	switch (op->type) {
	case CPOOL:
		return env->cpool[op->value.id].value.vint;
	case TEMP:
		return env->temps[op->value.id].value.vint;
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
		vm_get_temp_value(env, OPCODE.result)->value.vint =
			vm_get_op_value(env, &OPCODE.op1) + vm_get_op_value(env, &OPCODE.op2);
	DISPATCH;

	OP(PRINT):
		printf("%d\n", vm_get_op_value(env, &OPCODE.op1));
	DISPATCH;

	OP(HALT): goto exit;

	END_OPCODES;
exit:
	return;
}
