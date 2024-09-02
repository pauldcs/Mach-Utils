#include "common.h"
#include <bootstrap.h>
#include <mach/mach.h>

mach_port_t connect_to_service(const char *service_name)
{
	mach_port_t   bs_port;
	mach_port_t   service_port;
	kern_return_t kr;

	task_get_bootstrap_port(mach_task_self(), &bs_port);
	kr = bootstrap_look_up(bs_port, service_name, &service_port);

	if (kr != KERN_SUCCESS) {
		__logger(error, "connect_to_service: %s",
			 mach_error_string(kr));
		return (MACH_PORT_NULL);
	}

	return (service_port);
}
