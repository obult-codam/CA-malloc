/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/14 15:11:38 by obult         #+#    #+#                 */
/*   Updated: 2020/11/24 20:12:48 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Iterates the list ’lst’ and applies the function
**	’f’ to the content of each element. Creates a new
**	list resulting of the successive applications of
**	the function ’f’. The ’del’ function is used to
**	delete the content of an element if needed.
*/

t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list		*new;

	if (lst)
	{
		new = ft_lstnew(f(lst->content));
		if (!new)
			return (0);
		new->next = ft_lstmap(lst->next, f, del);
		if (!new->next)
		{
			del(new);
			return (0);
		}
		if (new->next == (t_list *)-1)
			new->next = 0;
		return (new);
	}
	return ((t_list *)-1);
}
