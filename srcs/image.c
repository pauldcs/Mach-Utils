#include "common.h"
#include "ios-macos-utils.h"
#include <mach-o/loader.h>
#include <mach/mach.h>
#include <mach/mach_traps.h>
#include <mach-o/fat.h>
#include <mach-o/loader.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>

const char *__attribute__((const)) magic_to_cstr(uint32_t magic)
{
	switch (magic) {
	case FAT_MAGIC:
		return "Universal binary 32-bit LE";
	case FAT_CIGAM:
		return "Universal binary 32-bit BE";
	case FAT_MAGIC_64:
		return "Universal binary 64-bit LE";
	case FAT_CIGAM_64:
		return "Universal binary 64-bit BE";
	case MH_MAGIC:
		return "Mach-O executable 32-bit LE";
	case MH_CIGAM:
		return "Mach-O executable 32-bit BE";
	case MH_MAGIC_64:
		return "Mach-O 64-bit executable LE";
	case MH_CIGAM_64:
		return "Mach-O 64-bit executable BE";
	default:
		return "???";
	}
}

const char *__attribute__((const)) load_command_to_cstr(uint32_t command)
{
	switch (command) {
	case LC_SEGMENT:
		return "segment";
	case LC_SYMTAB:
		return "symbol_table";
	case LC_THREAD:
		return "thread";
	case LC_UNIXTHREAD:
		return "unix_thread";
	case LC_LOADFVMLIB:
		return "fixed_VM_shared_library_load";
	case LC_IDFVMLIB:
		return "fixed_VM_shared_library_identification";
	case LC_FVMFILE:
		return "fixed_vm_file_inclusion";
	case LC_PREPAGE:
		return "Prepage";
	case LC_DYSYMTAB:
		return "dynamic_symbol_table";
	case LC_LOAD_DYLIB:
		return "dynamically_linked_shared_library_load";
	case LC_ID_DYLIB:
		return "dynamically_linked_shared_library_identification";
	case LC_LOAD_DYLINKER:
		return "dynamic_linker_load";
	case LC_ID_DYLINKER:
		return "dynamic_linker_identification";
	case LC_PREBOUND_DYLIB:
		return "prebound_modules_for_dynamically_linked_shared_library";
	case LC_ROUTINES:
		return "image_routines";
	case LC_SUB_FRAMEWORK:
		return "sub_framework";
	case LC_SUB_UMBRELLA:
		return "sub_umbrella";
	case LC_SUB_CLIENT:
		return "sub_client";
	case LC_SUB_LIBRARY:
		return "sub_library";
	case LC_TWOLEVEL_HINTS:
		return "two-level_namespace_lookup_hints";
	case LC_PREBIND_CKSUM:
		return "prebind_checksum";
	case LC_LOAD_WEAK_DYLIB:
		return "weakly_linked_dynamically_linked_shared_library_load";
	case LC_SEGMENT_64:
		return "64_bit_segment";
	case LC_ROUTINES_64:
		return "64_bit_image_routines";
	case LC_UUID:
		return "uuid";
	case LC_RPATH:
		return "runpath_additions";
	case LC_CODE_SIGNATURE:
		return "code_signature";
	case LC_SEGMENT_SPLIT_INFO:
		return "split_segments";
	case LC_REEXPORT_DYLIB:
		return "re_export_dynamically_linked_shared_library";
	case LC_LAZY_LOAD_DYLIB:
		return "delay_load_of_dynamically_linked_shared_library";
	case LC_ENCRYPTION_INFO:
		return "encryption_information";
	case LC_DYLD_INFO:
		return "dynamic_linker_information";
	case LC_DYLD_INFO_ONLY:
		return "dynamic_linker_information_(only)";
	case LC_LOAD_UPWARD_DYLIB:
		return "load_upward_dylib";
	case LC_VERSION_MIN_MACOSX:
		return "minimum_macOS_version";
	case LC_VERSION_MIN_IPHONEOS:
		return "minimum_iPhoneOS_version";
	case LC_FUNCTION_STARTS:
		return "compressed_table_of_function_start_addresses";
	case LC_DYLD_ENVIRONMENT:
		return "dynamic_linker_environment";
	case LC_MAIN:
		return "replacement_for_UNIX_thread";
	case LC_DATA_IN_CODE:
		return "table_of_non-instructions_in___text";
	case LC_SOURCE_VERSION:
		return "source_version";
	case LC_DYLIB_CODE_SIGN_DRS:
		return "code_signing_DRs_copied_from_linked_dylibs";
	case LC_ENCRYPTION_INFO_64:
		return "64_bit_encryption_information";
	case LC_LINKER_OPTION:
		return "linker_options";
	case LC_LINKER_OPTIMIZATION_HINT:
		return "optimization_hints";
	case LC_VERSION_MIN_TVOS:
		return "minimum_tvOS_version";
	case LC_VERSION_MIN_WATCHOS:
		return "minimum_watchOS_version";
	case LC_NOTE:
		return "arbitrary_data_in_Mach-O_file";
	case LC_BUILD_VERSION:
		return "build_version_information";
	case LC_DYLD_EXPORTS_TRIE:
		return "shared_trie_of_exported_symbols";
	case LC_DYLD_CHAINED_FIXUPS:
		return "chained_fixups";
	case LC_FILESET_ENTRY:
		return "fileset_entry";
	default:
		return "???";
	}
}

const char *__attribute__((const)) attr_type_to_cstr(uint32_t attr_type)
{
	switch (attr_type) {
	case 0:
		return "none";
	case S_ATTR_PURE_INSTRUCTIONS:
		return "pure_instructions";
	case S_ATTR_NO_TOC:
		return "no_toc";
	case S_ATTR_STRIP_STATIC_SYMS:
		return "strip_static_syms";
	case S_ATTR_NO_DEAD_STRIP:
		return "no_dead_strip";
	case S_ATTR_LIVE_SUPPORT:
		return "live_support";
	case S_ATTR_SELF_MODIFYING_CODE:
		return "self_modifying_code";
	case S_ATTR_DEBUG:
		return "debug";
	default:
		return "???";
	}
}

const char *__attribute__((const)) cputype_to_cstr(uint32_t cputype)
{
	switch (cputype) {
	case CPU_TYPE_VAX:
		return "VAX";
	case CPU_TYPE_MC680x0:
		return "Motorola 680x0";
	case CPU_TYPE_I386:
		return "Intel 386";
	case CPU_TYPE_X86_64:
		return "x86_64";
	case CPU_TYPE_MC88000:
		return "Motorola 88000";
	case CPU_TYPE_MC98000:
		return "Motorola 98000";
	case CPU_TYPE_HPPA:
		return "HP PA-RISC";
	case CPU_TYPE_ARM:
		return "ARM";
	case CPU_TYPE_ARM64:
		return "ARM64";
	case CPU_TYPE_ARM64_32:
		return "ARM64_32";
	case CPU_TYPE_SPARC:
		return "SPARC";
	case CPU_TYPE_I860:
		return "Intel 860";
	case CPU_TYPE_POWERPC:
		return "PowerPC";
	case CPU_TYPE_POWERPC64:
		return "PowerPC 64-bit";
	default:
		return "???";
	}
}

const char *__attribute__((const)) cpusubtype_to_cstr(uint32_t cputype,
						      uint32_t cpusubtype)
{
	switch (cputype) {
	case CPU_TYPE_VAX:
		switch (cpusubtype) {
		case CPU_SUBTYPE_VAX_ALL:
			return "all";
		case CPU_SUBTYPE_VAX780:
			return "vax780";
		case CPU_SUBTYPE_VAX785:
			return "vax785";
		case CPU_SUBTYPE_VAX750:
			return "vax750";
		case CPU_SUBTYPE_VAX730:
			return "vax730";
		case CPU_SUBTYPE_UVAXI:
			return "uvaxI";
		case CPU_SUBTYPE_UVAXII:
			return "uvaxII";
		case CPU_SUBTYPE_VAX8200:
			return "vax8200";
		case CPU_SUBTYPE_VAX8500:
			return "vax8500";
		case CPU_SUBTYPE_VAX8600:
			return "vax8600";
		case CPU_SUBTYPE_VAX8650:
			return "vax8650";
		case CPU_SUBTYPE_VAX8800:
			return "vax8800";
		case CPU_SUBTYPE_UVAXIII:
			return "uvaxIII";
		default:
			return "???";
		}
	case CPU_TYPE_MC680x0:
		switch (cpusubtype) {
		case CPU_SUBTYPE_MC68030:
			return "mc68030";
		case CPU_SUBTYPE_MC68040:
			return "mc68040";
		case CPU_SUBTYPE_MC68030_ONLY:
			return "mc68030 only";
		default:
			return "???";
		}
	case CPU_TYPE_I386:
		switch (cpusubtype) {
		case CPU_SUBTYPE_386:
			return "386";
		case CPU_SUBTYPE_486:
			return "486";
		case CPU_SUBTYPE_486SX:
			return "486sx";
		case CPU_SUBTYPE_PENT:
			return "pentium";
		case CPU_SUBTYPE_PENTPRO:
			return "Pentium Pro";
		case CPU_SUBTYPE_PENTII_M3:
			return "Pentium 3 M3";
		case CPU_SUBTYPE_PENTII_M5:
			return "Pentium 3 M5";
		case CPU_SUBTYPE_CELERON:
			return "Celeron";
		case CPU_SUBTYPE_CELERON_MOBILE:
			return "Celeron Mobile";
		case CPU_SUBTYPE_PENTIUM_3:
			return "Pentium 3";
		case CPU_SUBTYPE_PENTIUM_3_M:
			return "Pentium 3 M";
		case CPU_SUBTYPE_PENTIUM_3_XEON:
			return "Pentium 3 Xeon";
		case CPU_SUBTYPE_PENTIUM_M:
			return "Pentium Mobile";
		case CPU_SUBTYPE_PENTIUM_4:
			return "Pentium 4";
		case CPU_SUBTYPE_PENTIUM_4_M:
			return "Pentium 4 M";
		case CPU_SUBTYPE_ITANIUM:
			return "Itanium";
		case CPU_SUBTYPE_ITANIUM_2:
			return "Itanium 2";
		case CPU_SUBTYPE_XEON:
			return "Xeon";
		case CPU_SUBTYPE_XEON_MP:
			return "Xeon MP";
		default:
			return "???";
		}
	case CPU_TYPE_X86_64:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_X86_64_ALL:
			return "all";
		case CPU_SUBTYPE_X86_ARCH1:
			return "arch 1";
		default:
			return "???";
		}
	case CPU_TYPE_MC88000:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_MC88000_ALL:
			return "all";
		case CPU_SUBTYPE_MC88100:
			return "mc88100";
		case CPU_SUBTYPE_MC88110:
			return "mc88110";
		default:
			return "???";
		}
	case CPU_TYPE_MC98000:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_MC98000_ALL:
			return "all";
		case CPU_SUBTYPE_MC98601:
			return "mc98601";
		default:
			return "???";
		}
	case CPU_TYPE_HPPA:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_HPPA_7100:
			return "hppa7100";
		case CPU_SUBTYPE_HPPA_7100LC:
			return "hppa7100LC";
		default:
			return "???";
		}
	case CPU_TYPE_ARM64:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_ARM64_ALL:
			return "all";
		case CPU_SUBTYPE_ARM64_V8:
			return "arm64v8";
		case CPU_SUBTYPE_ARM64E:
			return "arm64e";
		default:
			return "Unknown arm64 subtype";
		}
	case CPU_TYPE_ARM:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_ARM_ALL:
			return "all";
		case CPU_SUBTYPE_ARM_V4T:
			return "v4t";
		case CPU_SUBTYPE_ARM_V6:
			return "v6";
		case CPU_SUBTYPE_ARM_XSCALE:
			return "xscale";
		case CPU_SUBTYPE_ARM_V7:
			return "v7";
		case CPU_SUBTYPE_ARM_V7F:
			return "v7f";
		case CPU_SUBTYPE_ARM_V7S:
			return "v7s";
		case CPU_SUBTYPE_ARM_V7K:
			return "v7k";
		case CPU_SUBTYPE_ARM_V7M:
			return "v7m";
		case CPU_SUBTYPE_ARM_V7EM:
			return "v7em";
		default:
			return "???";
		}
	case CPU_TYPE_SPARC:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_SPARC_ALL:
			return "all";
		default:
			return "???";
		}
	case 8:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_MIPS_ALL:
			return "all";
		case CPU_SUBTYPE_MIPS_R2300:
			return "r2300";
		case CPU_SUBTYPE_MIPS_R2600:
			return "r2600";
		case CPU_SUBTYPE_MIPS_R2800:
			return "r2800";
		case CPU_SUBTYPE_MIPS_R2000a:
			return "r2000a";
		case CPU_SUBTYPE_MIPS_R2000:
			return "r2000";
		case CPU_SUBTYPE_MIPS_R3000a:
			return "r3000a";
		case CPU_SUBTYPE_MIPS_R3000:
			return "r3000";
		default:
			return "???";
		}
	case CPU_TYPE_I860:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_I860_ALL:
			return "all";
		case CPU_SUBTYPE_I860_860:
			return "860";
		default:
			return "???";
		}
	case CPU_TYPE_POWERPC:
	case CPU_TYPE_POWERPC64:
		switch (cpusubtype & 0xff) {
		case CPU_SUBTYPE_POWERPC_ALL:
			return "all";
		case CPU_SUBTYPE_POWERPC_601:
			return "601";
		case CPU_SUBTYPE_POWERPC_602:
			return "602";
		case CPU_SUBTYPE_POWERPC_603:
			return "603";
		case CPU_SUBTYPE_POWERPC_603e:
			return "603e";
		case CPU_SUBTYPE_POWERPC_603ev:
			return "603ev";
		case CPU_SUBTYPE_POWERPC_604:
			return "604";
		case CPU_SUBTYPE_POWERPC_604e:
			return "604e";
		case CPU_SUBTYPE_POWERPC_620:
			return "620";
		case CPU_SUBTYPE_POWERPC_750:
			return "750";
		case CPU_SUBTYPE_POWERPC_7400:
			return "7400";
		case CPU_SUBTYPE_POWERPC_7450:
			return "7450";
		case CPU_SUBTYPE_POWERPC_970:
			return "970";
		default:
			return "???";
		}
	}
	return "???";
}

const char *__attribute__((const)) filetype_to_cstr(uint32_t filetype)
{
	switch (filetype) {
	case MH_OBJECT:
		return "relocatable_object";
	case MH_EXECUTE:
		return "executable_file";
	case MH_FVMLIB:
		return "fixed_vm_shared_library";
	case MH_CORE:
		return "core_file";
	case MH_PRELOAD:
		return "preloaded_executable_file";
	case MH_DYLIB:
		return "dynamically_bound_shared_library";
	case MH_DYLINKER:
		return "dynamic_link_editor";
	case MH_BUNDLE:
		return "dynamically_bound_bundle_file";
	case MH_DYLIB_STUB:
		return "shared_library_stub_for_static_linking";
	case MH_DSYM:
		return "companion_file_with_only_debug_sections";
	case MH_KEXT_BUNDLE:
		return "kernel_extension_bundle_file";
	case MH_FILESET:
		return "kernel_cache_file";
	default:
		return "???";
	}
}

const char *__attribute__((const)) sectype_to_cstr(uint32_t filetype)
{
	switch (filetype) {
	case S_REGULAR:
		return "regular";
	case S_CSTRING_LITERALS:
		return "cstring_literals";
	case S_4BYTE_LITERALS:
		return "4byte_literals";
	case S_8BYTE_LITERALS:
		return "8byte_literals";
	case S_16BYTE_LITERALS:
		return "16byte_literals";
	case S_LITERAL_POINTERS:
		return "literal_pointers";
	case S_NON_LAZY_SYMBOL_POINTERS:
		return "non_lazy_symbol_pointers";
	case S_LAZY_SYMBOL_POINTERS:
		return "lazy_symbol_pointers";
	case S_SYMBOL_STUBS:
		return "symbol_stubs";
	case S_MOD_INIT_FUNC_POINTERS:
		return "mod_init_funcs";
	case S_MOD_TERM_FUNC_POINTERS:
		return "mod_term_funcs";
	case S_COALESCED:
		return "coalesced";
	case S_INTERPOSING:
		return "interposing";
	case S_THREAD_LOCAL_REGULAR:
		return "thread_local_regular";
	case S_THREAD_LOCAL_VARIABLES:
		return "thread_local_variables";
	case S_THREAD_LOCAL_INIT_FUNCTION_POINTERS:
		return "thread_local_init_function_pointers";
	default:
		return "???";
	}
}

bool get_image_address_by_cputype(task_t task, vm_address_t *baddr,
				  int32_t cputype)
{
	kern_return_t			kr;
	vm_address_t			addr = 0;
	vm_size_t			size;
	vm_region_submap_info_data_64_t info_var;
	mach_msg_type_number_t info_count = VM_REGION_SUBMAP_INFO_COUNT_64;
	unsigned int	       depth	  = 0;
	size_t		       read_size  = sizeof(unsigned int);

	const uint8_t hdr[sizeof(struct mach_header_64)];
	while (true) {
		kr = vm_region_recurse_64(task, &addr, &size, &depth,
					  (vm_region_info_t)&info_var,
					  &info_count);

		if (kr != KERN_SUCCESS)
			break;

		unsigned int header;
		vm_size_t    bytes_read;

		kr = vm_read_overwrite(task, addr, read_size,
				       (vm_address_t)&header, &bytes_read);

		if (kr != KERN_SUCCESS) {
			__logger(error,
				 "get_image_address: vm_read_overwrite: %s",
				 mach_error_string(kr));

			return (false);
		}

		if (bytes_read != read_size) {
			__logger(
				error,
				"get_image_address: vm_read_overwrite: Read %d bytes instead of %d",
				bytes_read, read_size);

			return (false);
		}

		if (header == MH_MAGIC_64) {
			if (!memory_r(task, addr, hdr,
				      sizeof(struct mach_header_64)))
				return (false);

			struct mach_header_64 *head =
				(struct mach_header_64 *)hdr;
			__logger(info, "Mach-O header at %p: %s %s %s (%s)",
				 addr, magic_to_cstr(head->magic),
				 cputype_to_cstr(head->cputype),
				 cpusubtype_to_cstr(head->cputype,
						    head->cpusubtype),
				 filetype_to_cstr(head->filetype));
			*baddr = addr;
			if (head->cputype == cputype)
				break;
		}

		addr += size;
	}

	if (!addr) {
		__logger(error, "Could not find a suitable the Mach-O image");
		return (false);
	}

	return (true);
}
