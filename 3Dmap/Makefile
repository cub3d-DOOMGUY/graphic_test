# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: incho <incho@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/01 08:18:34 by incho             #+#    #+#              #
#    Updated: 2022/04/16 17:07:14 by incho            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SUFFIXES: .c .o .h

NAME	=	3dmap
NAME_DL	=	libmlx.dylib

CP		=	cp
RM		=	rm -f
MKDIR	=	mkdir -p
MV		=	mv
ECHO	=	echo
MAKE	=	make


# ----------------------------------
# Echo Colors
# ----------------------------------

NOCOLOR		= \033[0m
RED			= \033[0;31m
GREEN		= \033[0;32m
ORANGE		= \033[0;33m
BLUE		= \033[0;34m
PURPLE		= \033[0;35m
CYAN		= \033[0;36m
LIGHTGRAY	= \033[0;37m
DARKGRAY	= \033[1;30m
LIGHTRED	= \033[1;31m
LIGHTGREEN	= \033[1;32m
YELLOW		= \033[1;33m
LIGHTBLUE	= \033[1;34m
LIGHTPURPLE	= \033[1;35m
LIGHTCYAN	= \033[1;36m
WHITE		= \033[1;37m

# ----------------------------------
# Mandatory source files
# ----------------------------------

SRCS		=	main.c

# ----------------------------------
# Object files
# ----------------------------------

OBJS		=	$(addprefix $(DIR_OBJ)/, $(SRCS:%.c=%.o))

# ----------------------------------
# Directories and Paths
# ----------------------------------

DIR_INC	=	.
DIR_SRC	=	.
DIR_OBJ	=	./obj
DIR_MLX =	./mlx

VPATH	=	$(DIR_SRC) $(addprefix $(DIR_SRC)/, $(SUBDIRS))

# ----------------------------------
# Compiler and flags
# ----------------------------------

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror
FRMWRK	=	-framework Metal -framework MetalKit

# ----------------------------------
# Rules
# ----------------------------------

all				:	$(NAME)

run				:	all
			./$(NAME)

$(DIR_OBJ)		:
			@$(MKDIR) $@

$(NAME)			:	$(DIR_OBJ) $(OBJS)
			$(CC) $(CFLAGS) -I$(DIR_INC) -L. -lmlx $(FRMWRK) $(OBJS) -o $(NAME)
			@$(ECHO) "Maked $(BLUE)$(NAME)$(NOCOLOR) $(GREEN)Successfully$(NOCOLOR)"

$(DIR_OBJ)/%.o	:	%.c
			$(CC) $(CFLAGS) -I$(DIR_INC) -c $< -o $@
			@$(ECHO) "Compiled $(ORANGE)" $< " $(GREEN)Successfully$(NOCOLOR)"

clean:
			@$(RM) -r $(DIR_OBJ)
			@$(ECHO) "Clean up $(RED)$(NAME)$(NOCOLOR)"

fclean			:	clean
			@$(RM) -r $(NAME)
			@$(ECHO) "Removed $(RED)$(NAME)$(NOCOLOR)"

re				:	fclean $(NAME)

.PHONY			:	clean fclean re