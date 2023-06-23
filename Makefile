# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/23 08:28:27 by nlegrand          #+#    #+#              #
#    Updated: 2023/06/23 21:57:50 by nlegrand         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
			setup/setup_utils.c \
			utils/utils.c \
			utils/utils_env.c \
			utils/utils_tokens.c \
			tokenizer/tokenizer.c \
			tokenizer/tokenizer_utils_1.c \
			tokenizer/tokenizer_utils_2.c \
			tokenizer/tokenizer_utils_3.c \
			parser/parser.c \
			parser/parser_utils.c \
			builtins/pwd/pwd.c \
			builtins/cd/cd.c \
			builtins/exit/exit.c \
			builtins/echo/echo.c \
			builtins/export/export.c \
			builtins/unset/unset.c \
			builtins/env/env.c \
			test_code_remove_later/display_utils.c \
			test_code_remove_later/test_expansion.c
#			test_code_remove_later/test_signals.c
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
	mkdir -p $(OBJDIR)/builtins/export
	mkdir -p $(OBJDIR)/builtins/unset
	mkdir -p $(OBJDIR)/builtins/env
	mkdir -p $(OBJDIR)/setup
	mkdir -p $(OBJDIR)/utils
	mkdir -p $(OBJDIR)/tokenizer
	mkdir -p $(OBJDIR)/parser
	mkdir -p $(OBJDIR)/test_code_remove_later

clean:
	make clean -C $(LIBDIR)/$(LIBFT)
	$(RM) $(OBJDIR)

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

.PHONY: all bonus clean fclean re rebonus norm
