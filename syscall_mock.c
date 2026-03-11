/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_mock.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikawamuk <ikawamuk@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 19:11:39 by ikawamuk          #+#    #+#             */
/*   Updated: 2026/03/11 19:17:55 by ikawamuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syscall_mock.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

void	*__real_malloc(size_t size);
void	__real_free(void *ptr);
ssize_t	__real_read(int fd, void *buf, size_t count);
int		__real_open(const char *pathname, int flags);
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstlast(t_list *lst);

int						_wrap_errno_ = 0;
enum e_error_syscall	err_syscall = NOTHING;
t_list					dummy_head = (t_list){0};

void	*__wrap_malloc(size_t size)
{
	static int	called_count = 0;
	void		*p;
	t_list		*new_node;

	if (err_syscall == MALLOC)
	{
		called_count++;
		if (called_count == MALLOC_FAIL_COUNT)
		{
			errno = _wrap_errno_;
			return (NULL);
		}
	}
	p = __real_malloc(size);
	if (!p)
		return (NULL);
	new_node = __real_malloc(sizeof(*new_node));
	if (!new_node)
	{
		__real_free(p);
		return (NULL);
	}
	new_node->content = p;
	new_node->next = NULL;
	ft_lstadd_back(&dummy_head.next, new_node);
	return (p);
}

void	__wrap_free(void *ptr)
{
	t_list	*cur;
	t_list	*prev;

	cur = dummy_head.next;
	prev = &dummy_head;
	while (cur)
	{
		if (cur->content == ptr)
		{
			prev->next = cur->next;
			__real_free(cur->content);
			__real_free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
	printf("ERROR: invalid free.\n");
	abort();
}

bool	is_memory_remain(void)
{
	return (dummy_head.next != NULL);
}

ssize_t	__wrap_read(int fd, void *buf, size_t count)
{
	if (err_syscall == READ)
	{
		errno = _wrap_errno_;
		return (-1);
	}
	return (__real_read(fd, buf, count));
}

int	__wrap_open(const char *pathname, int flags)
{
	if (err_syscall == OPEN)
	{
		errno = _wrap_errno_;
		return (-1);
	}
	return (__real_open(pathname, flags));
}

#include <setjmp.h>

jmp_buf	g_test_jump_env;
int		g_last_exit_status;

void	__wrap_exit(int status)
{
	g_last_exit_status = status;
	longjmp(g_test_jump_env, 1);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (*lst == NULL)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;
}

t_list	*ft_lstlast(t_list *lst)
{
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}