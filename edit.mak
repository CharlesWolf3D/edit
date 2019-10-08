CC=@gcc
CFLAGS=-std=gnu99 -MD -O2 -Wall -Wextra
LFLAGS= -O2
OBJSC:=edit.elf term.elf ventana.elf
DEPS=$(OBJSC:.elf=.d)
RM=@rm -f

all:edit

edit:$(OBJSC)
	$(CC) $(OBJSC) -o edit $(LFLAGS)

%.elf:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

-include $(DEPS)

clean:
	-$(RM) $(OBJSC)
	-$(RM) $(DEPS)
	-$(RM) edit

rebuild: clean all

.PHONY: all clean rebuild
