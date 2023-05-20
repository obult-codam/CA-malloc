/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/19 10:40:38 by obult         #+#    #+#                 */
/*   Updated: 2020/11/19 10:53:20 by obult         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strnstr(const char *hay, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!*needle)
		return ((char *)hay);
	while (hay[i] && i < len)
	{
		j = 0;
		while (needle[j] && needle[j] == hay[i + j] && i + j < len)
			j++;
		if (!needle[j])
			return ((char *)&hay[i]);
		i++;
	}
	return (0);
}
