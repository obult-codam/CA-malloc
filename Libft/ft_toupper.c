/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_toupper.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:22:41 by obult         #+#    #+#                 */
/*   Updated: 2020/11/20 11:42:24 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int		ft_toupper(int c)
{
	if (c <= 'z' && c >= 'a')
		return (c + 'A' - 'a');
	return (c);
}