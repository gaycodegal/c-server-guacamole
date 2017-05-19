SERVER = server
CLIENT = client
all:
#	make re -C ../list
	gcc $(SERVER).c -o ./$(SERVER) -Wall -Werror -pedantic -I../../include -L../../lib -lmy -llist
	gcc battlestix.c -o ./$(CLIENT) -Wall -Werror -pedantic  -lcurses  -I../../include -L../../lib -lmy -llist
clean:
	rm -f $(SERVER).o $(CLIENT).o
fclean:
	rm -f ./$(SERVER) $(SERVER).o ./$(CLIENT) $(CLIENT).o
re:
	make fclean all
relib:
	make re -C ../list
	make re run

