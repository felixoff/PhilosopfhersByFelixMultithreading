/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:50:26 by sjennett          #+#    #+#             */
/*   Updated: 2021/03/02 16:50:27 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		init_sem(t_philo *philo, int i)
{
	philo->sem1 = ft_name("sem", i);
	sem_unlink(philo->sem1);
	philo->sem2 = sem_open(philo->sem1, O_CREAT, 0644, 1);
	return (0);
}

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
		init_data->philo[i].init_data = init_data;
		init_sem(&init_data->philo[i], i);
	}
	sem_unlink("fork");
	sem_unlink("sem");
	init_data->fork = sem_open("fork", O_CREAT, 0644, init_data->kol);
	init_data->sem = sem_open("sem", O_CREAT, 0644, 1);
	return (0);
}

int		init_input(int argc, char **argv, t_init_data *init_data)
{
	if (argc < 5 || argc > 6)
	{
		write(2, "Неверное количество аргументов",
		ft_strlen("Неверное количество аргументов"));
		return (1);
	}
	if ((init_data->kol = ft_atoi(argv[1])) < 2)
		write(2, "Неверное количество философов",
		ft_strlen("Неверное количество философов"));
	else if ((init_data->die = ft_atoi(argv[2])) < 60)
		write(2, "Неверное время смерти", ft_strlen("Неверное время смерти"));
	else if ((init_data->eat = ft_atoi(argv[3])) < 60)
		write(2, "Неверное время еды", ft_strlen("Неверное время еды"));
	else if ((init_data->sleep = ft_atoi(argv[4])) < 60)
		write(2, "Неверное время сна", ft_strlen("Неверное время сна"));
	else if (argc == 6 && (init_data->count_eat = ft_atoi(argv[5])) < 0)
		write(2, "Неверное количество блюд",
		ft_strlen("Неверное количество блюд"));
	else
		return (init_philo(init_data));
	return (1);
}

int		main(int argc, char **argv)
{
	t_init_data	init_data;
	int			i;

	memset(&init_data, 0, sizeof(t_init_data));
	if ((i = init_input(argc, argv, &init_data)))
		return (error(&init_data, i));
	if ((i = init_thread(&init_data)))
		return (error(&init_data, 3));
	clean_all(&init_data);
	return (0);
}

int		init_thread(t_init_data *init_data)
{
	int			i;
	pthread_t	thread;

	init_data->time = timer();
	i = 0;
	while (i < init_data->kol)
	{
		init_data->philo[i].time = timer();
		if (pthread_create(&thread, NULL,
		start_process, (void*)&init_data->philo[i]))
			return (1);
		pthread_detach(thread);
		if (pthread_create(&init_data->philo[i].thread, NULL,
		&dead_philo, (void*)&init_data->philo[i]))
			return (1);
		usleep(1000);
		i++;
	}
	i = 0;
	while (i < init_data->kol)
		pthread_join(init_data->philo[i++].thread, NULL);
	return (0);
}
