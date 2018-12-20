# NetComEngine    
Linux：select、epoll     
Windows：select、IOCP      

手机和手机局域网通信：手机A开热点后（不要设置密码），IP地址是一个固定的值：192.168.43.1，手机B将服务端地址设置为192.168.43.1，可以进行局域网通信。     

手机和PC局域网通信：PC插在路由器的一个端口上，手机连路由器的wifi，这样手机和PC就处于一个局域网了，可以进行局域网通信。    

- 用socket API建立简易TCP服务端		    
1、建立一个socket		     
2、绑定接受客户端连接的端口 bind		     
3、监听网络端口 listen		      
4、等待接受客户端连接 accept		      
5、向客户端发送一条数据 send      
6、关闭socket closesocket		     

- 用socket API建立简易TCP客户端   		    
1、建立一个socket		     
2、连接服务器 connect		     
3、接收服务器信息 recv		     
4、关闭socket closesocket		    

![](https://github.com/havenow/NetComEngine/blob/master/pic/socket(tcp).png)


![](https://github.com/havenow/NetComEngine/blob/master/pic/%E6%8C%81%E7%BB%AD%E5%A4%84%E7%90%86%E8%AF%B7%E6%B1%82%E7%BD%91%E7%BB%9C%E7%A8%8B%E5%BA%8F.png)


![](https://github.com/havenow/NetComEngine/blob/master/pic/%E5%8F%91%E9%80%81%E7%BB%93%E6%9E%84%E5%8C%96%E7%9A%84%E7%BD%91%E7%BB%9C%E6%B6%88%E6%81%AF%E6%95%B0%E6%8D%AE.png)


![](https://github.com/havenow/NetComEngine/blob/master/pic/1%E5%AF%B91%E7%9A%84%E9%98%BB%E5%A1%9E%E6%A8%A1%E5%BC%8F%E7%BD%91%E7%BB%9C%E7%A8%8B%E5%BA%8F.png)

![](https://github.com/havenow/NetComEngine/blob/master/pic/%E6%9C%8D%E5%8A%A1%E7%AB%AF%E4%B8%BAselect%E6%A8%A1%E5%9E%8B%E5%A4%84%E7%90%86%E5%A4%9A%E5%AE%A2%E6%88%B7%E7%AB%AF.png)
