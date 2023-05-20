/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstclear.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/14 15:10:25 by obult         #+#    #+#                 */
/*   Updated: 2020/11/15 17:19:34 by obult         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Deletes and frees the given element and every
**	successor of that element, using the function ’del’
**	and free(3).
**	Finally, the pointer to the list must be set to
**	NULL.
*/

void		ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list		*temp;

	temp = *lst;
	if (temp->next)
	{
		ft_lstclear(&(temp->next), del);
	}
	del(temp->content);
	free(temp);
	*lst = 0;
}
