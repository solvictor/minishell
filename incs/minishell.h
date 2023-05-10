/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:01:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/10 17:42:39 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <errno.h>
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

# define MSH_PROMPT		"\e[38;2;255;0;255mminishell>\e[0m " // lame... make it better

// ERROR MESSAGES
# define MSH_ERROR		"\e[31;7;1m[MINISHELL ERROR]\e[0m "
# define ME_AC			"Minishell takes no arguments\n"
# define ME_ENV			"Failed to create minishell environment\n"
# define ME_TOKEN_CMD	"Failed to tokenize command\n"
# define ME_PARSE_CMD	"Failed to parse command\n"
# define ME_SPLIT_ARGS	"Failed to split command arguments\n"
# define ME_EXEC_CMD	"Failed to exec command\n"
# define ME_BAD_FORMAT	"Bad format command\n"
# define ME_SIGNALS		"Failed to bind signals\n"
# define ME_LOOP		"Minishell input loop exited with -1, \
						don't forget to free stuff\n"

typedef struct s_parse_stack	t_parse_stack; // remoooooooooooooooooooooooooove later
struct s_parse_stack
{
	int				index; // probably change to char* later (if i end up using strdup)
	t_parse_stack	*next;
};

typedef enum e_tokentype	t_tokentype;
typedef struct s_tokenlist	t_tokenlist;
typedef struct s_cmd		t_cmd;
typedef struct s_env		t_env;
typedef struct s_msh		t_msh;
typedef int					(*t_builtin)(t_msh *, char **);

enum e_tokentype
{
	SINGLE_QUOTED_STR,
	DOUBLE_QUOTED_STR,
	UNQUOTED_STR,
	PIPE,
	LOGIC_OR,
	LOGIC_AND,
	L_REDIR_TRUNC,
	R_REDIR_TRUNC,
	L_REDIR_APPEND,
	R_REDIR_APPEND,
	L_BRACKET,
	R_BRACKET,
	UNKNOWN
};
struct s_tokenlist
{
	char		*data;
	t_tokentype	type;
	t_tokenlist	*next;
};
struct s_cmd
{
	char	*path;
	char	**args;
};
struct	s_env
{
	char	*var;
	t_env	*next;
};
struct s_msh
{
	//char		**envp;
	t_env		*env;
	char		*input;
	t_tokenlist	*tokens; // restructure later so that it's a local variable
	//t_cmd	cmd; // will have to replace with pipeline or some other new big struct that will contain the whole parsed thing
	int		exit;
	int		ret;
};

// minishell.c
int	msh_loop(t_msh *msh);

// setup.c
int	msh_setup(t_msh	*msh, int ac, char **envp);
// setup_env.c
int	make_env(t_msh *msh, char **envp);

// utils.c
void	msh_terminate(t_msh *msh);
void	clear_strarr(char **arr);
// utils_env.c
t_env	*env_new(char *var);
char	**env_to_arr(t_env *env);
void	destroy_env_list(t_env **env);
char	*get_val(t_env *env, char *key);

// -------- //
// BUILTINS //
// -------- //
t_builtin	get_builtin(t_cmd *cmd);
// pwd
int	builtin_pwd(t_msh *msh, char **args);
// cd
int	builtin_cd(t_msh *msh, char **args);
// exit
int	builtin_exit(t_msh *msh, char **args);
// echo
int	builtin_echo(t_msh *msh, char **args);
// -------- //
//          //
// -------- //

// -------- //
//  SIGNALS //
// -------- //
int	setup_signals(void);

// tests folder // remooooooooooooooooooooooooooooooooooooooooooooooooooooooooove later
//void	test_parentheses(const char *line, int left);
//void	test_parsing(const char *line);
//void	test_quotes(t_msh *msh, const char *line);
void	test_tokenizer(t_msh *msh);
void	destroy_tokenlist(t_tokenlist **begin);
int		tokenize(t_msh *msh, const char *input, t_tokenlist **tokens);
void	test_pathfinding(t_msh *msh);

#endif
