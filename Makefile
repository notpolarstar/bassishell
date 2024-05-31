##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## C++ Makefile
##

SRC	=	src/main.cpp	\
		src/pipes.cpp	\
		src/redirections.cpp

OBJ	=	$(SRC:.cpp=.o)

NAME	=	bassishell

CFLAGS	=	-W -Wall -Wextra

CPPFLAGS = 	-I./include

STD	=	-std=c++17

TESTS_FLAGS	=	--coverage -lcriterion

TESTS_NAME	=	unit_tests

TESTS_SRC	=	tests/tests.c	\
			tests/parsing.c	\
			tests/print_numbers.c

TESTS_OBJ	=	$(TESTS_SRC:.c=.o)

CC	=	g++

all: $(NAME)

$(NAME):	$(OBJ)
		$(CC) -o $(NAME) $(STD) $(OBJ) $(CFLAGS) $(CPPFLAGS)

$(LIBMY):
		make -C lib

debug:	CFLAGS += -ggdb3 -Og -fsanitize=address
debug:	$(NAME)

unit_tests:	$(TESTS_OBJ)
		$(CC) -o $(TESTS_NAME) $(TESTS_SRC) $(TESTS_FLAGS) $(CFLAGS)

tests_run:	unit_tests
		./unit_tests && gcovr

clean:
		rm -f $(OBJ)
		find . -name "*~" -delete
		find . -name "#*#" -delete
		find . -name "*.o" -delete
		find . -name "unit_tests" -delete
		find . -name "*.gcda" -delete
		find . -name "*.gcno" -delete
		find . -name "*.gcov" -delete
		find . -name "vgcore.*" -delete

fclean:		clean
		rm -f $(NAME)

re:		fclean all

.PHONY: all clean fclean re unit_tests tests_run debug
