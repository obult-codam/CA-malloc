/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/12 21:16:23 by obult         #+#    #+#                 */
/*   Updated: 2021/04/07 17:49:08 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int			ft_strclen(const char *str, char c)
{
	if (*str && *str != c)
		return (1 + ft_strclen(str + 1, c));
	return (0);
}

int					count_words(char *s, char c, int wrds)
{
	while (*s)
	{
		if (*str != c && (str[1] == c || !str[1]))
			wrds++;
		str++;
	}
	return (wrds);
}

static void			ft_freeer(char **pnt, int i)
{
	while (i)
	{
		i--;
		free(pnt[i]);
	}
	free(pnt);
}

static char			**arrange(char **pnt, const char *s, char c)
{
	int		i;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			pnt[i] = ft_substr(s, 0, ft_strclen(s, c));
			if (!(pnt[i]))
			{
				ft_freeer(pnt, i);
				return (0);
			}
			i++;
		}
		while (*s && *s != c)
			s++;
	}
	return (pnt);
}

char				**ft_split(char const *s, char c)
{
	char	**pnts;
	int		words;

	if (!s)
		return (0);
	words = count_words((char *)s, c, 0);
	pnts = malloc(sizeof(char *) * (words + 1));
	if (!pnts)
		return (0);
	pnts[words] = 0;
	return (arrange(pnts, s, c));
}
