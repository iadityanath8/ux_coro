CC = cc
CFlags = -Wall -Wextra -Wnonnull -pedantic

run:
	$(CC) $(CFlags) -o main examples/main.c && ./main 

clean:
	rm -rf main
