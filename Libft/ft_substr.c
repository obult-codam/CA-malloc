/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_substr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:20:13 by obult         #+#    #+#                 */
/*   Updated: 2020/11/21 00:46:43 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*substart;
	char	*result;

	if (!s)
		return (0);
	i = 0;
	j = 0;
	substart = (char *)s + start;
	if (start >= ft_strlen(s))
		substart = (char *)s + ft_strlen(s);
	while (substart[i] && i < len)
		i++;
	result = malloc(i + 1);
	if (!result)
		return (0);
	result[i] = 0;
	while (j < i)
	{
		result[j] = substart[j];
		j++;
	}
	return (result);
}
