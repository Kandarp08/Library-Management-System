#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include "../include/manage_users.h"
#include "../include/manage_books.h"
#include "../include/manage_transactions.h"
#include "../include/server_utils.h"

void init()
{   
    int user_info_fd = open("./data/user_info.dat", O_RDWR);

    int user_id = 1;
    write(user_info_fd, &user_id, sizeof(int));

    int book_info_fd = open("./data/book_info.dat", O_RDWR);

    int book_id = 1;
    write(book_info_fd, &book_id, sizeof(int));

    struct user_info main_admin;

    // Main admin credentials

    strcpy(main_admin.username, "admin_username");
    strcpy(main_admin.password, "admin_password");
    strcpy(main_admin.user_type, "admin");

    add_user(user_info_fd, &main_admin);

    close(user_info_fd);
    close(book_info_fd);

    printf("Data files initialized successfully.\n");
    printf("Admin account added to database.\n\n");
}

void setup(struct file_fd *file_fds, char user_type[])
{
    if (!strcmp(user_type, "admin"))
    {
        file_fds->user_info_fd = open("./data/user_info.dat", O_RDWR);
        file_fds->book_info_fd = open("./data/book_info.dat", O_RDWR);
        file_fds->transactions_fd = open("./data/transactions.dat", O_RDWR);
    }

    else
    {
        file_fds->user_info_fd = open("./data/user_info.dat", O_RDONLY);
        file_fds->book_info_fd = open("./data/book_info.dat", O_RDONLY);
        file_fds->transactions_fd = open("./data/transactions.dat", O_RDONLY);
    }
}

void serve_client(char *argv[])
{
    int sockfd = atoi(argv[0]);
    struct user_info client_info;
    char msg[50];

    read(sockfd, &client_info, sizeof(client_info));

    int user_info_fd = open("./data/user_info.dat", O_RDONLY);
    struct file_fd file_fds;

    while (authenticate_user(user_info_fd, &client_info) == AUTHENTICATION_FAILURE)
    {
        strcpy(msg, "Invalid Credentials. Please try again.");
        write(sockfd, msg, sizeof(msg));
        read(sockfd, &client_info, sizeof(client_info));
    }
    
    strcpy(msg, "Authenticated successfully.");
    write(sockfd, msg, sizeof(msg));
    write(sockfd, &client_info, sizeof(client_info));

    setup(&file_fds, client_info.user_type);

    while (1)
    {
        int query_num;
        read(sockfd, &query_num, sizeof(int));

        if (query_num == 0)
            break;

        serve_client_request(sockfd, &file_fds, query_num);
    }

    close(sockfd);
}

int main()
{
    init();

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    int res = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (res < 0)
    {
        perror("bind");
        exit(0);
    }

    res = listen(sockfd, 5);

    if (res < 0)
    {
        perror("listen");
        exit(0);
    }

    int addr_len = sizeof(struct sockaddr_in);

    printf("Library Server is listening.\n\n");

    while (1)
    {
        int newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

        pthread_t thread;
        char *argv[1];

        argv[0] = (char *)malloc(10 * sizeof(char));
        sprintf(argv[0], "%d", newsockfd);

        pthread_create(&thread, NULL, (void *)serve_client, argv);
    }
}