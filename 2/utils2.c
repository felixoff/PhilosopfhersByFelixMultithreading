/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:26:15 by sjennett          #+#    #+#             */
/*   Updated: 2021/03/02 18:47:08 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		timer(void)
{
	int				i;
	struct timeval	time;

	i = 0;
	gettimeofday(&time, NULL);
	i = (time.tv_sec * 1000 + time.tv_usec / 1000);
	return (i);
}

int		count_eat(void *args)
{
	t_philo *philo;

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
	t_philo *philo;

	philo = args;
	while (!philo->init_data->finish)
	{
		pthread_mutex_lock(&philo->mutex);
		if (!philo->init_data->finish &&
		timer() - philo->time > philo->init_data->die)
		{
			show(philo, 5);
			pthread_mutex_unlock(&philo->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->mutex);
		usleep(100);
	}
	return (NULL);
}
