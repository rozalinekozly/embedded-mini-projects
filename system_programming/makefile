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
	@echo "$(YELLOW)  make git   - smart git add/commit/push$(RESET)"
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
git:
	@files=$$(find . -type f -mmin -60 ! -path "./.git/*"); \
	if [ -z "$$files" ]; then \
		echo "$(YELLOW)no recent files found$(RESET)"; \
		exit 0; \
	fi; \
	echo "$(YELLOW)recent files (last 60 min):$(RESET)"; \
	i=1; \
	for f in $$files; do \
		echo "$$i) $$f"; \
		eval file_$$i="$$f"; \
		i=$$((i+1)); \
	done; \
	echo ""; \
	echo "$(YELLOW)pick number (or 0 for all):$(RESET)"; \
	read choice; \
	if [ "$$choice" = "0" ]; then \
		git add $$files; \
		label="multiple files"; \
	else \
		eval selected=\$$file_$$choice; \
		git add $$selected; \
		label="$$(basename $$selected)"; \
	fi; \
	echo "$(YELLOW)enter commit message:$(RESET)"; \
	read msg; \
	git commit -m "$$label : $$msg" && git push && \
	echo "$(GREEN)pushed successfully ^_^ good job roza <3 $(RESET)"
clear:
	@rm -f $(TARGETS)
	@echo "$(YELLOW)--- binaries removed ---$(RESET)"
