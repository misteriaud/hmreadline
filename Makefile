NAME=libhmreadline.a
TESTNAME=unit_test
SCRS_TEST=.unit_testing/main.c
SRCSDIR=srcs
OBJSDIR=objs
SRCS=xreadline.c ft_itoa.c
OBJS=$(SRCS:%.c=$(OBJSDIR)/%.o)
CC = gcc
FLAGS = -Wall -Wextra -Werror -g
HDRS= includes

all: $(NAME)

$(NAME): ${OBJS}
	@echo "// BUILDING $(NAME) //"
	@ar rc $(NAME) $(OBJS)
	@echo "Todo bien !"

$(OBJSDIR)/%.o: $(SRCSDIR)/%.c $(HDRS)/$(NAME:.a=.h) Makefile
	@mkdir -p $(dir $@)
	@${CC} ${FLAGS} -c -I$(HDRS) -o $@ $<

libmemory.a: libmemory
	make -C libmemory
	mv libmemory/libmemory.a .

$(TESTNAME): libmemory.a $(OBJS) $(SCRS_TEST)
	@echo "// UNIT_TESTING LOADING //"
	@$(CC) $(FLAGS) -I$(HDRS) $(SCRS_TEST) $(OBJS) -L. -lmemory -o $@
	@./$(TESTNAME)

clean:
	rm -rf ${OBJSDIR}
	make -C libmemory fclean

fclean: clean
	${RM} ${NAME} libmemory.a

re: fclean all

.PHONY: all clean fclean re
