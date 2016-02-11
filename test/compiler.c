#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct instruction {
	const char *name;
	int opcode;
	int has_op1;
	int has_op2;
	int has_result;
};

const struct instruction instrs[] = {
	{"plus",	OP_PLUS, 1, 1, 1},
	{"print", 	OP_PRINT, 1, 0, 0},
	{NULL, 0}
};

const struct instruction *find_inst(const char *name)
{
	int i = 0;
	for (; instrs[i].name != NULL; ++i) {
		if (strcmp(name, instrs[i].name) == 0) {
			return &instrs[i];
		}
	}
	return NULL;
}

vm_operand make_operand(vm_env *env, const char *data)
{
	vm_operand op;
	int temp;

	switch (data[0]) {
	case '$':
		op.type = CONST;
		temp = atoi(data+1);
		op.value.id = vm_add_const(env, INT, &temp);
		break;
	case '#':
		op.type = TEMP;
		op.value.id = atoi(data+1);
		break;
	}
	return op;
}

void compile_line(vm_env *env, char *line)
{
	char *mnemonic = strtok(line, " ");
	char *op1 = strtok(NULL, " ");
	char *op2 = strtok(NULL, " ");
	char *result = strtok(NULL, " ");
	vm_inst new_inst;
	const struct instruction *inst = find_inst(mnemonic);

	if (inst == NULL) return;

	memset(&new_inst, 0, sizeof(vm_inst));

	new_inst.opcode = inst->opcode;

	if (inst->has_op1) {
		new_inst.op1 = make_operand(env, op1);
	}
	if (inst->has_op2) {
		new_inst.op2 = make_operand(env, op2);
	}
	if (inst->has_result) {
		new_inst.result = atoi(result+1);
	}

	vm_add_inst(env, new_inst);

#if DEBUG
	printf("%s - %d ; %d ; %d ; %d\n", inst->name, inst->opcode,
		new_inst.op1.value.id, new_inst.op2.value.id, new_inst.result);
#endif
}

void compile_file(vm_env *env, const char *filename)
{
	char *line = NULL;
	size_t size = 0;
	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Erro: %m\n");
		exit(1);
	}

	while (getline(&line, &size, fp) != -1) {
		if (line[0] == ';') continue;
		compile_line(env, line);
	}
	free(line);

	fclose(fp);
}
