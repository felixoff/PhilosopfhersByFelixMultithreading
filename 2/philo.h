/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:16:08 by sjennett          #+#    #+#             */
/*   Updated: 2021/04/20 23:15:49 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <fcntl.h>

typedef struct s_philo
{
	int					index;
	int					count_eat;
	int					time;
	int					lf;
	int					rf;
	char				*sem1;
	pthread_t			thread;
	pthread_mutex_t		mutex;
	struct s_init_data	*init_data;
}					t_philo;

typedef struct s_init_data
{
	int				kol;
	int				sleep;
	int				eat;
	int				die;
	int				count_eat;
	int				time;
	int				finish;
	int				add_fat_philo;
	t_philo			*philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	mutex;
}					t_init_data;

int					clean_all(t_init_data *init_data);
int					error(t_init_data *init_data, int i);
void				ft_putchar_fd(int fd, char c);
void				ft_putnbr_fd(int fd, int number);
int					ft_strlen(char *str);
int					atoi2(char *str);
int					ft_atoi(char *str);
int					init_philo(t_init_data *init_data);
int					init_input(int argc, char **argv, t_init_data *init_data);
int					main(int argc, char **argv);
void				take_fork_on(t_philo *philo);
void				take_fork_off(t_philo *philo);
void				eat(t_philo *philo);
void				ft_sleep(t_philo *philo);
int					timer(void);
int					init_thread (t_init_data *init_data);
int					count_eat(void *args);
void				*dead_philo(void *args);
void				*start_process(void *data);
char				*output(int i);
void				show(t_philo *philo, int i);

#endif
