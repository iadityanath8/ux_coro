CC = cc
CFlags = -Wall -Wextra -Wnonnull -pedantic

comp:
	$(CC) $(CFlags) -o main examples/main.c && ./main 
