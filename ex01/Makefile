NAME	=	Fixed

CC		=	@c++ -Wall -Werror -Wextra -std=c++98
RM		=	@rm -f

FILES	=	Fixed.cpp main.cpp

SRCS	=	$(FILES)

OBJ		=	$(SRCS:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
			@ $(CC) $(OBJ) -o $(NAME)
			@ echo "\033[1;32m$(NAME) created.\033[0m"

%.o: %.cpp
			@ $(CC) -c $< -o $@

clean:
			@ $(RM) $(OBJ)
			@ echo "\033[1;31m$(OBJ) deleted.\033[0m"

fclean:
			@ $(RM) $(NAME)
			@ $(RM) $(OBJ)
			@ echo "\033[1;31m$(NAME) deleted.\033[0m"

re:			fclean all

.PHONY:		all clean fclean re