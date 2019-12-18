CPPC=@g++
CPPFLAGS=-std=c++98 -MD -O2 -Wall -Wextra -Wno-misleading-indentation
LFLAGS= -O2
OBJSC:=main.elf tui.elf term.elf window.elf controls.elf menu.elf edit.elf
DEPS=$(OBJSC:.elf=.d)
RM=@rm -f

all:edit

edit:$(OBJSC)
	$(CPPC) $(OBJSC) -o edit $(LFLAGS)

%.elf:%.cpp
	$(CPPC) -c $< -o $@ $(CPPFLAGS)

-include $(DEPS)

clean:
	-$(RM) $(OBJSC)
	-$(RM) $(DEPS)
	-$(RM) edit

rebuild: clean all

.PHONY: all clean rebuild
