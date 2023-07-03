#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int
main(int argc, char *argv[])
{
  int buf=666;
  int p2c[2],c2p[2];
  pipe(p2c);
  pipe(c2p);
  if(fork()==0){
    close(p2c[1]);//养成写端不用就关闭的好习惯
    read(p2c[0],(void*)&buf,sizeof(buf));
    printf("%d: received ping\n",getpid());
    close(p2c[0]);

    close(c2p[0]);
    write(c2p[1],(void*)&buf,sizeof(buf));
    close(c2p[1]);
  }
  else{
    close(p2c[0]);
    write(p2c[1],(void*)&buf,sizeof(buf));
    close(p2c[1]);//养成写端不用就关闭的好习惯
    
    close(c2p[1]);
    read(c2p[0],(void*)&buf,sizeof(buf));
    printf("%d: received pong\n",getpid());

    close(c2p[0]);
  }
  exit(0);
}
