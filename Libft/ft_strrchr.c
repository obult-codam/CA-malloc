/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   strrchr.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:22:29 by obult         #+#    #+#                 */
/*   Updated: 2020/11/04 18:33:33 by obult         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	t;

	i = 0;
	t = -1;
	while (s[i])
	{
		if (s[i] == c)
			t = i;
		i++;
	}
	if (c == 0)
		return ((char *)s + i);
	if (t < 0)
		return (0);
	return ((char *)s + t);
}
