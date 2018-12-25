摘录自《TCP IP网络编程》 


![](https://github.com/havenow/NetComEngine/blob/master/pic/IO%E5%A4%8D%E7%94%A8%E7%9A%84%E7%90%86%E8%A7%A3.png)

![](https://github.com/havenow/NetComEngine/blob/master/pic/%E5%A4%8D%E7%94%A8%E6%8A%80%E6%9C%AF%E5%9C%A8%E6%9C%8D%E5%8A%A1%E5%99%A8%E7%AB%AF%E7%9A%84%E5%BA%94%E7%94%A8.png)

- # select函数的功能和调用顺序
![](https://github.com/havenow/NetComEngine/blob/master/pic/select%E5%87%BD%E6%95%B0%E8%B0%83%E7%94%A8%E8%BF%87%E7%A8%8B.png)

- 设置文件描述符     
select函数可以同时监视多个文件描述符（套接字），首先需要将要监视的文件描述符集中到一起，使用fd_set数组变量执行此项操作。下图 是fd_set结构体

![](https://github.com/havenow/NetComEngine/blob/master/pic/fd_set%E7%BB%93%E6%9E%84%E4%BD%93.png)

fd_set变量中注册或更改值的操作有下列宏完成：    

![](https://github.com/havenow/NetComEngine/blob/master/pic/fd_set%E7%9B%B8%E5%85%B3%E5%87%BD%E6%95%B0%E7%9A%84%E5%8A%9F%E8%83%BD.png)


- 设置检查（监视）范围及超时      

![](https://github.com/havenow/NetComEngine/blob/master/pic/select%E5%87%BD%E6%95%B0.png)

![](https://github.com/havenow/NetComEngine/blob/master/pic/select%E5%87%BD%E6%95%B0%E5%8F%82%E6%95%B0.png)


select函数的超时时间与select函数的最后一个参数有关，timeval结构体定义如下：
```c++
struct timeval
{
    long tv_sec;    //seconds
    long tv_usec;   //microseconds
};
```

- 调用select函数后查看结果     
select函数的返回值如果大于0，说明相应数量的文件描述符发生变化。     

![](https://github.com/havenow/NetComEngine/blob/master/pic/fd_set%E5%8F%98%E9%87%8F%E7%9A%84%E5%8F%98%E5%8C%96.png)

- # select函数调用示例

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

![](https://github.com/havenow/NetComEngine/blob/master/pic/%E8%BF%90%E8%A1%8C%E7%BB%93%E6%9E%9C.png)

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

