#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
    pid_t id = fork(); // 调用fork()创建子进程
    if( id == 0) {//子进程

        sleep(3);
        printf("I'm a child\n"); //子进程运行3s后结束

    }
    else if( id > 0 ){//父进程
    while(1)
    {
       sleep(3);
           printf("I'm a father\n"); //父进程一直运行
     }

    }
    else{
        printf("fail~~~\n");

    }

    return 0;

}

