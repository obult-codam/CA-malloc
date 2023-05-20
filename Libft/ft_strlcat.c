/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:21:52 by obult         #+#    #+#                 */
/*   Updated: 2020/11/20 11:39:48 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_strlcat(char *dst, const char *src, size_t dst_size)
{
	size_t		dst_len;
	size_t		src_len;
	size_t		extra_size;
	size_t		i;

	i = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	extra_size = dst_size - dst_len;
	if (dst_len >= dst_size)
		return (dst_size + src_len);
	while (src[i] && i < extra_size - 1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = 0;
	return (dst_len + src_len);
}
