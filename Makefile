# Project structure
NAME	=	minishell
INCDIR	=	incs
LIBDIR	=	libs
SRCDIR	=	srcs
OBJDIR	=	objs
LIBFT	=	libft

# Compiler options
CC		=	cc
CWARNS	=	-Wall -Wextra -MD
CLIBS	=	-L./$(LIBDIR) -lft -lreadline
CINCS	=	-I./$(INCDIR) -I./$(LIBFT)

# Other
RM		=	rm -rf
DEBUG	=	-g3 -gdwarf-4
# Colors
COL_FRONT	=	\033[44;37;1m
COL_BACK	=	\033[7m
COL_ERR		=	\033[41;37;5;1m
COL_OK		=	\033[32;1m
COL_RESET	=	\033[0m

# Files
SRCS	:=	minishell.c \
			setup/setup.c \
			setup/setup_env.c \
			utils/utils.c \
			utils/utils_env.c \
			builtins/pwd/pwd.c \
			builtins/cd/cd.c \
			builtins/exit/exit.c \
			builtins/echo/echo.c \
			tests/test_parenthesis.c \
			tests/test_parsing.c \
			tests/test_tokenizer.c \
			tests/test_builtins_finder.c \
			tests/test_signals.c \
			tests/test_pathfinding.c
OBJS	:=	$(SRCS:.c=.o)
SRCS	:=	$(addprefix $(SRCDIR)/, $(SRCS))
OBJS	:=	$(addprefix $(OBJDIR)/, $(OBJS))

all: $(OBJDIR) $(NAME)

bonus: all

$(NAME): $(LIBDIR)/libft.a $(OBJS)
	$(CC) $(CWARNS) $(OBJS) $(CLIBS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CWARNS) $(CINCS) -c $< -o $@

$(LIBDIR)/libft.a:
	make -C $(LIBDIR)/$(LIBFT)
	cp $(LIBDIR)/$(LIBFT)/libft.a $(LIBDIR)/

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/builtins
	mkdir -p $(OBJDIR)/builtins/pwd
	mkdir -p $(OBJDIR)/builtins/cd
	mkdir -p $(OBJDIR)/builtins/exit
	mkdir -p $(OBJDIR)/builtins/echo
	mkdir -p $(OBJDIR)/setup
	mkdir -p $(OBJDIR)/utils
	mkdir -p $(OBJDIR)/tests # REMOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOVE later

clean:
	make clean -C $(LIBDIR)/$(LIBFT)
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(LIBDIR)/$(LIBFT)/libft.a
	$(RM) $(LIBDIR)/libft.a
	$(RM) $(NAME)

re: fclean all

rebonus: fclean bonus

-include $(OBJS:.o=.d)

norm:
	@echo -e "$(COL_FRONT)              $(COL_RESET)"
	@echo -e "$(COL_FRONT)  LIBS NORM:  $(COL_BACK)  $(COL_RESET)"
	@echo -e "$(COL_FRONT)              $(COL_BACK)  $(COL_RESET)"
	@echo -e " $(COL_BACK)               $(COL_RESET)"
	@norminette $(LIBDIR) | awk '{if ($$NF == "OK!") { print "$(COL_OK)"$$0"$(COL_RESET)" } else if ($$NF == "Error!") { print "$(COL_ERR)"$$0"$(COL_RESET)" } else { print }}'
	@echo -e "\n"
	@echo -e "$(COL_FRONT)              $(COL_RESET)"
	@echo -e "$(COL_FRONT)  SRCS NORM:  $(COL_BACK)  $(COL_RESET)"
	@echo -e "$(COL_FRONT)              $(COL_BACK)  $(COL_RESET)"
	@echo -e " $(COL_BACK)               $(COL_RESET)"
	@norminette $(SRCDIR) | awk '{if ($$NF == "OK!") { print "$(COL_OK)"$$0"$(COL_RESET)" } else if ($$NF == "Error!") { print "$(COL_ERR)"$$0"$(COL_RESET)" } else { print }}'
	@echo -e "\n"
	@echo -e "$(COL_FRONT)              $(COL_RESET)"
	@echo -e "$(COL_FRONT)  INCS NORM:  $(COL_BACK)  $(COL_RESET)"
	@echo -e "$(COL_FRONT)              $(COL_BACK)  $(COL_RESET)"
	@echo -e " $(COL_BACK)               $(COL_RESET)"
	@norminette $(INCDIR) | awk '{if ($$NF == "OK!") { print "$(COL_OK)"$$0"$(COL_RESET)" } else if ($$NF == "Error!") { print "$(COL_ERR)"$$0"$(COL_RESET)" } else { print }}'

.PHONY: all bonus clean fclean re rebonus norm $(OBJDIR)
