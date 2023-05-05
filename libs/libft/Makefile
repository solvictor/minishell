# Compiler options
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

# Other
RM	= rm -rf
AR	= ar rcs

# Files
NAME	=	libft.a
SRCS	=	ft_isalpha.c \
			ft_isdigit.c \
			ft_isalnum.c \
			ft_isascii.c \
			ft_isprint.c \
			ft_strlen.c \
			ft_memset.c \
			ft_bzero.c \
			ft_memcpy.c \
			ft_memmove.c \
			ft_strlcpy.c \
			ft_strlcat.c \
			ft_toupper.c \
			ft_tolower.c \
			ft_strchr.c \
			ft_strrchr.c \
			ft_strncmp.c \
			ft_memchr.c \
			ft_memcmp.c \
			ft_strnstr.c \
			ft_atoi.c \
			ft_calloc.c \
			ft_strdup.c \
			ft_substr.c \
			ft_strjoin.c \
			ft_strtrim.c \
			ft_split.c \
			ft_itoa.c \
			ft_strmapi.c \
			ft_striteri.c \
			ft_putchar_fd.c \
			ft_putstr_fd.c \
			ft_putendl_fd.c \
			ft_putnbr_fd.c \
			ft_lstnew.c \
			ft_lstadd_front.c \
			ft_lstsize.c \
			ft_lstlast.c \
			ft_lstadd_back.c \
			ft_lstdelone.c \
			ft_lstclear.c \
			ft_lstiter.c \
			ft_lstmap.c \
			get_next_line.c \
			get_next_line_utils.c \
			get_next_line_wrapper.c \
			ft_printf.c \
			ft_printf_utils.c \
			ft_printf_utils2.c \
			ft_printf_conv_parser.c \
			ft_printf_handler_utils.c \
			ft_printf_handler_nums.c \
			ft_printf_handler_chars.c \
			ft_printf_field_utils.c \
			ft_atois.c \
			ft_abs.c \
			ft_min.c \
			ft_max.c
OBJS	=	$(SRCS:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

ls:
	echo $(SRCS)
	echo $(OBJS)

.PHONY: all clean fclean re
