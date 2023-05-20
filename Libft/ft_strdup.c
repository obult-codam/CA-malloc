/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:21:45 by obult         #+#    #+#                 */
/*   Updated: 2020/12/08 01:46:49 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		len;
	char	*temp;

	len = ft_strlen(s);
	temp = malloc((len + 1) * sizeof(char));
	if (!temp)
		return (temp);
	temp[len] = 0;
	return (ft_memcpy(temp, s, len));
}
