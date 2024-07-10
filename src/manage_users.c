#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "../include/manage_users.h"

int authenticate_user(int user_info_fd, struct user_info *client_info)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(user_info_fd, F_SETLKW, &lock);

    struct user_info user;
    lseek(user_info_fd, sizeof(int), SEEK_SET);

    while (1)
    {
        int res = read(user_info_fd, &user, sizeof(user));

        if (res == 0)
            break;

        if (user.user_id != -1 && !strcmp(user.username, client_info->username) && !strcmp(user.password, client_info->password)
            && !strcmp(user.user_type, client_info->user_type))
        {
            lock.l_type = F_UNLCK;
            fcntl(user_info_fd, F_SETLKW, &lock);

            client_info->user_id = user.user_id;
            return AUTHENTICATION_SUCCESS;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(user_info_fd, F_SETLKW, &lock);

    return AUTHENTICATION_FAILURE;
}

int add_user(int user_info_fd, struct user_info *client_info)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(user_info_fd, F_SETLKW, &lock);

    lseek(user_info_fd, sizeof(int), SEEK_SET);

    while (1)
    {
        struct user_info user;
        int res = read(user_info_fd, &user, sizeof(user));

        if (res == 0)
            break;

        if (user.user_id == -1)
            continue;

        if (!strcpy(user.username, client_info->username) && !strcpy(user.user_type, client_info->user_type))
            return 1; 
    }

    lock.l_type = F_WRLCK;
    fcntl(user_info_fd, F_SETLKW, &lock);

    lseek(user_info_fd, 0, SEEK_SET);

    int user_id;
    read(user_info_fd, &user_id, sizeof(int));
    client_info->user_id = user_id;

    int new_user_id = client_info->user_id + 1;

    lseek(user_info_fd, 0, SEEK_SET);
    write(user_info_fd, &new_user_id, sizeof(int));
    
    lseek(user_info_fd, 0, SEEK_END);
    write(user_info_fd, client_info, sizeof(struct user_info));

    lock.l_type = F_UNLCK;
    fcntl(user_info_fd, F_SETLK, &lock);
}

int remove_user(int user_info_fd, int user_id)
{
    if (user_id == 1)
        return 2;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(user_info_fd, F_SETLKW, &lock);

    lseek(user_info_fd, 0, SEEK_SET);

    struct user_info user;
    int curr_user_id;

    read(user_info_fd, &curr_user_id, sizeof(int));

    while (1)
    {
        int res = read(user_info_fd, &user, sizeof(user));

        if (res == 0)
            break;

        if (user.user_id == user_id)
        {
            lseek(user_info_fd, -sizeof(user), SEEK_CUR);
            
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = 0;
            lock.l_len = sizeof(user);

            fcntl(user_info_fd, F_SETLKW, &lock);

            user.user_id = -1;
            write(user_info_fd, &user, sizeof(user));
            
            lock.l_type = F_UNLCK;
            fcntl(user_info_fd, F_SETLK, &lock);

            return 0;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(user_info_fd, F_SETLKW, &lock);

    return 1;
}

int modify_user(int user_info_fd, struct user_info *client_info)
{
    if (client_info->user_id == 1)
        return 2;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(user_info_fd, F_SETLKW, &lock);

    lseek(user_info_fd, 0, SEEK_SET);

    struct user_info user;
    int user_id;

    read(user_info_fd, &user_id, sizeof(int));

    while (1)
    {
        int res = read(user_info_fd, &user, sizeof(user));

        if (res == 0)
            break;

        if (user.user_id == client_info->user_id)
        {
            lseek(user_info_fd, -sizeof(user), SEEK_CUR);
            
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = 0;
            lock.l_len = sizeof(user);

            fcntl(user_info_fd, F_SETLKW, &lock);

            write(user_info_fd, client_info, sizeof(struct user_info));
            
            lock.l_type = F_UNLCK;
            fcntl(user_info_fd, F_SETLK, &lock);

            return 0;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(user_info_fd, F_SETLKW, &lock);

    return 1;
}

void show_all_users(int sockfd, int user_info_fd)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(user_info_fd, F_SETLKW, &lock);

    lseek(user_info_fd, sizeof(int), SEEK_SET);

    struct user_info user;

    while (1)
    {
        int res = read(user_info_fd, &user, sizeof(user));

        if (res == 0)
            break;

        else if (user.user_id == -1)
            continue;

        write(sockfd, &user, sizeof(user));
    }

    lock.l_type = F_UNLCK;
    fcntl(user_info_fd, F_SETLKW, &lock);

    user.user_id = -1;
    write(sockfd, &user, sizeof(user));
}

int find_user(int user_info_fd, int user_id)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(user_info_fd, F_SETLKW, &lock);

    lseek(user_info_fd, sizeof(int), SEEK_SET);
    struct user_info user;

    while (1)
    {
        int res = read(user_info_fd, &user, sizeof(user));

        if (res == 0)
            break;

        if (user.user_id == user_id && !strcmp(user.user_type, "member"))
        {
            lock.l_type = F_UNLCK;
            fcntl(user_info_fd, F_SETLKW, &lock);

            return 0;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(user_info_fd, F_SETLKW, &lock);
    
    return 1;
}