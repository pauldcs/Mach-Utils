NAME       := libkernutils.a
CC         := clang
SRCS_DIR   := srcs
OBJS_DIR   := .objs
BUILD_DIR  := build
INCS_DIR   := incs
CFLAGS     := \
	-Wall \
	-Wextra \
	-Werror \
	-pedantic

CFLAGS_DBG := \
	-g3                      \
	-O0                      \
	-fsanitize=address       \
	-fsanitize=undefined     \
	-fno-omit-frame-pointer  \
	-fstack-protector-strong \
	-fno-optimize-sibling-calls 

SRCS := \
	common/hexdump.c \
	common/fs.c \
	common/vec.c \
	common/spawn.c \
	common/strpcmp.c \
	common/path-attach.c \
	common/str-to-print.c \
	common/logger.c \
	image.c \
	memory.c \
	task.c 
