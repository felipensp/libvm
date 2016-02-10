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
# define OPCODES     const static void* labels[] = { OP_LABELS }; goto *labels[inst[pc].opcode]
# define DISPATCH    ++pc; goto *labels[inst[pc].opcode]
# define END_OPCODES
# define VM_GOTO(n)  pc = n; goto *labels[inst[pc].opcode]
#else
# define OP(name)    case OP_##name
# define OPCODES     for (;;) { switch (inst[pc].opcode) {
# define DISPATCH    ++pc; break
# define END_OPCODES EMPTY_SWITCH_DEFAULT_CASE(); } }
# define VM_GOTO(n)  pc = n; break
#endif

#define OPCODE    inst[pc]

#define CPOOL_MAX_SIZE 100 /* Constant pool max size */

typedef struct {
	enum { CPOOL } type;
	union {
		int id; /* For constant pool index */
	} value;
} vm_operand;

typedef struct {
	int opcode;
	vm_operand op1;
	vm_operand op2;
} vm_inst;

typedef struct {
	enum { INT, DOUBLE, STRING } type;
	union {
		int vint;
		char *vstr;
		void *vptr;
	} value;
} vm_cpool_entry;

typedef struct {
	vm_inst *insts; 						/* Program instructions */
	vm_cpool_entry cpool[CPOOL_MAX_SIZE]; 	/* Constant pool */
	int cpool_count;
} vm_env;

#define OP_LABELS \
	&&OP_S_SCOPE, \
	&&OP_E_SCOPE, \
	&&OP_PLUS, \
	&&OP_HALT

enum {
	OP_S_SCOPE,
	OP_E_SCOPE,
	OP_PLUS,
	OP_HALT
};


void vm_init(vm_env*);
int vm_add_constant(vm_env*, int, void*);

#endif /* LIBVM_VM_H */
