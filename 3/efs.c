/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   efs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:46:44 by sjennett          #+#    #+#             */
/*   Updated: 2021/03/02 22:35:13 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		start_process(t_philo *philo)
{
	pthread_t	thread;
	int			count_eat;

	count_eat = 0;
	philo->time = timer();
	if (pthread_create(&thread, NULL, &dead_philo, philo))
		return (1);
	pthread_detach(thread);
	while (1)
	{
		take_fork_on(philo);
		eat(philo);
		count_eat++;
		if (count_eat == philo->init_data->count_eat)
			sem_post(philo->init_data->sem);
		take_fork_off(philo);
		show(philo, 4);
	}
	return (0);
}

void	take_fork_on(t_philo *philo)
{
	sem_wait(philo->init_data->fork);
	sem_wait(philo->init_data->fork);
	show(philo, 1);
	show(philo, 1);
}

void	take_fork_off(t_philo *philo)
{
	sem_post(philo->init_data->fork);
	sem_post(philo->init_data->fork);
}

void	eat(t_philo *philo)
{
	show(philo, 2);
	philo->time = timer();
	usleep(philo->init_data->eat * 1000);
	philo->count_eat++;
}

void	ft_sleep(t_philo *philo)
{
	show(philo, 3);
	usleep(philo->init_data->sleep * 1000);
}
