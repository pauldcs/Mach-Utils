#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *path_attach(const char *dirname, const char *name)
{
	size_t size = strlen(dirname) + strlen(name) + (2 * sizeof(char));
	char  *dest = malloc(size);
	if (!dest) {
		return (NULL);
	}

	(void)memset(dest, 0, size);

	const char *destp    = dest;
	const char *dirnamep = dirname;

	if (dirname[0] != '.' || dirname[1] != '\0') {
		while (*dirnamep) {
			*dest++ = *dirnamep++;
		}

		if (dirnamep > dirname && dirnamep[-1] != '/') {
			*dest++ = '/';
		}
	}

	while (*name) {
		*dest++ = *name++;
	}

	return ((char *)destp);
}
