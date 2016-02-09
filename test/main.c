#include "vm.h"

int main(int argc, char **argv)
{
	vm_inst insts[] = {{ OP_S_SCOPE }, { OP_E_SCOPE }, { OP_HALT }};

	vm_run(insts);

	return 0;
}
