#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];

    // 创建socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("无法创建socket");
        return 1;
    }

    // 设置服务器地址
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(9000);

    // 连接到服务器
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("连接错误");
        return 1;
    }

    // 发送数据
    message = "GET /api/graphs/ HTTP/1.1\r\nHost: localhost\r\n\r\n";
    if (send(socket_desc, message, strlen(message), 0) < 0)
    {
        puts("发送失败");
        return 1;
    }

    // 接收并打印服务器回复
    char *body;
    int body_len = 0;
    while (recv(socket_desc, server_reply, 2000, 0) > 0)
    {
        if (body_len == 0)
        {
            body = strstr(server_reply, "\r\n\r\n");
            if (body != NULL)
            {
                body += 4;
                body_len = strlen(body);
            }
        }
        puts(server_reply);
        memset(server_reply, 0, 2000);
    }

    return 0;
}
