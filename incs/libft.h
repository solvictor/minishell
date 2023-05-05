/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 17:48:35 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 16:37:11 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdarg.h>
# include <stdio.h>

// -----------------------//
//       BASE LIBFT       //
// -----------------------//
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(const char *s);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_atoi(const char *nptr);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strtrim(const char *s1, const char *set);
char	**ft_split(const char *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

// BONUS
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// -----------------------//
//      GET_NEXT_LINE     //
// -----------------------//
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_line
{
	char			buf[BUFFER_SIZE + 1];
	struct s_line	*next;
}				t_line;

typedef struct s_fds
{
	int				fd;
	char			buf[BUFFER_SIZE + 1];
	struct s_fds	*next;
}				t_fds;

char	*get_next_line(int fd);
char	*deplete_buf(char *buf);
char	*get_fd_buf(int fd, t_fds **fds);
void	remove_fd(int fd, t_fds **fds);
int		read_line(int fd, t_line **line, int *len, char *buf);
t_line	*read_one(int fd, int *ret, int *len, char *buf);
char	*make_line(t_line **line, int len);
void	split_buf_rest(char *tmpbuf, char *buf);
void	clear_line(t_line **line);
char	*get_endl(char *buf);
int		gnl_w(int fd, char **line);

// -----------------------//
//        FT_PRINTF       //
// -----------------------//

// SETTINGS
# ifndef PRINT_FD
#  define PRINT_FD STDOUT_FILENO
# endif
# ifndef PRINT_SIZE
#  define PRINT_SIZE 1024
# endif

// UTILITY DEFINES
# define PRINT_INIT		0
# define PRINT_RESET	1
# define SET_CONV		"cs%pdiuxX"
# define SET_FLAG		"-0# +"
# define SET_HEX		"0123456789abcdef"
# define STR_NULL		"(null)"
# define STR_NIL		"(nil)"
# define STR_ZERO		"\0"
# define PREF_XL		"0x"
# define PREF_XU		"0X"
# define PREF_PLUS		"+"
# define PREF_SPACE		" "
# define PREF_MINUS		"-"

// FLAG MASKS
# define FM_MINUS	0x01
# define FM_ZERO	0x02
# define FM_HASH	0x04
# define FM_SPACE	0x08
# define FM_PLUS	0x10

typedef struct s_print	t_print;
struct s_print
{
	char			*s;
	int				fd;
	int				ret;
	char			buf[PRINT_SIZE];
	int				pos;
	unsigned int	convi;
	char			convc;
	int				gxl;
	char			fillc;
	unsigned int	flags;
	int				width;
	int				preci;
	char			*pref;
	int				(*handler)(t_print *, va_list);
	int				bad;
};

// MAIN
int		ft_printf(const char *s, ...);
int		ft_vdprintf(int fd, const char *s, va_list valist);
int		ft_dprintf(int fd, const char *s, ...);

// UTILS
void	set_state(t_print *print, int mode, const char *s, int fd);
int		output_full(t_print *print);
int		fill_buf(t_print *print, char *s, int size, void *free);
// UTILS 2
int		malloc_safe(t_print *print, char **ptr, size_t n);
int		atoi_safe(const char *nptr);
int		maxi(int a, int b);
int		mini(int a, int b);
int		comb_len(char *a, char *b);

// CONV PARSER & UTILS
int		set_conv_state(t_print *print);
int		get_flags(t_print *print, char **s);
int		get_width(t_print *print, char **s);
int		get_preci(t_print *print, char **s);
int		get_conv(t_print *print, char **s);

// HANDLERS & UTILS
int		hdl_c(t_print *print, va_list valist);
int		hdl_s(t_print *print, va_list valist);
int		hdl_ps(t_print *print, va_list valist);
int		hdl_p(t_print *print, va_list valist);
int		hdl_di(t_print *print, va_list valist);
int		hdl_u(t_print *print, va_list valist);
int		hdl_x(t_print *print, va_list valist);
int		hdl_bad(t_print *print, va_list valist);
void	get_hex_str(t_print *print, char *x, size_t n, int caps);
void	get_diu_str(t_print *print, char *x, long n);

// FIELD UTILS
void	set_compat(t_print *print);
int		set_field_str(t_print *print, char *field, char *s, int max);
int		set_field_nums(t_print *print, char *field, char *x, int max);
void	set_field_bad(t_print *print, char *field, int *len);

// -----------------------//
//      CUSTOM LIBFT      //
// -----------------------//
int		ft_atois(int *n, char *nptr);
int		ft_abs(int n);
int		ft_min(int a, int b);
int		ft_max(int a, int b);
#endif
