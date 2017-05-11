TEST = battlestix
all:
	gcc $(TEST).c -o $(TEST) -Wall -Werror -pedantic -lcurses -I../../include -L../../lib -llist
clean:
	rm -f $(TEST).o
relib:
	make re -C ../list
	make all
fclean:
	rm -f $(TEST) $(TEST).o
re:
	make fclean all
run:
	./$(TEST)
