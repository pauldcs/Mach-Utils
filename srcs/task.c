#include "common.h"
#include <mach/mach_error.h>
#include "common.h"
#include <libproc.h>
#include <stdio.h>
#include <string.h>
#include <mach/mach.h>
#include <mach/task.h>
#include <stdbool.h>
#include <sys/types.h>

bool process_find_by_name(const char *name, pid_t *pid)
{
	pid_t pids[2048];
	int   bytes = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));

	int n_proc = bytes / sizeof(pids[0]);

	for (int i = 0; i < n_proc; i++) {
		struct proc_bsdinfo proc;
		int st = proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &proc,
				      PROC_PIDTBSDINFO_SIZE);

		if (st == PROC_PIDTBSDINFO_SIZE) {
			if (strcmp(name, proc.pbi_name) == 0) {
				*pid = pids[i];
				return (true);
			}
		}
	}

	__logger(error, "Could not find the process '%s'", name);
	return (false);
}

bool process_get_task(pid_t pid, task_t *task)
{
	kern_return_t kr;

	kr = task_for_pid(mach_task_self(), pid, task);

	if (kr != KERN_SUCCESS) {
		__logger(error, "task_for_pid: %s", mach_error_string(kr));
		return (false);
	}

	return (true);
}

bool process_suspend(task_t task)
{
	kern_return_t kr;

	kr = task_suspend(task);
	if (kr != KERN_SUCCESS) {
		__logger(error, "task_suspend: %s",
			 mach_error_string(kr)) return (false);
	}
	return (true);
}

bool process_resume(task_t task)
{
	kern_return_t kr;

	kr = task_resume(task);
	if (kr != KERN_SUCCESS) {
		__logger(error, "task_resume: %s",
			 mach_error_string(kr)) return (false);
	}
	return (true);
}
