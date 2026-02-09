# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/02 15:24:08 by toferrei          #+#    #+#              #
#    Updated: 2026/02/09 20:10:03 by toferrei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserver

# Directories
SRC			=	src/
SRV 		=	Server/
CMD			=	commands/

SOURCE		=	$(SRC)main.cpp \
				$(SRC)Channel/Channel.cpp \
				$(SRC)Client/Client.cpp \
				$(SRC)$(SRV)Server.cpp \
				$(SRC)$(SRV)commandHandler.cpp \
				$(SRC)$(SRV)$(CMD)cap.cpp \
				$(SRC)$(SRV)$(CMD)pass.cpp \
				$(SRC)$(SRV)$(CMD)nick.cpp \
				$(SRC)$(SRV)$(CMD)user.cpp \
				$(SRC)$(SRV)$(CMD)privmsg.cpp \
				$(SRC)$(SRV)$(CMD)ping.cpp \
				$(SRC)$(SRV)$(CMD)mode.cpp \
				$(SRC)$(SRV)$(CMD)join.cpp \
				$(SRC)$(SRV)$(CMD)invite.cpp
				
OBJ_DIR		=	Objects/

OBJECTS		=	$(patsubst %.cpp, $(OBJ_DIR)%.o, $(SOURCE))

CXX			=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -g -std=c++98

all: $(NAME)

$(NAME): $(OBJECTS)
	@$(CXX) $(CPPFLAGS) $(OBJECTS) -o $(NAME)
	@echo "All files were created"

$(OBJ_DIR)%.o: %.cpp 
	@mkdir -p $(dir $@)
	@$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJECTS)
	@rm -rf $(OBJ_DIR)
	@echo "Object files were cleaned"

# Full clean
fclean: clean
	@rm -f $(NAME)
	@echo "All files were cleaned"

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re