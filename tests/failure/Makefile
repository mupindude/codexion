NAME		= test_thread_failure

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
LDFLAGS 	= -Wl,--wrap=pthread_create \
			  -Wl,--wrap=pthread_mutex_init \
			  -Wl,--wrap=pthread_cond_init

ROOT		= ../..
OBJ_DIR		= obj

SRCS		= $(ROOT)/src/validate_args.c \
			  $(ROOT)/src/check_number.c \
			  $(ROOT)/src/start_coders.c \
			  $(ROOT)/src/start_simulation.c \
			  $(ROOT)/src/finish_simulation.c \
			  $(ROOT)/src/join_threads.c \
			  $(ROOT)/src/init_data.c \
			  $(ROOT)/src/init_waiters.c \
			  $(ROOT)/src/init_sync.c \
			  $(ROOT)/src/init_dongles.c \
			  $(ROOT)/src/init_arrays.c \
			  $(ROOT)/src/init_resources.c \
			  $(ROOT)/src/heap.c \
			  $(ROOT)/src/heap_compare.c \
			  $(ROOT)/src/heap_reorder.c \
			  $(ROOT)/src/heap_remove.c \
			  $(ROOT)/src/coder.c \
			  $(ROOT)/src/phase.c \
			  $(ROOT)/src/compile.c \
			  $(ROOT)/src/scheduler.c \
			  $(ROOT)/src/dongle_utils.c \
			  $(ROOT)/src/requests.c \
			  $(ROOT)/src/monitor.c \
			  $(ROOT)/src/time_utils.c \
			  test_thread_failure.c

OBJS		= $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(ROOT)/src/%.c $(ROOT)/codexion.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(ROOT) -c $< -o $@

$(OBJ_DIR)/test_thread_failure.o: test_thread_failure.c $(ROOT)/codexion.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(ROOT) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: all
	./$(NAME) coder

test-coder: all
	./$(NAME) coder

test-monitor: all
	./$(NAME) monitor

.PHONY: all clean fclean re test test-coder test-monitor