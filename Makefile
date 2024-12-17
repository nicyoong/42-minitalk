# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories and files
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
SRC_CLIENT = client.c
SRC_SERVER = server.c
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)
OBJ_SERVER = $(SRC_SERVER:.c=.o)
NAME_CLIENT = client
NAME_SERVER = server

# Targets
all: $(NAME_CLIENT) $(NAME_SERVER)

# Build the client and server executables
$(NAME_CLIENT): $(OBJ_CLIENT) $(LIBFT)
	$(CC) $(OBJ_CLIENT) -o $(NAME_CLIENT) $(LIBFT)

$(NAME_SERVER): $(OBJ_SERVER) $(LIBFT)
	$(CC) $(OBJ_SERVER) -o $(NAME_SERVER) $(LIBFT)

# Compile the libft.a (assuming Makefile exists in libft folder)
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Clean object files
clean:
	rm -f $(OBJ_CLIENT) $(OBJ_SERVER)
	$(MAKE) -C $(LIBFT_DIR) clean

# Clean object files and the executables
fclean: clean
	rm -f $(NAME_CLIENT) $(NAME_SERVER)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything (clean + all)
re: fclean all
