#ifndef SYSCALL_MOCK_H
#define SYSCALL_MOCK_H

#include <stdbool.h>
#include <setjmp.h>

/*
 * Which malloc call should fail when err_syscall == MALLOC.
 * Override at compile time: -DMALLOC_FAIL_COUNT=2
 */
#ifndef MALLOC_FAIL_COUNT
# define MALLOC_FAIL_COUNT 1
#endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

enum e_error_syscall
{
	NOTHING = 0,
	MALLOC,
	READ,
	OPEN,
};

extern int					_wrap_errno_;
extern enum e_error_syscall	err_syscall;

bool	is_memory_remain(void);

extern jmp_buf	g_test_jump_env;
extern int		g_last_exit_status;

#endif

