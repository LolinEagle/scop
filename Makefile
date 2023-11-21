# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frrusso <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 12:52:18 by frrusso           #+#    #+#              #
#    Updated: 2023/11/21 12:52:20 by frrusso          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	scop
SRCS		=	main.cpp \
				ve_window.cpp
CPP			=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -MMD
RM			=	rm -rf
LIBS		=	-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
INCS		=	-I ./incs/
SRCS_PATH	=	./srcs/
OBJS_PATH	=	./objs/
OBJS_DIRS	=	$(OBJS_PATH)
OBJS		=	$(addprefix $(OBJS_PATH),$(SRCS:.cpp=.o))
DEPS		=	$(addprefix $(OBJS_PATH),$(SRCS:.cpp=.d))

$(OBJS_PATH)%.o:$(SRCS_PATH)%.cpp
	$(CPP) $(CPPFLAGS) $(INCS) -c $< -o $@

$(OBJS_DIRS):
	mkdir $(OBJS_DIRS)

$(NAME):$(OBJS_DIRS) $(OBJS)
	$(CPP) $(OBJS) $(LIBS) -o $@

all:$(NAME)

clean:
	$(RM) $(OBJS_PATH)

fclean:clean
	$(RM) $(NAME)

re:fclean
	make all

run:re
	clear && ./$(NAME)

-include $(DEPS)
