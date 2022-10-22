/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbardett <mbardett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 18:15:36 by mbardett          #+#    #+#             */
/*   Updated: 2022/10/22 17:53:17 by mbardett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// checks whether the string passed as argument is made of only numbers or not
int	ft_nums_only(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while ((argv[i][j] >= 9 && argv[i][j] <= 13) || argv[i][j] == 32)
			j++;
		if (argv[i][j] == '+')
			j++;
		if (argv[i][j] == '\0')
			return (0);
		while (argv[i][j] != '\0')
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

//turn string of numeric chars into ints
int	ft_atoi(char *str)
{
	int	i;
	int	sign;
	int	to_ret;

	i = 0;
	sign = 1;
	to_ret = 0;
	if ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == 43 || str[i] == 45)
	{
		if (str[i] == 45)
			sign *= -1;
		i += 1;
	}
	if (str[i] == 43 || str[i] == 45)
		return (0);
	while (str[i] >= 48 && str[i] <= 57)
	{
		to_ret = (to_ret * 10) + (str[i] - 48);
		i++;
	}
	return (to_ret * sign);
}

//init each philo, including mutexes and other vars.
void	ft_init_aux(t_rules *rules)
{
	int	i;

	i = -1;
	while (++i < rules->nbr_philos)
	{
		rules->philos[i].rules = rules;
		rules->philos[i].id = i + 1;
		rules->philos[i].nmeals = 0;
		rules->philos[i].last_meal = 0;
		rules->must_run = 1;
		pthread_mutex_init(&rules->philos[i].rfork, NULL);
		pthread_mutex_init(&rules->philos[i].lfork, NULL);
		pthread_mutex_init(&rules->philos[i].philo_lock, NULL);
		if (i == 0)
			rules->philos[i].lfork = rules->forks
			[rules->nbr_philos - 1].fork_mutex;
		else
			rules->philos[i].lfork = rules->forks[i].fork_mutex;
		if (i == rules->nbr_philos && rules->nbr_philos != 1)
			rules->philos[i].rfork = rules->philos[0].lfork;
		else
			rules->philos[i].rfork = rules->forks[i + 1].fork_mutex;
	}
}

//malloc mutexes and philo_structs,init rules vars and mutexes
void	ft_init(t_rules *rules, char **argv)
{
	int	i;

	i = 0;
	rules->nbr_philos = ft_atoi(argv[1]);
	rules->tm_die = ft_atoi(argv[2]);
	rules->tm_eat = ft_atoi(argv[3]);
	rules->tm_sleep = ft_atoi(argv[4]);
	rules->forks = malloc (sizeof(t_forks) * rules->nbr_philos);
	if (!rules->forks)
	{
		write(1, "not enough memory to allocate\n", 33);
		ft_exit(rules);
	}
	i = -1;
	while (++i < rules->nbr_philos)
		pthread_mutex_init(&rules->forks[i].fork_mutex, NULL);
	pthread_mutex_init(&rules->uber_lock, NULL);
	pthread_mutex_init(&rules->write, NULL);
	rules->philos = malloc(sizeof(t_philo) * rules->nbr_philos);
	if (!rules->philos)
	{
		write(1, "not enough memory to allocate\n", 33);
		ft_exit(rules);
	}
	ft_init_aux(rules);
}

//handle input, only if numeric, ft_atoi them, launch pthread_create, call
// a "cleaning"function for pthread_destroy and free.
int	main(int argc, char **argv)
{
	t_rules	rules;

	if ((argc < 5 || argc > 6) || ft_nums_only(argc, argv) == 0)
	{
		printf("Error:invalid number or type of arguments\n");
		exit (0);
	}
	ft_init(&rules, argv);
	if (argc == 6)
		rules.nbr_meals = ft_atoi(argv[5]);
	else
		rules.nbr_meals = -1;
	ft_multithreads(&rules);
}
