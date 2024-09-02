include libkernutils.mk

SRCS_OBJS := $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS))

$(OBJS_DIR)/%.o:$(SRCS_DIR)/%.c
	mkdir -vp $(dir $@)
	$(CC) \
		$(CFLAGS) \
		-MMD \
		-MP \
		-o $@ \
		-c $< \
		-I $(INCS_DIR)

all: $(NAME)

-include  $(SRCS_OBJS:.o=.d)

$(NAME): $(SRCS_OBJS)
	ar rc \
		$(NAME) \
		$(SRCS_OBJS)

asan: CFLAGS += $(CFLAGS_ASAN)
asan: all

release: CFLAGS = $(CFLAGS_RELEASE)
release: all

format:
	find . \( -name "*.c" -o -name "*.h" \) \
		-type f \
		-exec \
			clang-format \
			--verbose \
			-style=file -i {} \;

clean:
	rm -rf *.dSYM
	rm -rf .cache
	rm -rf .vscode
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
ra: fclean asan

.PHONY	: \
	all     \
	clean   \
	release \
	fclean  \
	format  \
	re      \
	ra
