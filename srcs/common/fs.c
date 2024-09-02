#include "common.h"
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool file_exists(const char *fn)
{
	return (access(fn, F_OK) == 0);
}

bool is_file_directory(const char *fn)
{
	struct stat sb = { 0 };
	if (stat(fn, &sb) == 0) {
		return ((S_IFDIR & sb.st_mode) == S_IFDIR);
	}

	__logger(error, "is_file_directory: %s", strerror(errno));
	return (false);
}

bool is_file_regular(const char *fn)
{
	struct stat sb = { 0 };
	if (stat(fn, &sb) == 0) {
		return ((S_IFREG & sb.st_mode) == S_IFREG);
	}

	return (false);
}

bool file_get_size(const char *fn, size_t *size)
{
	struct stat sb = { 0 };
	if (stat(fn, &sb) == 0) {
		*size = sb.st_size;
		return (true);
	}

	__logger(error, "file_get_size: %s", strerror(errno));
	return (false);
}

bool file_open_read(const char *fn, int *fd)
{
	*fd = open(fn, O_RDONLY, 0666);
	if (*fd != -1)
		return (true);

	__logger(error, "file_open_read: %s", strerror(errno));
	return (false);
}

bool file_open_append(const char *fn, int *fd)
{
	*fd = open(fn, O_CREAT | O_APPEND | O_WRONLY, 0666);
	if (*fd != -1)
		return (true);

	__logger(error, "file_open_append: %s", strerror(errno));
	return (false);
}

bool file_open_write(const char *fn, int *fd)
{
	*fd = open(fn, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (*fd != -1)
		return (true);

	__logger(error, "file_open_write: %s", strerror(errno));
	return (false);
}

bool fd_read_at(int fd, void *dest, size_t n, off_t offset)
{
	ssize_t ret;

	ret = lseek(fd, offset, SEEK_SET);
	if (ret == -1) {
		__logger(error, "fd_read_at: Cannot lseek to offset: %s",
			 strerror(errno));
		return (false);
	}
	if (ret != (ssize_t)offset) {
		__logger(error, "fd_read_at: Seeked to offset %d instead of %d",
			 ret, offset);
		return (false);
	}

	ret = read(fd, dest, n);
	if (ret == -1) {
		__logger(error, "fd_read_at: Cannot read the file: %s",
			 strerror(errno));
		return (false);
	}
	if (ret != (ssize_t)n) {
		__logger(error,
			 "fd_read_at: Could only read %d bytes out of %d", ret,
			 n);
		return (false);
	}
	return (true);
}

bool fd_sneek_read(int fd, void *dest, size_t n)
{
	off_t	start_offset;
	ssize_t ret;

	start_offset = lseek(fd, 0, SEEK_CUR);
	if (start_offset == -1) {
		__logger(error, "fd_sneek_read: Lseek failed: %s",
			 strerror(errno));
		return (false);
	}

	ret = read(fd, dest, n);
	if (ret == -1) {
		__logger(error, "fd_sneek_read: Cannot read the file: %s",
			 strerror(errno));
		return (false);
	}
	if (ret != (ssize_t)n) {
		__logger(error,
			 "fd_sneek_read: Could only read %d bytes out of %d",
			 ret, n);
		return (false);
	}

	ret = lseek(fd, start_offset, SEEK_SET);
	if (ret == -1) {
		__logger(error, "fd_sneek_read: Lseek failed: %s",
			 strerror(errno));
		return (false);
	}
	if (ret != (ssize_t)start_offset) {
		__logger(error,
			 "fd_sneek_read: Seeked to offset %d instead of %d",
			 ret, start_offset);
		return (false);
	}
	return (true);
}

bool fd_read(int fd, void *dest, size_t n)
{
	ssize_t ret;

	ret = read(fd, dest, n);
	if (ret == -1) {
		__logger(error, "fd_read: Read failed: %s", strerror(errno));
		return (false);
	}

	if (ret != (ssize_t)n) {
		__logger(error, "fd_read: Could only read %d bytes out of %d",
			 ret, n);
		return (false);
	}

	return (true);
}
