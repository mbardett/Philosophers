/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbardett <mbardett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 18:27:25 by mbardett          #+#    #+#             */
/*   Updated: 2022/10/22 17:50:03 by mbardett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//We can use the pthread_create function in order to create a single thread,
//but one of its arguments has to be another function defining(actually,
//running) its ROUTINE; we must also use a monitor function which,
//for each thread, inside a loop (while(1)), locks/gets/unlocks current time,
//then checks if it's greater than timeto_die(in which case the execution
// ends),otherwise checks the state fo the philo, if it has not reached the
//number of meals or has already eaten, returns 1, otherwise returns 0
//(it may sound counterintuitive, but this implementation allow us to re-use
//this function later to create another loop and already set its break
//condition);
//monitor helper
int	monitor_aux(t_rules *rules, t_philo *philo, int	*mealrounds)
{
	int			i;
	long long	time;

	i = 0;
	while (i < rules->nbr_philos)
	{
		pthread_mutex_lock(&rules->uber_lock);
		time = ft_get_time() - rules->strt_time - philo[i].last_meal;
		pthread_mutex_unlock(&rules->uber_lock);
		if (time > rules->tm_die)
		{
			ft_logs(&philo[i], "died");
			pthread_mutex_lock(&philo->rules->uber_lock);
			rules->must_run = 0;
			pthread_mutex_unlock(&philo->rules->uber_lock);
			ft_exit(rules);
		}
		if (ft_check_state(1, &philo[i]))
			(*mealrounds)++;
		i++;
	}
	return (0);
}

//monitor
void	ft_monitor(t_rules *rules)
{
	int			mealrounds;
	t_philo		*philo;

	philo = rules->philos;
	while (1)
	{
		mealrounds = 0;
		if (monitor_aux(rules, philo, &mealrounds) == 1)
			return ;
		if (mealrounds == rules->nbr_meals)
		{
			pthread_mutex_lock(&rules->uber_lock);
			rules->must_run = 0;
			pthread_mutex_unlock(&rules->uber_lock);
		}
	}
}

//thrroutine_aux
void	ft_thrroutine_aux(t_philo *ph)
{
	pthread_mutex_lock(&ph->lfork);
	ft_logs(ph, "has taken a fork");
	ft_logs(ph, "is eating");
	pthread_mutex_lock(&ph->rules->uber_lock);
	ph->last_action = ft_get_time();
	if (ph->nmeals == ph->rules->nbr_meals)
	{
		pthread_mutex_lock(&ph->philo_lock);
		ph->rules->must_run = 0;
		pthread_mutex_unlock(&ph->philo_lock);
	}
	ph->nmeals++;
	ph->last_meal = ft_get_time() - ph->rules->strt_time;
	pthread_mutex_unlock(&ph->rules->uber_lock);
	ft_wait(ph->rules->tm_eat);
	pthread_mutex_unlock(&ph->rfork);
	pthread_mutex_unlock(&ph->lfork);
	ft_logs(ph, "is sleeping");
	ph->last_action = ft_get_time();
	ft_wait(ph->rules->tm_sleep);
	ft_logs(ph, "is thinking");
	ph->last_action = ft_get_time();
}

//thread routine: lock/set last meal time/unlock;then starts a while loop
//defined by the ft_check_state function(which returns one as long as philos
//are still alive and haven't reached the number of meals defined by the
//optional argv[5] argument, basically letting the routine keep running
//for as long as necessary).read first comment above and in ft_utils.c.
void	*ft_thrroutine(void *philo)
{
	t_philo	*ph;

	ph = philo;
	pthread_mutex_lock(&ph->rules->uber_lock);
	ph->last_meal = ft_get_time() - ph->rules->strt_time;
	pthread_mutex_unlock(&ph->rules->uber_lock);
	if (ph->id % 2 == 0)
		ft_wait(ph->rules->tm_eat);
	while (ft_check_state(0, ph))
	{
		pthread_mutex_lock(&ph->rfork);
		if (ph->rules->nbr_philos == 1)
			break ;
		ft_logs(ph, "has taken a fork");
		ft_thrroutine_aux(ph);
	}
	return (NULL);
}

//ft_multithreads:init start_time in rules struct, then call pthread_create
//n times(n = nbr of philos)and feed it thrroutine as one of its args.
void	ft_multithreads(t_rules *rules)
{
	int		i;
	t_philo	*philo;

	philo = rules->philos;
	rules->strt_time = ft_get_time();
	i = -1;
	while (++i < rules->nbr_philos)
	{
		philo->last_action = rules->strt_time;
	}
	i = -1;
	while (++i < rules->nbr_philos)
	{
		pthread_create(&philo[i].philo_thread, NULL, ft_thrroutine, &philo[i]);
	}
	ft_monitor(rules);
	ft_exit(rules);
}
