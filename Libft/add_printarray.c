/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   add_printarray.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: oswin <oswin@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/28 23:23:49 by oswin         #+#    #+#                 */
/*   Updated: 2020/11/28 23:29:56 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	add_printarray_fd(char **arr, int fd)
{
	int		i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		ft_putendl(arr[i], fd);
		i++;
	}
}
