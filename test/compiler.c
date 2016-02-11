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
	{"print", 	OP_PRINT, 1, 1, 1},
	{"plus",	OP_PLUS, 1, 0, 0},
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

void compile_line(vm_env *env, char *line)
{
	char *mnemonic = strtok(line, " ");
	char *op1 = strtok(line, " ");
	char *op2 = strtok(line, " ");
	char *result = strtok(line, " ");

	const struct instruction *inst = find_inst(mnemonic);
	printf("%s %d\n", inst->name, inst->opcode);
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
