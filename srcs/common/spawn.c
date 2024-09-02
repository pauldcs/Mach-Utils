#include "common.h"
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <spawn.h>
#include <stdlib.h>

bool spawn_program(pid_t *pid, const char *binpath)
{
	posix_spawnattr_t attr;

	static struct {
		char *const *argv[4];
		const char  *envp[1];
	} arguments;

	arguments.argv[0] = (char *const *)binpath;
	arguments.argv[1] = NULL;
	arguments.envp[0] = NULL;

	if (posix_spawnattr_init(&attr) != 0) {
		__logger(error, "posix_spawnattr_init %s", strerror(errno));
		return (false);
	}

	if (posix_spawnattr_setflags(&attr, POSIX_SPAWN_START_SUSPENDED) != 0) {
		__logger(error, "posix_spawnattr_setflags %s", strerror(errno));
		return (false);
	}

	if (posix_spawn(pid, (const char *)arguments.argv[0], NULL, &attr,
			(char *const *)arguments.argv,
			(char *const *)arguments.envp) != 0) {
		__logger(error, "posix_spawn %s", strerror(errno));
		return (false);
	}

	return (true);
}
