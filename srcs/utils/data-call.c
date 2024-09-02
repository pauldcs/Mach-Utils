#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <stdbool.h>

// # define FUNC_NAME "test"
// # define ARG_1     "42"
// # define ARG_2     "24"

// void __call(void) {
//     asm volatile (
//         "stp    x29, x30, [sp, #-16]! \n"
//         "mov    x29, sp               \n"
//
//         "mov    x0, "ARG_1"           \n"
//         "mov    x1, "ARG_2"           \n"
//         "bl     _"FUNC_NAME"           \n"
//
//         "ldp    x29, x30, [sp], #16   \n"
//     );
// }

// int fonction(void) {
//     return (42);
// }

// mov w0, #0x2a
// ret

const char *code = "\x40\x05\x80\x52"
		   "\xc0\x03\x5f\xd6";
int	    main(void)
{
	//if (mach_vm_protect(mach_task_self(), (vm_address_t)code, sizeof(code), false,
	//                    VM_PROT_READ) != KERN_SUCCESS) {
	//  return (-1);
	//}

	return ((int (*)(void))code)();
}
