/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:01:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/19 10:11:29 by nlegrand         ###   ########.fr       */
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
# define MSH_PROMPT			"\e[38;2;255;0;255mminishell>\e[0m " // lame... make it better
# define RNG_MODULUS 		10000000
# define RNG_BIT_ROTATIONS	13
# define RNG_ZERO_FIX_SEED	694201337

// ERROR MESSAGES
# define MSH_ERROR		"\e[31;7;1m[MINISHELL ERROR]\e[0m "
# define ME_AC			"Minishell takes no arguments\n"
# define ME_ENV			"Failed to create minishell environment\n"
# define ME_RNG			"Failed to random number generator\n"
# define ME_TOKENIZE	"Failed to tokenize\n"
# define ME_PARSE_CMD	"Failed to parse command\n"
# define ME_SPLIT_ARGS	"Failed to split command arguments\n"
# define ME_EXEC_CMD	"Failed to exec command\n"
# define ME_BAD_FORMAT	"Bad format command\n"
# define ME_SIGNALS		"Failed to bind signals\n"
# define ME_LOOP		"Minishell input loop exited with -1\n"
# define ME_USAGE		"Incorrect usage\n"

typedef struct s_parse_stack	t_parse_stack; // remoooooooooooooooooooooooooove later
struct s_parse_stack
{
	int				index; // probably change to char* later (if i end up using strdup)
	t_parse_stack	*next;
};

typedef enum e_tokentype	t_tokentype;
typedef enum e_nodetype		t_nodetype;
typedef struct s_tokenlist	t_tokenlist;
typedef struct s_rng		t_rng;
typedef struct s_cmd		t_cmd;
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
	OUTPUT_TRUNC,
	OUTPUT_APPEND,
	//L_ARROW_SINGLE,
	//R_ARROW_SINGLE,
	//L_ARROW_DOUBLE,
	//R_ARROW_DOUBLE,
	PIPE,
	L_BRACKET,
	R_BRACKET,
	LOGIC_OR,
	LOGIC_AND,
	UNKNOWN
};
enum e_nodetype // DONT FORGET TO LASKDFJLAKSDJFLAKSDJFLAKFJKLAJFASKDLJFALSFJAKLSJFAKLSD
{
	COMMAND,
	PIPELINE,
	CHILD_NODE
};

//struct s_astnode
//{
//	void		*left;
//	void		*right;
//	t_nodetype	left_type;
//	t_nodetype	right_type;
//	t_tokentype	operand;
//}

struct s_rng
{
	int		fd_urandom;
	size_t	curr_rand;
	size_t	multiplier;
	size_t	increment;
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

};
struct	s_env
{
	t_env	*prev;
	char	*var;
	t_env	*next;
};
struct s_msh
{
	//char		**envp;
	t_env		*env;
	char		*input;
	//t_tokenlist	*tokens; // restructure later so that it's a local variable
	//t_cmd	cmd; // will have to replace with pipeline or some other new big struct that will contain the whole parsed thing
	int		exit;
	int		ret;
	t_rng	rng;
};

// minishell.c
int	msh_loop(t_msh *msh);

// ----- //
// SETUP //
// ----- //
int	msh_setup(t_msh	*msh, int ac, char **envp);
int	make_env(t_msh *msh, char **envp);
int	init_rng(t_rng *rng);

// ----- //
// UTILS //
// ----- //
void	msh_terminate(t_msh *msh);
void	clear_strarr(char **arr);
size_t	rng_bit_rot(size_t num);
t_env	*env_new(char *var);
char	**env_to_arr(t_env *env);
void	destroy_env_list(t_env **env);
char	*get_env_val(t_env *env, char *key);
t_env	*get_env(t_env *env, char *key);

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
// export
int	builtin_export(t_msh *msh, char **args);
// unset
int	builtin_unset(t_msh *msh, char **args);
// -------- //
//          //
// -------- //

// -------- //
//  SIGNALS //
// -------- //
int	setup_signals(void);

// tests folder // remooooooooooooooooooooooooooooooooooooooooooooooooooooooooove later
//void	test_parentheses(const char *line, int left);
//void	test_parsing(t_msh *msh);
void	test_command(t_msh *msh);
void	test_rng(t_msh *msh);
//void	test_quotes(t_msh *msh, const char *line);
void	test_tokenizer(t_msh *msh);
void	destroy_tokenlist(t_tokenlist **begin);
int		tokenize(t_msh *msh, const char *input, t_tokenlist **tokens);

char	**get_paths(t_env *env);
int	find_cmd(char **paths, t_cmd *cmd);
void	test_pathfinding(t_msh *msh);

void	display_tokens(t_tokenlist *begin);

char	*make_expansion(t_env *env, char *str);

#endif
