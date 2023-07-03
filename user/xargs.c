// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/fs.h"



// int
// main(int argc, char *argv[])
// {
//     printf("=========开始执行xargs======\n");
//     for(int i=0;i<argc;i++){
//         printf("%s\n",argv[i]);
//     }
//     char* cmd=argv[1];
//     char* argvNew[argc-1];
//     argvNew[0]=cmd;
//     //初始化
//     for(int i=1;i<argc-2;i++){
//         argvNew[i]=argv[i+1];
//     }
//     char* lastElement=argv[argc-1];
//     char* p=argv[argc-1];
//     while(1){
//         if(p==0)
//         {
//             argvNew[argc-2]=lastElement;
//             if(fork()==0){
//                 exec(cmd,argvNew);
//                 exit(0);
//             }
//             else{
//                 wait(0);
//             }
//             break;
//         }
//         printf("当前*p=%c\n",*p);
//         if(*p==' '){//别忽视了空格的作用
//             printf("当前读取到的是空格\n");
//             p++;
//         }
//         else{
//             *p='\0';//呃呃，是用\0还是0来着
//             argvNew[argc-2]=lastElement;
//             if(fork()==0){
//                 exec(cmd,argvNew);
//                 exit(0);
//             }
//             else{
//                 wait(0);
//             }
//             lastElement=p+1;
//             p++;
//         }
//     }
//     return 0;
  
  
// }

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void copy(char **p1, char *p2){
    *p1 = malloc(strlen(p2) + 1);
    strcpy(*p1, p2);
}

// i为起始下标
int readLine(char **pars, int i){
    int d = 1024;
    char buf[d];
    int j = 0;
    // 读取1行
    while (read(0, buf + j, 1)){
        // 按行划分
        if (buf[j] == '\n'){
            buf[j] = 0;
            break;
        }
        j++;
        if (j == d){
            fprintf(2, "Parameters are too long!\n");
            exit(1);
        }
    }
    
    // 没有读取内容
    if (j == 0){
        return -1;
    }

    // 按照空格划分
    int k = 0;
    while (k < j){
        if (i > MAXARG){
            fprintf(2, "Too much parameters!\n");
            exit(1);
        }
        // '   abc   sdf'
        // 忽略
        while ((k < j) && (buf[k] == ' ')){
            k++;
        }
        // 起始位置
        int l = k;
        // 保留字符
        while ((k < j) && (buf[k] != ' ')){
            k++;
        }
        // 注意需要k++
        buf[k++] = 0;
        copy(&pars[i], buf + l);
        i++;
    }

    return i;
}

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Please enter more parameters!\n");
        exit(1);
    }else{
        int i;
        char *pars[MAXARG];
        for (i = 1; i < argc; i++){
            copy(&pars[i - 1], argv[i]);
        }
        
        int end;
        while ((end = readLine(pars, argc - 1)) != -1){
            pars[end] = 0;
            if (fork() == 0){
                exec(pars[0], pars);
                exit(1);
            }else{
                wait(0);
            }
        }
        exit(0);
    }
}