#include <stdio.h>
#include <string.h>
#include "vm.h"

void vm_init(vm_env *env)
{
	memset(env, 0, sizeof(vm_env));
}

unsigned int vm_add_const(vm_env *env, int type, void *value)
{
	env->cpool[env->cpool_count] = (vm_value) { .type = type };

	switch (type) {
	case INT: env->cpool[env->cpool_count].value.vint = *(int*)value;       break;
	case DBL: env->cpool[env->cpool_count].value.vdouble = *(double*)value; break;
	case STR: env->cpool[env->cpool_count].value.vstr = (char*)value;       break;
	case PTR: env->cpool[env->cpool_count].value.vptr = value;              break;
	}

	return env->cpool_count++;
}

unsigned int vm_add_inst(vm_env *env, vm_inst inst)
{
	env->insts[env->insts_count] = inst;

	return env->insts_count++;
}

unsigned int vm_get_temp(vm_env *env)
{
	return env->temps_count++;
}

unsigned int vm_get_last_temp(vm_env *env)
{
	return env->temps_count - 1;
}

static inline vm_value *vm_get_temp_value(vm_env *env, int id)
{
	return &env->temps[id];
}

static inline vm_value *vm_get_op_value(vm_env *env, const vm_operand *op)
{
	switch (op->type) {
	case CONST:	return &env->cpool[op->value.id];
	case TEMP:	return &env->temps[op->value.id];
	default: 	break;
	}

	return NULL;
}

void vm_run(vm_env *env)
{
	unsigned int pc = 0;
	OPCODES;

	OP(ADD): 	VM_CALL_HANDLER(); DISPATCH;
	OP(SUB): 	VM_CALL_HANDLER(); DISPATCH;
	OP(MULT): 	VM_CALL_HANDLER(); DISPATCH;
	OP(DIV): 	VM_CALL_HANDLER(); DISPATCH;
	OP(PRINT): 	VM_CALL_HANDLER(); DISPATCH;
	OP(JMP): 	VM_GOTO(OPCODE.op1.value.id);

	OP(HALT): goto exit;

	END_OPCODES;
exit:
	return;
}
