#include "common.h"
#include "ios-macos-utils.h"
#include <mach/mach_error.h>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/vm_map.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

bool memory_w(task_t task, vm_address_t addr, const uint8_t *buf,
	      vm_size_t bufsize)
{
	kern_return_t kr;
	vm_prot_t     initial_prot;

	if (!memory_prot_get(task, &initial_prot, addr, bufsize)) {
		return (false);
	}

	if (!memory_prot_set(task, addr, bufsize,
			     VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY)) {
		return (false);
	}

	kr = mach_vm_write(task, addr, (vm_offset_t)buf, bufsize);
	if (kr != KERN_SUCCESS) {
		__logger(error, "mach_vm_write: %s", mach_error_string(kr));
		return (false);
	}

	if (!memory_prot_set(task, addr, bufsize, initial_prot)) {
		return (false);
	}

	if (!memory_flush_caches(task, addr, bufsize)) {
		return (false);
	}

	return (true);
}

bool memory_r(task_t task, vm_address_t addr, const uint8_t *buf,
	      vm_size_t bufsize)
{
	mach_vm_size_t ret;
	kern_return_t  kr = mach_vm_read_overwrite(task, addr, bufsize,
						   (vm_address_t)buf, &ret);

	if (kr != KERN_SUCCESS) {
		__logger(error, "mach_vm_read_overwrite: %s",
			 mach_error_string(kr));
		return (false);
	}

	if (ret != bufsize) {
		__logger(error,
			 "mach_vm_read_overwrite: read %d bytes instead of %d",
			 ret, bufsize);
		return (false);
	}

	return (true);
}

bool memory_rchunk(task_t task, vm_address_t addr, const uint8_t *buf,
		   vm_size_t bufsize)
{
	vm_address_t end     = addr + bufsize;
	vm_size_t    ret     = 0;
	vm_size_t    to_read = bufsize;

	while (addr != end) {
		bufsize = (to_read > MAX_READ_SIZE) ? MAX_READ_SIZE : to_read;

		if (!memory_r(task, addr, buf + ret, bufsize)) {
			return (false);
		}

		ret += bufsize;
		addr += bufsize;
		to_read -= bufsize;
	}

	return (true);
}

/* Reads a pointer at 'addr', dereferences it and reads 'bufsize'
 * from there.
 */
bool memory_rderef_ptr_at(task_t task, vm_address_t addr, const uint8_t *buffer,
			  vm_size_t bufsize)
{
	const uint8_t ptrbuf[sizeof(void *)];

	if (!memory_r(task, addr, ptrbuf, sizeof(void *)))
		return (false);

	if (!memory_rchunk(task, *(int64_t *)ptrbuf, buffer, bufsize))
		return (false);

	return (true);
}

bool memory_flush_caches(task_t task, mach_vm_address_t address,
			 mach_vm_size_t size)
{
	vm_machine_attribute_val_t mattr_value = MATTR_VAL_CACHE_FLUSH;
	kern_return_t		   kr;

	kr = mach_vm_machine_attribute(task, address, size, MATTR_CACHE,
				       &mattr_value);

	if (kr != KERN_SUCCESS) {
		__logger(error, "vm_machine_attribute: %s",
			 mach_error_string(kr));
		return (false);
	}

	return (true);
}

bool memory_prot_get(task_t task, vm_prot_t *prot, vm_address_t address,
		     vm_size_t size)
{
	vm_region_basic_info_data_64_t info;
	mach_msg_type_number_t info_count  = VM_REGION_BASIC_INFO_COUNT_64;
	mach_vm_address_t      region	   = address;
	mach_vm_size_t	       region_size = size;
	mach_port_t	       object_name;
	kern_return_t	       kr;

	kr = mach_vm_region(task, &region, &region_size,
			    VM_REGION_BASIC_INFO_64, (vm_region_info_t)&info,
			    (mach_msg_type_number_t *)&info_count,
			    (mach_port_t *)&object_name);

	if (kr != KERN_SUCCESS) {
		__logger(error, "get_region_prot: %s", mach_error_string(kr));
		return (false);
	}

	*prot = info.max_protection;
	return (true);
}

bool memory_prot_set(vm_map_t task, mach_vm_address_t address,
		     mach_vm_size_t size, vm_prot_t prot)
{
	kern_return_t kr;

	kr = mach_vm_protect(task, address, size, false, prot);
	if (kr != KERN_SUCCESS) {
		__logger(error, "mach_vm_protect: %s", mach_error_string(kr));
		return (false);
	}

	return (true);
}

bool memory_region_info_get(task_t task, vm_address_t address,
			    mach_vm_address_t *region,
			    mach_vm_size_t    *region_size)
{
	vm_region_basic_info_data_64_t info;
	mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT_64;
	mach_port_t	       object_name;
	kern_return_t	       kr;

	*region_size = 0;
	*region	     = address;

	kr = mach_vm_region(task, region, region_size, VM_REGION_BASIC_INFO_64,
			    (vm_region_info_t)&info,
			    (mach_msg_type_number_t *)&info_count,
			    (mach_port_t *)&object_name);

	if (kr != KERN_SUCCESS) {
		__logger(error, "mach_vm_region: %s", mach_error_string(kr));
		return (false);
	}

	return (true);
}

bool memory_dump(task_t task, vm_address_t address, size_t size)
{
	uint8_t *buffer = NULL;

	buffer = malloc(size);
	if (!buffer) {
		__logger(error, "malloc: out of memory");
		return (false);
	}

	if (!memory_r(task, address, buffer, size)) {
		free(buffer);
		return (false);
	}

	(void)hexdump((const uint8_t *)buffer, size, address);

	free(buffer);
	return (true);
}
