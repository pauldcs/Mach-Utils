#ifndef __IOS_MACOS_UTILS_H__
#define __IOS_MACOS_UTILS_H__

#include <mach/mach.h>
#include <mach/mach_traps.h>
#include <stdbool.h>
#include <sys/types.h>

#define MAX_READ_SIZE 0x500

#define page_align(addr) \
	(vm_address_t)((uintptr_t)(addr) & (~(vm_page_size - 1)))
#define swap_32(v) v = __builtin_bswap32(v)

typedef struct patch_s {
	const uint64_t offset;
	const uint8_t *code;
	size_t	       code_size;
} patch_t;

const char *__attribute__((const)) cpusubtype_to_cstr(uint32_t cputype,
						      uint32_t cpusubtype);
const char *__attribute__((const)) cputype_to_cstr(uint32_t cputype);
const char *__attribute__((const)) filetype_to_cstr(uint32_t filetype);
const char *__attribute__((const)) magic_to_cstr(uint32_t magic);
const char *__attribute__((const)) load_command_to_cstr(uint32_t command);
const char *__attribute__((const)) sectype_to_cstr(uint32_t filetype);
const char *__attribute__((const)) attr_type_to_cstr(uint32_t attr_type);

/* IMAGE
*/
bool spawn_program(pid_t *pid, const char *binpath);
bool get_image_address_by_cputype(task_t task, vm_address_t *baddr,
				  int32_t cputype);

/* MEMORY
 */
bool memory_w(task_t task, vm_address_t addr, const uint8_t *buf,
	      vm_size_t bufsize);
bool memory_r(task_t task, vm_address_t addr, const uint8_t *buf,
	      vm_size_t bufsize);
bool memory_rchunk(task_t task, vm_address_t addr, const uint8_t *buf,
		   vm_size_t bufsize);
bool memory_rderef_ptr_at(task_t task, vm_address_t addr, const uint8_t *buffer,
			  vm_size_t bufsize);
bool memory_flush_caches(task_t task, mach_vm_address_t address,
			 mach_vm_size_t size);
bool memory_prot_get(task_t task, vm_prot_t *prot, vm_address_t address,
		     vm_size_t size);
bool memory_prot_set(vm_map_t task, mach_vm_address_t address,
		     mach_vm_size_t size, vm_prot_t prot);
bool memory_region_info_get(task_t task, vm_address_t address,
			    mach_vm_address_t *region,
			    mach_vm_size_t    *region_size);
bool memory_dump(task_t task, vm_address_t address, size_t size);

/* TASK
 */
mach_port_t connect_to_service(const char *service_name);

bool process_find_by_name(const char *name, pid_t *pid);
bool process_get_task(pid_t pid, task_t *task);
bool process_suspend(task_t task);
bool process_resume(task_t task);

#endif /* __IOS_MACOS_UTILS_H__ */
