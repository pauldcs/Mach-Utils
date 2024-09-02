#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#if __MINGW32__ || (defined(_WIN32) || defined(_WIN64)) && !defined(__GNUC__)
#define MACRO_LOG_FUNC __FUNCTION__
#define MACRO_WEAK_SYM
#elif defined(__EMSCRIPTEN__)
#define MACRO_LOG_FUNC __func__
#define MACRO_WEAK_SYM
#else
#define MACRO_LOG_FUNC __func__
#define MACRO_WEAK_SYM __attribute__((weak))
#endif

#include <stdint.h>

#define __logger(lvl, fmtstr, ...) \
	logger(lvl, __FILE__, MACRO_LOG_FUNC, __LINE__, fmtstr, ##__VA_ARGS__);

typedef enum e_log_level {
	verbose,
	info,
	debug,
	warning,
	error,
	fatal
} t_log_level;

extern t_log_level __log_level__;

void logger(t_log_level level, const char *filename, const char *func,
	    uint32_t lineno, const char *format, ...);

size_t	ascii_16_bytes(uint8_t *dst, const uint8_t *src, size_t n);
size_t	data_16_bytes_color(uint8_t *buffer, const uint8_t *addr, size_t n);
size_t	data_16_bytes(const uint8_t *dst, const uint8_t *src, size_t n);
size_t	pointer_16_bytes(uint8_t *dst, const uintptr_t p);
ssize_t hexdump(const uint8_t *addr, size_t n, size_t offset);

bool file_exists(const char *fn);
bool is_file_directory(const char *fn);
bool is_file_regular(const char *fn);
bool file_get_size(const char *fn, size_t *size);
bool file_open_read(const char *fn, int *fd);
bool file_open_append(const char *fn, int *fd);
bool file_open_write(const char *fn, int *fd);
bool fd_read_at(int fd, void *dest, size_t n, off_t offset);
bool fd_sneek_read(int fd, void *dest, size_t n);
bool fd_read(int fd, void *dest, size_t n);

const char *str_to_print(char *buf, size_t bufsiz, const char *str);
char	   *path_attach(const char *dirname, const char *name);

#endif /* __COMMON__ */
