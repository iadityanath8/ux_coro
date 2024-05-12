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

SRCS = examples\main.c

TARGET = main$(TARGET_EXT)

all: $(TARGET)

$(TARGET):$(SRCS)
	$(CC) -o $(TARGET) $(CFlags) $^ $(LDFlags) 

.PHONY: clean
clean:
	$(RM) $(TARGET)
