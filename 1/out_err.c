/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:55:07 by sjennett          #+#    #+#             */
/*   Updated: 2021/04/20 23:51:25 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	clean_all(t_init_data *init_data)
{
	int	i;

	i = 0;
	if (init_data)
	{
		if (init_data->philo)
		{
			while (i < init_data->kol)
			{
				sem_close(init_data->philo[i].sem2);
				sem_unlink(init_data->philo[i].sem1);
				free(init_data->philo[i].sem1);
				init_data->philo[i].sem1 = NULL;
				i++;
			}
			free(init_data->philo);
		}
		sem_close(init_data->fork);
		sem_close(init_data->sem);
		sem_unlink("fork");
		sem_unlink("sem");
	}
	return (1);
}

int	error(t_init_data *init_data, int i)
{
	if (i == 2)
	{
		write(2, "Ошибка маллока", ft_strlen("Ошибка маллока"));
		clean_all(init_data);
	}
	else if (i == 3)
	{
		write(2, "Ошибка семафора", ft_strlen("Ошибка семафора"));
		clean_all(init_data);
	}
	return (i);
}

char	*output(int i)
{
	if (i == 1)
		return (" : беру вилку\n");
	else if (i == 2)
		return (" : кушаю\n");
	else if (i == 3)
		return (" : сплю\n");
	else if (i == 4)
		return (" : думаю\n");
	else if (i == 5)
		return (" : умер RIP\n");
	return (" : все наелись\n");
}

void	show(t_philo *philo, int i)
{
	char	*tmp;

	sem_wait(philo->init_data->sem);
	if (!philo->init_data->finish)
	{
		tmp = output(i);
		ft_putnbr_fd(1, timer() - philo->init_data->time);
		write(1, "\t", 1);
		if (i != 6)
		{
			write(1, "\tPhilosoph number", ft_strlen(" Philosoph number"));
			ft_putnbr_fd(1, philo->index + 1);
		}
		write(1, tmp, ft_strlen(tmp));
		tmp = NULL;
		if (i == 5 || i == 6)
		{
			philo->init_data->finish = 1;
			return ;
		}
	}
	sem_post(philo->init_data->sem);
}

int	init_philo(t_init_data *init_data)
{
	int	i;

	i = -1;
	init_data->philo = malloc(sizeof(t_init_data) * init_data->kol);
	if (!(init_data->philo))
		return (2);
	while (++i < init_data->kol)
	{
		init_data->philo[i].index = i;
		init_data->philo[i].count_eat = 0;
		init_data->philo[i].init_data = init_data;
		init_sem(&init_data->philo[i], i);
	}
	sem_unlink("fork");
	sem_unlink("sem");
	init_data->fork = sem_open("fork", O_CREAT, 0644, init_data->kol);
	init_data->sem = sem_open("sem", O_CREAT, 0644, 1);
	return (0);
}
