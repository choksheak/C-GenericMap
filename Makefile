CC      = gcc
CFLAGS  = -Wall -pedantic -g3 -std=c99
SRCDIR  = src

TARGET	= TestMain.exe
SRCS    := ${wildcard $(SRCDIR)/*.c}
OBJS	:= $(SRCS:%.c=%.o)

all: $(TARGET)

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo 'Finished building target: $@'

clean:
	rm -f $(TARGET) $(OBJS) *~
