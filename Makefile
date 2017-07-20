CC      = gcc
CFLAGS  = -Wall -pedantic -g3 -std=c99
SRCDIR  = src
OBJDIR  = obj

TARGET	= TestMain.exe
SRCS    = ${wildcard $(SRCDIR)/*.c}
OBJS    = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(TARGET)

print:
	@echo SRCS   = $(SRCS)
	@echo OBJS   = $(OBJS)
	@echo TARGET = $(TARGET)

compile: $(OBJS)

$(OBJDIR)/%.o:
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $(@:$(OBJDIR)%.o=$(SRCDIR)%.c) -o $@

exe: $(TARGET)

$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo 'Finished building target: $@'

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f -r $(TARGET) $(OBJDIR)
