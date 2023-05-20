/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: obult <obult@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:19:32 by obult         #+#    #+#                 */
/*   Updated: 2020/11/10 13:55:52 by obult         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static void	ft_putdig_fd(int dig, int fd)
{
	dig = dig + '0';
	write(fd, &dig, 1);
}

void		ft_putnbr_fd(int n, int fd)
{
	if (n < 10 && n >= 0)
		ft_putdig_fd(n, fd);
	else if (n > 0)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putdig_fd(n % 10, fd);
	}
	else if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		write(fd, "-", 1);
		ft_putnbr_fd(n * -1, fd);
	}
}
