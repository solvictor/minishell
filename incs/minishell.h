/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:01:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/13 20:12:38 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <errno.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

// SETTINGS
//# define MSH_PROMPT			"\001\e[38;2;255;0;255m\002minishell>\001\e[0m\002 "
# define MSH_PROMPT			"\001\e[38;2;184;123;79m\0028\001\e[38;2;252;185\
;114m\002====\001\e[38;2;201;115;158m\002D\001\e[0m\002 "
# define RNG_MAX			INT_MAX
# define RNG_BIT_ROTATIONS	13
# define RNG_ZERO_FIX_SEED	694201337
# define HEREDOC_LEN		24
# define CONT_PARENT		0
# define CONT_PARENT_WAIT	1
# define CONT_CHILD_FORK	2
# define CONT_HEREDOC		3

// ERROR MESSAGES
# define MSH_ERROR		"\e[31;7;1m[MINISHELL ERROR]\e[0m "
# define ME_AC			"Minishell takes no arguments\n"
# define ME_SIGNALS		"Failed to bind signals\n"
# define ME_ENV			"Failed to create minishell environment\n"
# define ME_RNG			"Failed to random number generator\n"

typedef struct s_context	t_context;
typedef enum e_tokentype	t_tokentype;
typedef enum e_nodetype		t_nodetype;
typedef struct s_tokenlist	t_tokenlist;
typedef struct s_rng		t_rng;
typedef struct s_cmd		t_cmd;
typedef struct s_cmdline	t_cmdline;
typedef struct s_env		t_env;
typedef struct s_msh		t_msh;
typedef int					(*t_builtin)(t_msh *, char **);

enum e_tokentype
{
	UNQUOTED_STR,
	DOUBLE_QUOTED_STR,
	SINGLE_QUOTED_STR,
	MERGED_STR,
	HEREDOC,
	I_FILE,
	O_FILE_TRUNC,
	O_FILE_APPEND,
	PIPE,
	UNKNOWN
};

struct s_context
{
	int		cur;
	t_msh	*msh;
	int		heredoc_fd;
};
struct s_tokenlist
{
	char			*data;
	t_tokentype		type;
	long			data_opt;
	t_tokenlist		*next;
};
struct s_cmd
{
	char		*path;
	int			(*builtin)(t_msh *, char **);
	char		**args;
	t_tokenlist	*start_token;
	int			num;
	int			*redirs;
	int			empty;
};
struct s_cmdline
{
	int		cmds_n;
	t_cmd	*cmds;
	int		*pipes;
	int		*redirs;
	int		*pids;
	char	**paths;
	char	**envp;
};
struct	s_env
{
	t_env	*prev;
	char	*var;
	t_env	*next;
};
struct s_rng
{
	unsigned int	rand;
	unsigned int	mult;
	unsigned int	inc;
};
struct s_msh
{
	t_env		*env;
	t_tokenlist	*tokens;
	t_cmdline	cmdline;
	int			exit;
	int			ret;
	t_rng		rng;
};

// ----- //
// SETUP //
// ----- //
int				msh_setup(t_msh	*msh, int ac, char **envp);
unsigned int	get_randint(int fd_urandom);
unsigned int	randuint_rng(t_rng *rng);

// ----- //
// UTILS //
// ----- //
void			msh_terminate(t_msh *msh, int exit_code);
void			clear_strarr(char **arr);
void			clear_cmdline(t_cmdline *cmdline);
unsigned int	rng_bit_rot(unsigned int num);
void			set_int_array(int *arr, int val, int size);
void			close_valid_fds(int	*arr, int size);
void			unlink_heredocs(t_tokenlist *tokens);
void			set_context(t_msh *msh);
void			reset_cmdline(t_cmdline *cmdline);
// Signals
void			reset_signals(void);
void			handler_sigint(int sig);
// Env
t_env			*env_new(char *var);
char			**env_to_arr(t_env *env);
void			destroy_env_list(t_env **env);
char			*get_env_val(t_env *env, char *key);
t_env			*get_env(t_env *env, char *key, int len_key);
int				set_pwds(t_msh *msh);
int				is_valid_identifier(char *str);
int				is_numeric(char *str, int *dst);
int				set_shlvl(t_msh *msh);
// Tokens
t_tokenlist		*token_add_front(t_tokenlist **begin, char *data);
void			destroy_tokenlist(t_tokenlist **begin);
int				is_redir_token(t_tokenlist *token);
int				is_str_token(t_tokenlist *token);
void			clean_redir_tokens(t_tokenlist *tokens);

// --------- //
// TOKENIZER //
// --------- //
int				tokenize(const char *input, t_tokenlist **tokens);
int				is_whitespace(char c);
int				is_metachar(char c);
t_tokentype		get_metachar_tokentype(const char *input, int *i);
int				get_str_quoted(const char *str, char **dst);
int				get_str_unquoted(const char *str, char **dst);
int				push_str_token(t_tokenlist **tokens, const char *input, int *i);
int				push_metachar_token(t_tokenlist **tokens,
					const char *input, int *i);
char			*get_str_token(const char *input, int *i);
void			reverse_tokens(t_tokenlist **begin);
int				check_syntax_errors(t_tokenlist *tokens);

// ------ //
// PARSER //
// ------ //
int				parse(t_cmdline *cmdline, t_tokenlist *tokens);

// ---- //
// PREP //
// ---- //
int				prep_cmdline(t_msh *msh, t_cmdline *cmdline,
					t_tokenlist *tokens);
char			**get_paths(t_env *env, char *key);
char			*concat_path(char *path, char *name);
int				expand_str(t_msh *msh, char **str);
int				do_dollar_expansions(t_msh *msh, t_tokenlist *tokens);
size_t			trimmed_len(char *str);
int				merge_str_tokens(t_tokenlist *tokens);
int				merge_heredoc_tokens(t_tokenlist *tokens);
int				make_cmds_args(t_cmdline *cmdline);
int				pathfind_cmds(t_cmdline *cmdline);
char			*set_heredoc_name(char *filename, long rand);
int				do_heredocs(t_msh *msh);
int				do_redirections(t_cmdline *cmdline);
int				do_redir_input(t_cmd *cmd, t_tokenlist *token);
int				do_redir_output(t_cmd *cmd, t_tokenlist *token);

// ---- //
// EXEC //
// ---- //
int				exec_cmdline(t_msh *msh);
int				has_input_redir(t_cmd *cmd);
int				has_output_redir(t_cmd *cmd);
int				redirect_io(t_cmdline *cmdline, t_cmd *cmd);
int				redirect_builtin_io(t_cmdline *cmdline, t_cmd *cmd,
					int io_dup[2]);
int				unredirect_builtin_io(int io_dup[2]);
void			child_process(t_msh *msh, t_cmd *cmd);
int				wait_pipeline(pid_t last_pid, int n_children);
void			kill_children(t_cmdline *cmdline, int i);
int				redir_dup(t_cmdline *cmdline, t_cmd *cmd, int fd);
int				get_exit_status(int stat_loc);

// -------- //
// BUILTINS //
// -------- //
int				builtin_pwd(t_msh *msh, char **args);
int				builtin_cd(t_msh *msh, char **args);
int				builtin_exit(t_msh *msh, char **args);
int				builtin_echo(t_msh *msh, char **args);
int				builtin_export(t_msh *msh, char **args);
int				builtin_unset(t_msh *msh, char **args);
int				builtin_env(t_msh *msh, char **args);

#endif
