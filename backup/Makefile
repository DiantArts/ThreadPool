##
## EPITECH PROJECT, 2020
## Makefile
## File description:
## Makefile of the entire project
##

ARGV		:=

CFLAGS		+=	-Wpedantic -Wall -Wextra
CPPFLAGS	+=	-Iincludes
LDLIBS		+=	-lpthread
LDFLAGS>	+=	-L$(CURDIR)

ifeq (debug,$(findstring debug, $(MAKECMDGOALS)))
NAME		:=	timeControlled_debug
CFLAGS		+=	-Ofast -pipe -DDEBUG=2
OBJDIR		:=	.objects/debug
else ifeq (auto_valgrind,$(findstring auto_valgrind, $(MAKECMDGOALS)))
NAME		:=	bomberman_valgrind
CFLAGS		+=	-g3 -Og -pipe -DDEBUG=1
OBJDIR		:=	.objects/debug
else ifeq (tests_run,$(findstring tests_run, $(MAKECMDGOALS)))
NAME		:=	unit_tests
CFLAGS		+=	-Ofast -pipe --coverage
OBJDIR		:=	.objects/tests
LDLIBS		+=	-lcriterion
else
NAME		:=	timeControlled
CFLAGS		+=	-Ofast -pipe
OBJDIR		:=	.objects/release
endif

all : $(NAME)

ifeq (tests_run,$(findstring tests_run, $(MAKECMDGOALS)))
$(NAME) : $(patsubst ./%cpp,$(OBJDIR)/%o,$(shell find ./sources -type f -name \*.cpp ! -path ./sources/main.cpp) ./tests/tests.cpp)
	$(CXX) $(OUTPUT_OPTION) $(LDLIBS) $(LDFLAGS) --coverage $^
	@echo -e "$(LRED)[Linkage]$(NORMAL) $^ $(LDLIBS)"
else
$(NAME) : $(patsubst ./%cpp,$(OBJDIR)/%o,$(shell find ./sources -type f -name \*.cpp))
	$(CXX) $(OUTPUT_OPTION) $(LDLIBS) $(LDFLAGS) $^
	@echo -e "$(LRED)[Linkage]$(NORMAL) $^ $(LDLIBS)"
endif

$(OBJDIR)/%o : %cpp
	mkdir -p $(@D) $(patsubst .objects%,.dependencies%,$(@D))
	$(CXX) -c $(OUTPUT_OPTION) $(CFLAGS) $(CPPFLAGS) $< -MT $@ -MMD -MP -MF $(patsubst .objects%o,.dependencies/%d,$@)
	@echo -e "$(LRED)[Compilation]$(NORMAL) $<"

include $(shell find . -type f -name \*.d)

clean :
	rm -rf .objects .dependencies vgcore.*
	@echo -e "$(DARKGRAY)[Clean]$(NORMAL) done"

fclean : clean
	rm -f $(NAME) $(NAME)_debug unit_tests
	@echo -e "$(DARKGRAY)[Fclean]$(NORMAL) done"

re :
	$(MAKE) fclean
	$(MAKE) all

auto : all ; @echo -e "$(YELLOW)[Binary]$(NORMAL) auto $(ARGV)" ; valgrind -q ./$(NAME) $(ARGV)
debug : all ; @echo -e "$(YELLOW)[Binary]$(NORMAL) debug $(ARGV)" ; valgrind -q ./$(NAME) $(ARGV)
auto_valgrind : all ; @echo -e "$(YELLOW)[Binary]$(NORMAL) auto without valgrind $(ARGV)" ; ./$(NAME) $(ARGV)
tests_run : all ; @echo -e "$(YELLOW)[Binary]$(NORMAL) tests_run" ; ./$(NAME) ; gcovr -be tests

NORMAL			:=	\e[0m
DARKGRAY		:=	\e[1;30m
LRED			:=	\e[1;31m
YELLOW			:=	\e[1;33m

MAKEFLAGS		+=	--silent --no-print-directory
.PHONY: all clean fclean re $(NAME) $(NAME)_debug unit_tests auto debug auto_valgrind tests_run
