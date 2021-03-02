/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:26:15 by sjennett          #+#    #+#             */
/*   Updated: 2021/03/02 22:36:14 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		init_philo(t_init_data *init_data)
{
	int i;

	i = -1;
	if (!(init_data->philo = malloc(sizeof(t_init_data) * init_data->kol)))
		return (2);
	while (++i < init_data->kol)
	{
		init_data->philo[i].index = i;
		init_data->philo[i].count_eat = 0;
		init_data->philo[i].is_death = 1;
		init_data->philo[i].init_data = init_data;
	}
	sem_unlink("sem");
	sem_unlink("end");
	init_data->sem = sem_open("sem", O_CREAT, 0644, 0);
	init_data->fin = sem_open("end", O_CREAT, 0644, 0);
	sem_unlink("fork");
	sem_unlink("vivod");
	init_data->fork = sem_open("fork", O_CREAT, 0644, init_data->kol);
	init_data->vivod = sem_open("vivod", O_CREAT, 0644, 1);
	return (0);
}

int		timer(void)
{
	int				i;
	struct timeval	time;

	i = 0;
	gettimeofday(&time, NULL);
	i = (time.tv_sec * 1000 + time.tv_usec / 1000);
	return (i);
}

void	*dead_philo(void *args)
{
	t_philo *philo;

	philo = (t_philo*)args;
	while (philo->is_death)
	{
		if (philo->init_data->finish == 1 &&
		timer() - philo->time > philo->init_data->die)
		{
			philo->is_death = 0;
			show(philo, 5);
			sem_post(philo->init_data->fin);
		}
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
