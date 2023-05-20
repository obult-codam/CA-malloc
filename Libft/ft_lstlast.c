/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstlast.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/14 15:07:48 by obult         #+#    #+#                 */
/*   Updated: 2020/11/20 11:14:11 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Returns the last element of the list.
*/

t_list		*ft_lstlast(t_list *lst)
{
	if (!lst || !(lst->next))
		return (lst);
	return (ft_lstlast(lst->next));
}
