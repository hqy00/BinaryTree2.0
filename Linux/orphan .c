/*************************************************************************
	> File Name: myfile.c
	> Author: 
	> Mail: 
	> Created Time: Sun 16 Apr 2017 05:39:18 AM PDT
 ************************************************************************/


#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
    pid_t id = fork(); // 调用fork()创建子进程
    if( id == 0) {//子进程
        while(1)
       {
        sleep(3);
        printf("I'm a child\n"); //子进程一直运行
       }
    }
    else if( id > 0 ){//父进程
        sleep(3);
        printf("I'm a father\n"); //父进程运行3s后结束
    }
    else{
        printf("fail~~~\n");

    }

    return 0;

}

