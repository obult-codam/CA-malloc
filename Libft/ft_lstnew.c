/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstnew.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/14 15:04:06 by obult         #+#    #+#                 */
/*   Updated: 2020/11/19 14:14:11 by obult         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/*
**	Allocates (with malloc(3)) and returns a new
**	element. The variable ’content’ is initialized
**	with the value of the parameter ’content’. The
**	variable ’next’ is initialized to NULL.
*/

t_list		*ft_lstnew(void *content)
{
	t_list	*n_list;

	n_list = malloc(sizeof(t_list));
	if (!n_list)
		return (0);
	n_list->content = content;
	n_list->next = 0;
	return (n_list);
}
