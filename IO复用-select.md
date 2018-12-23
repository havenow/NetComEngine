摘录《TCP IP网络编程》     

- select函数调用示例

下面的代码可在Linux、MACOS下运行     
```c++
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    
    FD_ZERO(&reads);
    FD_SET(0, &reads);//将文件描述符0对应的位设置为1，即监视标准输入的变化（监视控制台）
    
    /*
     timeout.tv_sec = 5;
     timeout.tv_usec = 5000;
     */
    
    while (1) {
        temps = reads;//内容复制
        timeout.tv_sec = 5;//每次调用select之前都会初始化timeout
        timeout.tv_usec = 5000;
        result = select(1, &temps, 0, 0, &timeout);
        //调用select函数后，除发生变化的文件描述符对应位外，剩下的所有位将初始化为0。
        //调用select函数后，结构体tiemval的成员tv_sec和tv_usec的值将被替换为超时前剩余时间。
        if (result == -1)
        {
            puts("select() error!");
        }
        else if (result == 0)
        {
            puts("Time out!");
        }
        else
        {
            if (FD_ISSET(0, &temps))//验证发生变化的文件描述符是否为标准输入
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
    }
}

```

下面的实现方式不用做fd_set变量reads的复制，而是在while循环里面做FD_ZERO FD_SET     
```c++
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    
    /*
     timeout.tv_sec = 5;
     timeout.tv_usec = 5000;
     */
    
    while (1) {
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(0, &reads);
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        result = select(1, &reads, 0, 0, &timeout);
        if (result == -1)
        {
            puts("select() error!");
        }
        else if (result == 0)
        {
            puts("Time out!");
        }
        else
        {
            if (FD_ISSET(0, &reads))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
    }
}

```

