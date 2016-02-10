#ifndef LIBVM_VM_H
#define LIBVM_VM_H

#ifdef __GNUC__
# define LIBVM_GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
# define LIBVM_GCC_VERSION 0
#endif

#ifdef _WIN32
# define EMPTY_SWITCH_DEFAULT_CASE() default: __assume(0); break
#elsif defined(__GNUC__) && LIBVM_GCC_VERSION >= 40500
# define EMPTY_SWITCH_DEFAULT_CASE() __builtin_unreachable()
#else
# define EMPTY_SWITCH_DEFAULT_CASE() default: break
#endif

#if LIBVM_GCC_VERSION > 0
# define OP(name)    OP_##name
# define OPCODES     const static void* labels[] = { OP_LABELS }; goto *labels[env->insts[pc].opcode]
# define DISPATCH    ++pc; goto *labels[env->insts[pc].opcode]
# define END_OPCODES
# define VM_GOTO(n)  pc = n; goto *labels[env->insts[pc].opcode]
#else
# define OP(name)    case OP_##name
# define OPCODES     for (;;) { switch (env->insts[pc].opcode) {
# define DISPATCH    ++pc; break
# define END_OPCODES EMPTY_SWITCH_DEFAULT_CASE(); } }
# define VM_GOTO(n)  pc = n; break
#endif

#define OPCODE    env->insts[pc]

#define CPOOL_MAX_SIZE 100 /* Constant pool max size */
#define INSTS_MAX_SIZE 200 /* Instruction max size */
#define TEMPS_MAX_SIZE 150 /* Temporary storage max size */

typedef struct {
	enum { CPOOL, TEMP } type;
	union {
		int id;
	} value;
	int id;
} vm_operand;

typedef struct {
	enum { INT, DOUBLE, STRING } type;
	union {
		int vint;
		char *vstr;
		void *vptr;
	} value;
} vm_value;

typedef struct {
	int opcode;
	vm_operand op1;
	vm_operand op2;
	int result;
} vm_inst;

typedef struct {
	vm_inst insts[INSTS_MAX_SIZE]; 		/* Program instructions */
	vm_value cpool[CPOOL_MAX_SIZE]; 	/* Constant pool */
	vm_value temps[TEMPS_MAX_SIZE];		/* Temporary storage */
	int insts_count;
	int cpool_count;
	int temps_count;
} vm_env;

#define OP_LABELS \
	&&OP_S_SCOPE, \
	&&OP_E_SCOPE, \
	&&OP_PLUS, \
	&&OP_PRINT, \
	&&OP_HALT

enum {
	OP_S_SCOPE,
	OP_E_SCOPE,
	OP_PLUS,
	OP_PRINT,
	OP_HALT
};


void vm_init(vm_env*);
int vm_add_const(vm_env*, int, void*);
int vm_add_inst(vm_env*, vm_inst);
int vm_get_temp(vm_env*);

#endif /* LIBVM_VM_H */
