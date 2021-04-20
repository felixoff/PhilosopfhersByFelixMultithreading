/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:51:25 by sjennett          #+#    #+#             */
/*   Updated: 2021/04/20 23:07:03 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_putchar_fd(int fd, char c)
{
	if (!fd)
		return ;
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int fd, int number)
{
	long	nb;

	nb = number;
	if (nb < 0)
	{
		ft_putchar_fd(fd, '-');
		nb = nb * -1;
	}
	if (nb > 9)
	{
		ft_putnbr_fd(fd, nb / 10);
		ft_putchar_fd(fd, (nb % 10) + 48);
	}
	if (nb <= 9)
		ft_putchar_fd(fd, (nb % 10) + 48);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	atoi2(char *str)
{
	int	i;
	int	minus;
	int	result;

	i = 0;
	minus = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minus = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
		result = result * 10 + (str[i++] - '0');
	return (result * minus);
}

int	ft_atoi(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		i++;
	if (str[i] == '\0')
		return (atoi2(str));
	return (-1);
}
