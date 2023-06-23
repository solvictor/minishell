/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:01:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/23 22:31:12 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <errno.h>
# include <signal.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

// SETTINGS
//# define MSH_PROMPT			"\[\e[38;2;255;0;255mminishell>\e[0m\] " // lame... make it better
# define MSH_PROMPT			"minishell> " // lame... make it better
# define RNG_MAX			INT_MAX
# define RNG_BIT_ROTATIONS	13
# define RNG_ZERO_FIX_SEED	694201337

// ERROR MESSAGES
# define MSH_ERROR		"\e[31;7;1m[MINISHELL ERROR]\e[0m "
# define ME_AC			"Minishell takes no arguments\n"
# define ME_ENV			"Failed to create minishell environment\n"
# define ME_RNG			"Failed to random number generator\n"
# define ME_TOKENIZE	"Failed to tokenize\n"
# define ME_PARSE		"Failed to parse\n"
# define ME_SPLIT_ARGS	"Failed to split command arguments\n"
# define ME_EXEC_CMD	"Failed to exec command\n"
# define ME_BAD_FORMAT	"Bad format command\n"
# define ME_SIGNALS		"Failed to bind signals\n"
# define ME_LOOP		"Minishell input loop exited with -1\n"
# define ME_USAGE		"Incorrect usage\n"

typedef enum e_tokentype	t_tokentype;
typedef enum e_nodetype		t_nodetype;
typedef struct s_tokenlist	t_tokenlist;
typedef struct s_rng		t_rng;
typedef struct s_cmd		t_cmd;
typedef struct s_pipeline	t_pipeline;
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
	INPUTFILE,
	OUTPUTFILE_TRUNC,
	OUTPUTFILE_APPEND,
	PIPE,
	//NEWLINE, // added this as a comment because of the "\newline" syntax error context
	UNKNOWN
};

struct s_tokenlist
{
	char		*data;
	t_tokentype	type;
	int			concat_next;
	t_tokenlist	*next;
};
struct s_cmd
{
	char		*path;
	char		**args;
	char		**env;
	t_tokenlist	*start_token;
//	int			empty; // flag for pipelines with empty commands but rediretions so redirections work
};
struct s_pipeline
{
	int		cmds_n;
	t_cmd	*cmds;
//	int		ret;
};
struct	s_env
{
	t_env	*prev;
	char	*var;
	t_env	*next;
};
struct s_rng
{
	int				fd_urandom;
	unsigned int	rand;
	unsigned int	mult;
	unsigned int	inc;
};
struct s_msh
{
	t_env	*env;
	char	*input;
	int		exit;
	int		ret;
	t_rng	rng;
};

// minishell.c
int				msh_loop(t_msh *msh);
int				process_input(t_msh *msh);

// ----- //
// SETUP //
// ----- //
int				msh_setup(t_msh	*msh, int ac, char **envp);
unsigned int	get_randint(t_rng *rng);
void			handler_sigint(int sig);

// ----- //
// UTILS //
// ----- //
void			msh_terminate(t_msh *msh);
void			clear_strarr(char **arr);
unsigned int	rng_bit_rot(unsigned int num);
// Env
t_env			*env_new(char *var);
char			**env_to_arr(t_env *env);
void			destroy_env_list(t_env **env);
char			*get_env_val(t_env *env, char *key);
t_env			*get_env(t_env *env, char *key, int len_key);
// Tokens
t_tokenlist		*token_add_front(t_tokenlist **begin, char *data);
void			destroy_tokenlist(t_tokenlist **begin);
int				is_redir_token(t_tokenlist *token);
int				is_str_token(t_tokenlist *token);

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
void			clean_redir_tokens(t_tokenlist **tokens);

// ------ //
// PARSER //
// ------ //
int				parse(t_pipeline *pip, t_tokenlist *tokens);
int				count_pipeline_commands(t_tokenlist *tokens);
void			set_cmds_start_token(t_pipeline *pip, t_tokenlist *tokens);

// -------- //
// BUILTINS //
// -------- //
t_builtin		get_builtin(t_cmd *cmd);
int				builtin_pwd(t_msh *msh, char **args);
int				builtin_cd(t_msh *msh, char **args);
int				builtin_exit(t_msh *msh, char **args);
int				builtin_echo(t_msh *msh, char **args);
int				builtin_export(t_msh *msh, char **args);
int				builtin_unset(t_msh *msh, char **args);
int				builtin_env(t_msh *msh, char **args);
// -------- //
//          //
// -------- //

// TEST FUNCTIONS ONLY // REMOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOVE LATER
//void	display_token_type(t_tokenlist *token);
//void	display_tokens(t_tokenlist *begin);
//void	test_tokenizer(t_msh *msh);
//void	test_command(t_msh *msh);
//void	display_pipeline(t_pipeline *pip);
//char			**get_paths(t_env *env);
//int				find_cmd(char **paths, t_cmd *cmd);
//void			test_pathfinding(t_msh *msh);


char			*make_expansion(t_env *env, char *str);

#endif
