#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//key：忽视了wait-exit机制并不能实现子进程等待父进程的功能！这可能和实际的pcb实现有关，pcb仅仅维护父进程的所有子进程信息

//函数功能：筛选管道p内的数据，输出所有素数。关键点在于，要想到把文件描述符作为参数进行传递
void retrive(int p[2]){
  // pipe(p);
  int buf=0;
  int temp=0;
  //读之前必须先close
  close(p[1]);
  int ret=read(p[0],(void*)&temp,sizeof(temp));
  //printf("当前ret==%d\n",ret);
  if(ret==0){
    //printf("所有进程终止\n");
    exit(0);
  }
  printf("prime %d\n",temp);

  int fdTemp[2];
  pipe(fdTemp);
  //体会：管道的机制可以隐性地实现父子进程之间数据传输的同步关系
  if(fork()==0){
    close(p[0]);//本者用不到的尽量先close的原则
    //close(fdTemp[1]);
    retrive(fdTemp);
    exit(0);
  }
  else{
    close(fdTemp[0]);
    while(read(p[0],(void*)&buf,sizeof(buf))!=0){
        //向另一个管道写
        if(buf%temp!=0){
            write(fdTemp[1],(void*)&buf,sizeof(buf));
        }
    }
    close(p[0]);
    close(fdTemp[1]);
    wait((int*)0);
  }
}

int
main(int argc, char *argv[])
{
  int buf=0;
  int p1[2];
  pipe(p1);
  //初始化p1管道
  for(int i=0;i<34;i++){
    buf=i+2;
    write(p1[1],(void*)(&buf),sizeof(buf));
  }
  retrive(p1);

  exit(0);
}









//====================分割线，下面是错误尝试=================


// void retrive(int p1[2],int p2[2],int *length){
//   int buf=0;
// //   int p1[2];//用于父进程向子进程通信
// //   int p2[2];//用于子进程向它的子进程通信
//   if(*length==0)
//     exit(0);
//   pipe(p1);
//   pipe(p2);
//   if(fork()!=0){
//     int temp=0;
//     read(p1[0],(void*)&temp,sizeof(temp));
//     printf("prime %d\n",temp);
//     *length=0;
//     while(read(p1[0],(void*)&buf,sizeof(buf))!=0){
//         //向另一个管道写
//         if(buf%temp!=0){
//             write(p2[1],(void*)&buf,sizeof(buf));
//             *length=*length+1;
//         }
//     }
//     close(p1[0]);
//     close(p1[1]);
//     close(p2[0]);
//     close(p2[1]);

//     exit(0);//父进程的结束，再不使用一些信号机制的情况下，是不会导致子进程的结束的
//   }
//   else{
//     //使用递归函数的好处在于，解决了管道命名的冲突问题
//     wait((int *)0);//使用exit+wait确保先执行完数据传输，再进行下一次递归
//     retrive(p2,p1,length);
//   }
    

// }



// int
// main(int argc, char *argv[])
// {
//   int buf=0;
//   int length=0;
//   int p1[2];//用于父进程向子进程通信
//   int p2[2];//用于子进程向它的子进程通信
//   //上面的父子进程都是相对的概念
  
//   pipe(p1);
//   pipe(p2);
//   //初始化p1管道
//   for(int i=0;i<34;i++){
//     buf=i+2;
//     write(p1[1],(void*)(&buf),sizeof(buf));
//   }
//   close(p1[1]);
  
//   if(fork()!=0){
//     int temp=0;
//     read(p1[0],(void*)&temp,sizeof(temp));
   
//     printf("prime %d\n",temp);
//     while(read(p1[0],(void*)&buf,sizeof(buf))!=0){
//         //向另一个管道写
//         if(buf%temp!=0)
//             write(p2[1],(void*)&buf,sizeof(buf));
//     }
//     close(p1[0]);
//     close(p2[0]);
//     close(p2[1]);
//     exit(0);//这里必须要通过这种方式等待printf子进程也执行结束才可以，否则还是会出现输出乱序的情况
//   }
//   else{
//     wait((int *)0);
//     //printf("第一个子进程开始执行\n");
//     //使用递归函数的好处在于，解决了管道命名的冲突问题
//     retrive(p1,p2,&length); 
//   }
  


//   exit(0);
// }
