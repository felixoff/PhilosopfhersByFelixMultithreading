/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:26:15 by sjennett          #+#    #+#             */
/*   Updated: 2021/04/20 23:09:02 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	timer(void)
{
	int				i;
	struct timeval	time;

	i = 0;
	gettimeofday(&time, NULL);
	i = (time.tv_sec * 1000 + time.tv_usec / 1000);
	return (i);
}

int	count_eat(void *args)
{
	t_philo	*philo;

	philo = args;
	if (philo->count_eat >= philo->init_data->count_eat)
		philo->init_data->add_fat_philo++;
	if (philo->init_data->add_fat_philo == philo->init_data->kol)
	{
		show(philo, 6);
		return (1);
	}
	return (0);
}

void	*dead_philo(void *args)
{
	t_philo	*philo;

	philo = args;
	while (!philo->init_data->finish)
	{
		sem_wait(philo->sem2);
		if (!philo->init_data->finish && \
		timer() - philo->time > philo->init_data->die)
		{
			show(philo, 5);
			sem_post(philo->sem2);
			return (NULL);
		}
		sem_post(philo->sem2);
		usleep(100);
	}
	return (NULL);
}

char	*ft_name(char *str, int j)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = malloc(sizeof(char) * 100);
	while (i < ft_strlen(str))
	{
		tmp[i] = str[i];
		i++;
	}
	while (j > 0)
	{
		tmp[i] = j % 10;
		j = j / 10;
	}
	tmp[i] = '\0';
	return (tmp);
}
