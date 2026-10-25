NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS		=	src/main.c \
				src/validate_args.c \
				src/check_number.c \
				src/start_coders.c \
				src/start_simulation.c \
				src/finish_simulation.c \
				src/join_threads.c \
				src/init_data.c \
				src/init_waiters.c \
				src/init_sync.c \
				src/init_dongles.c \
				src/init_arrays.c \
				src/init_resources.c \
				src/heap.c \
				src/heap_compare.c \
				src/heap_reorder.c \
				src/heap_remove.c \
				src/coder.c \
				src/phase.c \
				src/compile.c \
				src/scheduler.c \
				src/dongle_utils.c \
				src/requests.c \
				src/monitor.c \
				src/time_utils.c

OBJ_DIR = obj

OBJS		= $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

HEADER		= codexion.h

RM			= rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c $(HEADER)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re