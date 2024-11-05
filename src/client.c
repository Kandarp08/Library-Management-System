#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../include/client_utils.h"

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8000);

    int res = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (res < 0)
    {
        perror("connect");
        exit(1);
    } 

    struct user_info client_info;
    char msg[50];

    while (1)
    {
        printf("Login as (admin / member): ");
        scanf("%[^\n]%*c", client_info.user_type);

        printf("Enter username: ");
        scanf("%[^\n]%*c", client_info.username);

        printf("Enter password: ");
        scanf("%[^\n]%*c", client_info.password);

        write(sockfd, &client_info, sizeof(client_info));

        read(sockfd, msg, sizeof(msg));
        printf("%s\n", msg);

        if (!strcmp(msg, "Authenticated successfully."))
            break;
    }

    read(sockfd, &client_info, sizeof(client_info));
    print_utils(client_info.user_type);

    while (1)
    {
        int query_num;
        printf("\nEnter a valid option: ");
        scanf("%d%*c", &query_num);

        if (query_num == 0)
        {
            printf("\nThank you for visiting the library!\n");
            write(sockfd, &query_num, sizeof(int));
            break;
        }

        send_request_to_server(sockfd, query_num, client_info.user_id, client_info.user_type);
    }

    close(sockfd);
}