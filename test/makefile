TEST = testlist
all:
	make re -C ../src/list
	gcc $(TEST).c -o ./$(TEST) -std=c99 -Wall -Werror -pedantic -I../include -L../lib -lmy -llist
clean:
	rm -f $(TEST).o
fclean:
	rm -f ./$(TEST) $(TEST).o
re:
	make fclean all
run:
	./$(TEST)
relib:
	make re run
only:
	gcc $(TEST).c -o ./$(TEST) -std=c99 -Wall -Werror -pedantic -I../include -L../lib -lmy -llist
	make run
