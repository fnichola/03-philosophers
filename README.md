# 03-philosophers
An introduction to multi-threaded programming and mutexes.

This project is a twist on the classic ["dining philosophers problem"](https://en.wikipedia.org/wiki/Dining_philosophers_problem).

<img width="294" alt="5-table" src="https://user-images.githubusercontent.com/11318028/155470096-3a2c9ad2-8216-49e1-be43-e6ff2b930c37.png">

Several philosophers are siting around a table either eating, thinking, or sleeping. On the table is a large bowl of spaghetti, and there is a fork between each philosopher. In order to eat they must pick up two forks (on their left and right), and they will set down the forks when they have finished eating. Each philospher must try to eat again soon enough to avoid dying. They do not speak to each other, and they do not know when another philospher is hungry.

In the program each philospher is represented by a POSIX thread and each fork is a mutex (to ensure it only be used by one thread at a time). The simulation parameters are defined by the following arguments to the program.

### Arguments
- **number_of_philosophers**: The number of philosophers and also the number of forks.
- **time_to_die** (in milliseconds): If a philosopher didn’t start eating time_to_die milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
- **time_to_eat** (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
- **time_to_sleep** (in milliseconds): The time a philosopher will spend sleeping.
- **number_of_times_each_philosopher_must_eat** (optional argument): If all philosophers have eaten at least number_of_times_each_philosopher_must_eat times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

### Allowed External Functions
memset, printf, malloc, free, write, usleep, gettimeofday, pthread_create, pthread_detach, pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock

## Compiling and Running
This code can be compiled by running _make_. 
Arguments are passed from the command line in order:

`./philo number_of_philosophers time_to_die time_to_eat time_to_sleep　[number_of_times_each_philosopher_must_eat]`

```
git clone https://github.com/fnichola/03-philosophers.git
cd 03-philosophers/philo
make
./philo 4 410 200 200
```

Use Ctrl+c to stop execution.

## Challenges
### Deadlock

<img width="292" alt="5-deadlock" src="https://user-images.githubusercontent.com/11318028/155467531-ce885ce4-9797-4d04-a750-973371e5810f.png"><img width="294" alt="5-llr" src="https://user-images.githubusercontent.com/11318028/155467584-75a7cabe-304d-4562-829f-58a63d581c5d.png">

In the classic dining philosophers problem it is most important to avoid "_deadlock_". If all of the philosphers reach for the fork to their left at the same time none of them will be able to pick up a second fork to start eating and they will all starve _(left figure above)_. There are several ways to avoid deadlock; for my project I made every third philosopher "right-handed" (meaning they will try to pick up the fork to their right first) and the rest of them "left-handed" _(right figure above)_. This is the optimal solution according to [this paper](https://www.researchgate.net/publication/220090796_An_optimal_distributed_solution_to_the_dining_philosophers_problem) published in the International Journal of Parallel Programming in 1986. In this configuration, even if all of the philosophers try to eat at the same time one or more of them should be able to pick up a second fork and start eating.

### Scheduling
In most implementations of the dining philosphers problem the philosphers eat for a random amount of time and do not have a starvation time limit. However, in this project all of the philosphers eat for a set length of time, and must eat again within _time_to_die_ milliseconds to avoid starvation. When _time_to_die_ is relatively short it becomes necesarry to force the philosphers into a regular eating schedule with optimum parallelization.

<img width="285" alt="4-table" src="https://user-images.githubusercontent.com/11318028/155467477-c55bab19-7df3-4957-8655-d800e420c56c.png">

For example, in the case of `./philo 4 410 200 200` each philosopher will eat for 200ms then sleep for 200ms. If they eat in order (1, 2, 3, 4) the 3rd philospher will die before getting a chance to eat. They must eat in pairs (e.g. 1 & 3 followed by 2 & 4). To delay philospher number 2 and 4 I had them think for 200ms at the beginning of the simulation.

<img width="1328" alt="4-schedule" src="https://user-images.githubusercontent.com/11318028/155466485-f9e03f28-f40e-479a-ab62-dac7cfeea7a7.png#gh-light-mode-only">
<img width="1236" alt="4-schedule-dark" src="https://user-images.githubusercontent.com/11318028/155499835-237cdedd-287d-4d5e-a4c5-217f3ffbc0dc.png#gh-dark-mode-only">

The eating schedule for an odd-numbered group of philosophers is a little bit different. For example, if there are three philosophers they must take turns eating one at a time. Philosopher #3 has to wait twice as long before starting to eat.

<img width="1107" alt="3-schedule" src="https://user-images.githubusercontent.com/11318028/155468585-df58dead-05e9-489e-b70d-01eaba87bef7.png#gh-light-mode-only">
<img width="1229" alt="3-schedule-dark" src="https://user-images.githubusercontent.com/11318028/155499920-a09be0f0-98c2-43b2-b7f1-02dd95cef276.png#gh-dark-mode-only">

The case of five philosphers is similar, except the first four can eat in pairs followed by number 5 eating alone.

<img width="1095" alt="5-schedule" src="https://user-images.githubusercontent.com/11318028/155468812-b4c6d33e-6685-49e4-8ec9-49f16c8aa966.png#gh-light-mode-only">
<img width="1239" alt="5-schedule-dark" src="https://user-images.githubusercontent.com/11318028/155499940-b2203759-cbac-466d-861e-d569068cfa34.png#gh-dark-mode-only">

### The Importance of sleep()
The obvious solution to keep track of eating, sleeping, and thinking time is to use the _sleep()_ (or _usleep_) function. However, _sleep_ and _usleep_ cannot be trusted to wake up on time. Checking the manpage for usleep reveals that "system activity or limitations may lengthen the sleep by an indeterminate amount." Using only _usleep_ causes random delays to propogate through the program, often causing on of the philosophers to die.

Using a loop like this to repeatedly check the current time can significantly improve accuracy.
```
void  my_sleep(unsigned long sleep_duration)
{
    unsigned long wake_time;
    
    wake_time = get_timestamp_m() + sleep_duration;
    while (get_timestamp_m() < wake_time)
    {
    }
}
```
However, the _while_ loop hogs processor resources and causes other threads' execution to be delayed. By adding a short sleep during the loop other threads have a chance to start executing and the wakeup time is still fairly accurate. A 1 ms (1000 µs) sleep seems to work well:
```
void  my_sleep(unsigned long sleep_duration)
{
    unsigned long wake_time;

    wake_time = get_timestamp_m() + sleep_duration;
    while (get_timestamp_m() < wake_time)
	{
        usleep(1000);
	}
}
```
