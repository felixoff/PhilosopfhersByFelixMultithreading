/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   efs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 18:45:21 by sjennett          #+#    #+#             */
/*   Updated: 2021/03/02 18:45:23 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_process(void *data)
{
	t_philo	*philo;

	philo = data;
	philo->time = timer();
	while (!philo->init_data->finish)
	{
		take_fork_on(philo);
		if (philo->init_data->finish)
			break ;
		eat(philo);
		if (philo->init_data->finish)
			break ;
		take_fork_off(philo);
		if (philo->init_data->count_eat > 0 && count_eat(philo))
			break ;
		if (philo->init_data->finish)
			break ;
		ft_sleep(philo);
		show(philo, 4);
	}
	pthread_mutex_unlock(&philo->init_data->fork[philo->lf]);
	pthread_mutex_unlock(&philo->init_data->fork[philo->rf]);
	pthread_mutex_unlock(&philo->init_data->mutex);
	return (NULL);
}

void	take_fork_on(t_philo *philo)
{
	pthread_mutex_lock(&philo->init_data->fork[philo->lf]);
	show(philo, 1);
	pthread_mutex_lock(&philo->init_data->fork[philo->rf]);
	show(philo, 1);
}

void	take_fork_off(t_philo *philo)
{
	pthread_mutex_unlock(&philo->init_data->fork[philo->lf]);
	pthread_mutex_unlock(&philo->init_data->fork[philo->rf]);
}

void	eat(t_philo *philo)
{
	show(philo, 2);
	philo->time = timer();
	pthread_mutex_lock(&philo->mutex);
	usleep(philo->init_data->eat * 1000);
	philo->count_eat++;
	pthread_mutex_unlock(&philo->mutex);
}

void	ft_sleep(t_philo *philo)
{
	show(philo, 3);
	usleep(philo->init_data->sleep * 1000);
}
