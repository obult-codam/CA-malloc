/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:21:32 by obult         #+#    #+#                 */
/*   Updated: 2020/12/08 01:42:47 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memset(void *s, int c, size_t n)
{
	while (n)
	{
		n--;
		((unsigned char *)s)[n] = (unsigned char)c;
	}
	return (s);
}
