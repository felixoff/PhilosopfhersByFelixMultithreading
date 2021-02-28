#include "philo.h"

int cleanAll(t_initData *initData)
{
    int i;

    i = 0;
    if (initData)
    {
        if (initData->philo)
        {
            while(i < initData->kol)
            {
                sem_close(initData->philo[i].sem2);
                sem_unlink(initData->philo[i].sem1);
                free(initData->philo[i].sem1);
                initData->philo[i].sem1=NULL;
                i++;
            }
            free(initData->philo)
        }
        sem_close(initData->fork);
        sem_close(initData->sem);
        sem_unlink("fork");
        sem_unlink("sem");
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
        write(2,"Ошибка семафора", ft_strlen("Ошибка мьютекса"));
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

int init_sem(t_philo *philo, int i)
{
    philo->sem1=ft_name("sem",i);
    sem_unlink(philo->sem1);
    philo->sem2=sem_open(philo->sem1,O_CREAT,0644,1);
    return(0);
}
int initPhilo(t_initData *initData)
{
int i;

i = -1;

if (!(initData->philo = malloc(sizeof(t_initData)* initData->kol)))
return(2);
while (++i < initData->kol)
{
initData->philo[i].index = i;
initData->philo[i].countEat = 0;
initData->philo[i].initData=initData;
init_sem(&initData->philo[i], i);
}
sem_unlink("fork");
sem_unlink("sem");
initData->fork=sem_open("fork",O_CREAT,0644,initData->kol);
initData->sem=sem_open("sem",O_CREATE,0644,1);
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
    sem_wait(philo->initData->fork);
    sem_wait(philo->initData->fork);
    show(philo, 1);
    show(philo, 1);
}

void takeForkOff(t_philo *philo)
{
    sem_post(philo->initData->fork);
    sem_post(philo->initData->fork);
}

void eat(t_philo *philo)
{
    show(philo, 2);
    philo->time = timer();
        sem_wait(philo->sem);
    usleep(philo->initData->eat * 1000);
    philo->countEat++;
        sem_wait(philo->sem);
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
        sem_wait(philo->sem2);
        if (!philo->initData->finish && chronos()-philo->time > philo->initData->die)
        {
            show(philo, 5);
            sem_post(philo->sem2);
            return(NULL);
        }
        sem_post(philo->sem2);
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
    sem_post(philo->initData->fork);
    sem_post(philo->initData->fork);
    sem_post(philo->initData->sem);
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

    sem_wait(&philo->initData->mutex);
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
    sem_post(&philo->initData->mutex);
}

char *ft_name(char *str, int j)
{
    int i;
    char *tmp;

    i = 0;
    tmp=malloc(sizeof(char)*100);
    while (i< ft_strlen(str))
    {
        tmp[i]=str[i];
        i++;
    }
    while (j > 0)
    {
        tmp[i]= j % 10;
        j = j / 10;
    }
    tmp[i]='\0';
    return(tmp);
}