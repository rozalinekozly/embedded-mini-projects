CC      = gcc
FLAGS_D = -ansi -pedantic-errors -Wall -Wextra -g
FLAGS_R = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
INCLUDES = -I ../utils/include
YELLOW = \033[1;33m
GREEN  = \033[1;32m
RESET  = \033[0m
SRCS    = $(wildcard src/*.c)
TARGETS = $(notdir $(SRCS:.c=))
.DEFAULT_GOAL := help
help:
	@echo "$(YELLOW)available targets:$(RESET)"
	@$(foreach t, $(TARGETS), \
		echo "$(YELLOW)  make d $(t).c  - debug build$(RESET)"; \
		echo "$(YELLOW)  make r $(t).c  - release build$(RESET)"; \
		echo "$(YELLOW)     last modified: $$(date -r src/$(t).c '+%Y-%m-%d %H:%M:%S')$(RESET)";)
	@echo "$(YELLOW)  make all   - debug build everything$(RESET)"
	@echo "$(YELLOW)  make clear - remove all binaries$(RESET)"
d:
	@target="$(filter-out d, $(MAKECMDGOALS))"; \
	[ "$${target##*.}" = "c" ] && target="$${target%.c}"; \
	$(CC) $(FLAGS_D) src/$$target.c $(INCLUDES) -lm -lpthread -o $$target && \
	echo "$(YELLOW)---------- debug: $$target ----------$(RESET)" && \
	printf "$(YELLOW)type r to run, x to skip: $(RESET)" && read input && \
	[ "$$input" = "r" ] && ./$$target || true
r:
	@target="$(filter-out r, $(MAKECMDGOALS))"; \
	[ "$${target##*.}" = "c" ] && target="$${target%.c}"; \
	$(CC) $(FLAGS_R) src/$$target.c $(INCLUDES) -lm -lpthread -o $$target && \
	echo "$(YELLOW)---------- release: $$target ----------$(RESET)" && \
	printf "$(YELLOW)type r to run, x to skip: $(RESET)" && read input && \
	[ "$$input" = "r" ] && ./$$target || true
%:
	@true
all:
	@for t in $(TARGETS); do \
		$(CC) $(FLAGS_D) src/$$t.c $(INCLUDES) -lm -lpthread -o $$t; \
	done
	@echo "$(YELLOW)--- all targets built in debug mode ---$(RESET)"
clear:
	@rm -f $(TARGETS)
	@echo "$(YELLOW)--- binaries removed ---$(RESET)"
	
wd_app:
	@$(CC) -Wall -Wextra -g \
		src/wd_app.c \
		../ds/src/scheduler.c \
		../ds/src/task.c \
		../ds/src/uid.c \
		../ds/src/pqueue.c \
		../ds/src/sorted_list.c \
		../ds/src/dlist.c \
		-I ../ds/include \
		-I ../utils/include \
		-lm -lpthread \
		-o wd_app && \
	echo "$(GREEN)--- wd_app built ---$(RESET)"
