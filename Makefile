# Name of the executable
NAME        = ircserv

# Compiler and Flags
CC          = c++
CFLAGS      = -Wall -Wextra -Werror -std=c++98

# Directories
SRC_DIR     = src/
INC_DIR     = inc/
OBJ_DIR     = obj/

# Color codes for terminal output
RESET       = \033[0m
GREEN       = \033[32m
YELLOW      = \033[33m
BLUE        = \033[34m
RED         = \033[31m
UP          = \033[A
CUT         = \033[K

# Source and object files using wildcard
# This looks for all .cpp files in SRC_DIR and its subdirectories
SRC_FILES   = $(shell find $(SRC_DIR) -name "*.cpp" -type f | sed 's|^$(SRC_DIR)||') # acho qe temos de remover, nao se pode usar wildcards
OBJ_FILES   = $(SRC_FILES:.cpp=.o)

# Detect all header files for dependency tracking
HEADERS     = $(shell find $(INC_DIR) -name "*.hpp" -type f)

# Full paths
SRC         = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ         = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

# Rule for building the executable
all: $(NAME)

# Compile the executable
$(NAME): $(OBJ)
	@echo "$(YELLOW)Compiling [$(NAME)]...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)Finished [$(NAME)]$(RESET)"

# Building object files
# Included $(HEADERS) as a dependency so it re-compiles if an .hpp changes
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling [$@]...$(RESET)"
	@$(CC) $(CFLAGS) -I $(INC_DIR) -o $@ -c $<
	@printf "$(UP)$(CUT)"
	@echo "$(GREEN)Finished [$@]$(RESET)"
	@printf "$(UP)$(CUT)"

# Clean rules
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		rm -rf $(OBJ_DIR); \
		echo "$(BLUE)Deleting all objects...$(RESET)"; \
	else \
		echo "No objects to remove."; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		rm -f $(NAME); \
		echo "$(BLUE)Deleting $(NAME)...$(RESET)"; \
	else \
		echo "No Executable to remove."; \
	fi

re: fclean all

.PHONY: all clean fclean re