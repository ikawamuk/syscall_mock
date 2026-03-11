/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_mock.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikawamuk <ikawamuk@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 01:37:49 by ikawamuk          #+#    #+#             */
/*   Updated: 2026/03/11 19:14:54 by ikawamuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSCALL_MOCK_H
# define SYSCALL_MOCK_H

# include <stdbool.h>
# include <setjmp.h>

# define MALLOC_FAIL_COUNT 3

enum e_error_syscall {
	NOTHING,
	OPEN,
	READ,
	WRITE,
	MALLOC
};

extern int		_wrap_errno_;
extern enum		e_error_syscall err_syscall;

extern jmp_buf	g_test_jump_env;
extern int		g_last_exit_status;

bool	is_memory_remain(void);

#endif