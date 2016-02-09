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

typedef struct {
	int opcode;
} vm_inst;


#define OP_LABELS \
	&&OP_S_SCOPE, \
	&&OP_E_SCOPE, \
	&&OP_HALT

enum {
	OP_S_SCOPE,
	OP_E_SCOPE,
	OP_HALT
};

#endif /* LIBVM_VM_H */
