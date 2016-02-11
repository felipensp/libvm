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

#if defined(__GNUC__) && LIBVM_GCC_VERSION >= 30040 && defined(__i386__)
# define LIBVM_FASTCALL __attribute__((fastcall))
#elif defined(_MSC_VER) && defined(_M_IX86)
# define LIBVM_FASTCALL __fastcall
#else
# define LIBVM_FASTCALL
#endif

#define OPCODE    env->insts[pc]
#define HANDLER   OPCODE.handler

#if LIBVM_GCC_VERSION > 0
# define OP(name)    OP_##name
# define OPCODES     const static void* labels[] = { OP_LABELS }; goto *labels[OPCODE.opcode]
# define DISPATCH    ++pc; goto *labels[OPCODE.opcode]
# define END_OPCODES
# define VM_GOTO(n)  pc = n; goto *labels[OPCODE.opcode]
#else
# define OP(name)    case OP_##name
# define OPCODES     for (;;) { switch (OPCODE.opcode) {
# define DISPATCH    ++pc; break
# define END_OPCODES EMPTY_SWITCH_DEFAULT_CASE(); } }
# define VM_GOTO(n)  pc = n; break
#endif

#define VM_CALL_HANDLER() do { if (OPCODE.handler) OPCODE.handler(vm_get_op_value(env, &OPCODE.op1), vm_get_op_value(env, &OPCODE.op2), vm_get_temp_value(env, OPCODE.result)); } while(0)
#define VM_HANDLER_ARGS vm_value *op1, vm_value *op2, vm_value *result

#define CPOOL_MAX_SIZE 100 /* Constant pool max size */
#define INSTS_MAX_SIZE 200 /* Instruction max size */
#define TEMPS_MAX_SIZE 150 /* Temporary storage max size */

/* LIBVM Data structure */

typedef struct {
	enum { CONST, TEMP, ADDR } type;
	union {
		int id;
	} value;
} vm_operand;

typedef struct {
	enum { INT, DBL, STR, PTR } type;
	union {
		int vint;
		double vdouble;
		char *vstr;
		void *vptr;
	} value;
} vm_value;

typedef void (LIBVM_FASTCALL *vm_handler)(VM_HANDLER_ARGS);

typedef struct {
	int opcode;
	vm_operand op1;
	vm_operand op2;
	int result;
	vm_handler handler;
} vm_inst;

typedef struct {
	vm_inst insts[INSTS_MAX_SIZE]; 		/* Program instructions */
	vm_value cpool[CPOOL_MAX_SIZE]; 	/* Constant pool */
	vm_value temps[TEMPS_MAX_SIZE];		/* Temporary storage */
	int insts_count;
	int cpool_count;
	int temps_count;
} vm_env;

/* LIBVM Opcodes */

#define OP_LABELS 	\
	&&OP_PLUS, 		\
	&&OP_PRINT, 	\
	&&OP_JMP, 		\
	&&OP_HALT

enum {
	OP_PLUS,
	OP_PRINT,
	OP_JMP,
	OP_HALT
};

/* LIBVM API */

#define VM_T(_op) _op->type
#define VM_INT(_op) _op->value.vint

void vm_init(vm_env*);
unsigned int vm_add_const(vm_env*, int, void*);
unsigned int vm_add_inst(vm_env*, vm_inst);
unsigned int vm_get_temp(vm_env*);
unsigned int vm_get_last_temp(vm_env*);

#endif /* LIBVM_VM_H */
