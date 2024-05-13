ifeq ($(OS),Windows_NT)
	CC = cl
	RM = cmd \/C del
	TARGET_EXT = .exe
	CFlags = /EHsc /W4
	LDFlags = /link win\kernel32.Lib
else
	CC = cc
	RM = rm -rf 
	TARGET_EXT = 
	CFlags = -Wall -Wextra -pedantic
endif

<<<<<<< HEAD
run:
	$(CC) $(CFlags) -o main examples/main.c && ./main 

clean:
	rm -rf main
=======
SRCS = examples\main.c

TARGET = main$(TARGET_EXT)

all: $(TARGET)

$(TARGET):$(SRCS)
	$(CC) -o $(TARGET) $(CFlags) $^ $(LDFlags) 

.PHONY: clean
clean:
	$(RM) $(TARGET)
>>>>>>> 34c0d0cb8db4aca72d71ae2c40c1ab755c0d45dc
