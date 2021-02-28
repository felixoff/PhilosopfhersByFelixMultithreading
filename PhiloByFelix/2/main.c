#include "philo.h"

int cleanAll(t_initData *initData)
{
    int i;

    i = 0;
    if (initData)
    {
        if (initData->fork)
        {
            while(i < initData->kol)
                pthread_mutex_destroy(&initData->fork[i++]);
            free(initData->fork)
        }
        pthread_mutex_destroy(&initData->mutex);
        if (initData->philo)
        {
            i = 0;
            while(i < initData->kol)
                pthread_mutex_destroy(&initData->philo[i++].mutex)
            free(initData->philo);
            initData->philo = NULL;
        }
    }
    return(1);
}

int error(t_initData *initData, int i)
{
    if (i == 2)
    {
        write(2,"Ошибка маллока", ft_strlen("Ошибка маллока"));
        cleanAll(initData)
    }
    else if (i == 3)
    {
        write(2,"Ошибка мьютекса", ft_strlen("Ошибка мьютекса"));
        cleanAll(initData)
    }
    return(i);
}
void ft_putchar_fd(int fd, char c)
{
    if (!fd)
        return ;
    write(fd,&c,1);
}

void ft_putnbr_fd(int fd, int number)
{
    long nb;

    nb = number;
    if (nb < 0)
    {
        ft_putchar_fd(fd, '-')
        nb = nb*-1;
    }
    if (nb > 9)
    {
        ft_putnbr_fd(fd, nb / 10 +48);
        ft_putchar_fd(fd, nb % 10 + 48);
    }
    if (nb < 9 )
        ft_putchar_fd(fd, nb % 10 + 48);
}

int ft_strlen(char *str)
{
int i;

i = 0;

while(str[i])
i++;
return(i);
}

int atoi2(char *str)
{
    int i;
    int minus;
    int result;

    i = 0;
    minus = 1;
    while (str[i] = ' ')
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            minus = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
        result = result * 10 + (str[i++] - '0');
    return(result*minus);
}

int atoi(char *str)
{
int i;

i = 0;
while (str[i] && str[i]==' ')
i++;
if (str[i] == '-' || str[i]=='+')
i++;
while (str[i] && (str[i] >= '0' && str[i] <= '9'))
i++;
if (str[i] == '\0')
    return(atoi2(str));
return(-1);
}

int initPhilo(t_initData *initData)
{
int i;

i = -1;

if (!(initData->philo = malloc(sizeof(t_initData)* initData->kol)) ||
!(initData->fork = malloc(sizeof(pthread_mutex_t)*initData->kol)))
return(2);
while (++i < initData->kol)
{
initData->philo[i].index = i;
initData->philo[i].countEat = 0;
initData->philo[i].initData=initData;
initData->philo[i].lf = i;
initData->philo[i].rf = (i + 1) % initData->kol;
if (pthread_mutex_init(&initData->philo[i].mutex, NULL))
return(3);
initData->philo[i].initData = initData;
if (pthread_mutex_init(&initData->fork[i], NULL))
return(3);
}
if (pthread_mutex_init(&initData->mutex, NULL))
return(3);
return(0);
}

int initInput(int argc, char *argv, t_initData *initData)
{
if (argc < 2 || argc > 6)
{
write(2,"Неверное количество аргументов", ft_strlen("Неверное количество аргументов"));
return(1);
}
if ((initData->kol=atoi(argv[1])) < 2)
write(2,"Неверное количество философов", ft_strlen("Неверное количество философов"));
else if ((initData->die=atoi(argv[2])) < 60)
write(2,"Неверное время смерти", ft_strlen("Неверное время смерти"));
else if ((initData->eat=atoi(argv[3])) < 60)
write(2,"Неверное время еды", ft_strlen("Неверное время еды"));
else if ((initData->sleep=atoi(argv[4])) < 60)
write(2,"Неверное время сна", ft_strlen("Неверное время сна"));
else if (argc == 6 && (initData->countEat=atoi(argv[5])) < 0)
write(2,"Неверное количество блюд", ft_strlen("Неверное количество блюд"));
else
return(initPhilo(&initData));
return(1);
}

int main(int argc, char **argv)
{
t_initData initData;
int i;

memset(&initData, 0, sizeof(t_initData));
if ((i =initInput(argc, argv, &initData)))
return(error(&initData,i));
if ((i=initThread(&initData)))
return(error(&initData,3));
clean(&initData);
return(0);
}

void takeForkOn(t_philo *philo)
{
    pthread_mutex_lock(&philo->initData->fork[philo->lf]);
    show(philo, 1);
    pthread_mutex_lock(&philo->initData->fork[philo->rf]);
    show(philo, 1);
}

void takeForkOff(t_philo *philo)
{
    pthread_mutex_unlock(&philo->initData->fork[philo->lf]);
    pthread_mutex_unlock(&philo->initData->fork[philo->rf]);
}

void eat(t_philo *philo)
{
    show(philo, 2);
    philo->time = timer();
    pthread_mutex_lock(&philo->mutex);
    usleep(philo->initData->eat * 1000);
    philo->countEat++;
    pthread_mutex_unlock(&philo->mutex);
}

void sleep(t_philo *philo)
{
    show(philo, 3);
    usleep(philo->initData->sleep * 1000);
}

int timer(void)
{
    int i;
    struct timeval time;

    i = 0;
    gettimeofday(&time, NULL);

    i = (time.tv_sec * 1000 + time.tv_usec / 1000);
    return(i);
}

int initThread (t_initData *initData)
{
    int i;
    pthread_t thread;

    initData->time = timer();
    i = 0;
    while(i < initData->kol)
    {
        initData->philo[i].time = timer();
        if(pthread_create(&thread, NULL, startProcess, (void*)&initData->philo[i]))
            return(1);
        pthread_detach(thread);
        if(pthread_create(&initData->philo[i].thread, NULL,
         deadPhilo, (void*)&initData->philo[i]))
            return(1);
        usleep(100);
        i++;
    }
    i = 0;
    while(i < initData->kol)
        pthread_join(&initData->philo[i++].thread, NULL);
    return(0);
}
int countEat(void *args)
{
    t_philo philo

    philo = args;
    if (philo->countEat >= philo->initData->countEat)
        philo->initDate->addFatPhilo++;
    if(philo->initData->addFatPhilo == philo->initData->kol)
    {
        show(philo, 6);
        return (1);
    }
    return(0);
}
void *deadPhilo(void *args)
{
    t_philo philo;

    philo = args;
    while (!philo->initData->finish)
    {
        pthread_mutex_lock(&philo->mutex)
        if (!philo->initData->finish && chronos()-philo->time > philo->initData->die)
        {
            show(philo, 5);
            pthread_mutex_unlock(&philo->mutex);
            return(NULL);
        }
        pthread_mutex_unlock(&philo->mutex);
        usleep(1000);
    }
    return(NULL);
}
void *startProcess(void *data)
{
    t_philo  *philo;

    philo = data;
    philo->time = timer();
    while(!philo->initData->finish)
    {
        takeForkOn(philo);
        if (philo->initData->finish)
            break;
        eat(philo);
        if (philo->initData->finish)
            break;
        takeForkOff(philo);
        if (philo->initData->countEat > 0 && countEat(philo))
            break;
        if (philo->initData->finish)
            break;
        sleep(philo);
        show(philo, 4)
    }
    pthread_mutex_unlock(&philo->initData->fork[philo->lf]);
    pthread_mutex_unlock(&philo->initData->fork[philo->rf]);
    pthread_mutex_unlock(&philo->initData->mutex);
    return(NULL);
}

char *output(int i)
{
    if (i == 1)
        return ("беру вилку\n");
    else if (i == 2)
        return ("кушаю\n");
    else if (i == 3)
        return ("сплю\n");
    else if (i == 4)
        return ("думаю\n");
    else if (i == 5)
        return ("умер RIP\n");
        return ("все наелись\n");
}

void show(t_philo philo, int i)
{
    char *tmp;

    pthread_mutex_lock(&philo->initData->mutex);
    if (!philo->initData->finish)
    {
    tmp = output(i);
    ft_putnbr_fd(chronos() - philo->initData->time, 1)
    if (i != 6)
    {
        write (1, "Philosoph number", ft_strlen("Philosoph number"))
        ft_putnbr_fd(philo->index + 1, 1)
    }
    write (1, tmp, ft_strlen(tmp));
    tmp = NULL;
    if (i == 5 || i == 6)
    {
        philo->initData->finish = 1;
        return;
    }
    }
    pthread_mutex_unlock(&philo->initData->mutex);
}