#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>  

#include "../include/manage_users.h"
#include "../include/manage_books.h"
#include "../include/manage_transactions.h"
#include "../include/server_utils.h"

void serve_client_request(int sockfd, struct file_fd *file_fds, int query_num)
{
    char msg[100];

    if (query_num == ADD_BOOK)
    {
        struct book_info book;
        read(sockfd, &book, sizeof(book));

        add_book(file_fds->book_info_fd, &book);

        sprintf(msg, "Book added successfully with id %d", book.book_id);
        write(sockfd, msg, sizeof(msg));
    }

    else if (query_num == REMOVE_BOOK)
    {
        int book_id;
        read(sockfd, &book_id, sizeof(int));

        int res = remove_book(file_fds->book_info_fd, book_id, file_fds->transactions_fd);

        if (res == 0)
            strcpy(msg, "Book removed successfully.");

        else if (res == 1)
            strcpy(msg, "Book not found.");

        else if (res == 2)
            strcpy(msg, "Cannot remove book. Some copies are currently borrowed.");

        write(sockfd, msg, sizeof(msg));
    }

    else if (query_num == MODIFY_BOOK)
    {
        struct book_info book;
        read(sockfd, &book, sizeof(book));

        int res = modify_book(file_fds->book_info_fd, &book);

        if (res == 0)
            strcpy(msg, "Book modified successfully.");

        else
            strcpy(msg, "Book not found.");

        write(sockfd, msg, sizeof(msg));
    }

    else if (query_num == ADD_USER)
    {
        struct user_info client_info;
        read(sockfd, &client_info, sizeof(client_info));

        int res = add_user(file_fds->user_info_fd, &client_info);

        if (res == 0)
            strcpy(msg, "User registered successfully.");
        
        else
            strcpy(msg, "User already exists with given username.");
        
        write(sockfd, msg, sizeof(msg));
    }

    else if (query_num == DELETE_USER)
    {
        int user_id;
        read(sockfd, &user_id, sizeof(int));

        int res = remove_user(file_fds->user_info_fd, user_id, file_fds->transactions_fd);

        if (res == 0)
            strcpy(msg, "User removed successfully.");

        else if (res == 1)
            strcpy(msg, "User not found.");

        else if (res == 3)
            strcpy(msg, "Cannot remove user. User has currently borrowed some books.");

        else
            strcpy(msg, "Main admin cannot be deleted.");

        write(sockfd, msg, sizeof(msg));
    }

    else if (query_num == MODIFY_USER)
    {
        struct user_info user;
        read(sockfd, &user, sizeof(user));

        int res = modify_user(file_fds->user_info_fd, &user);

        if (res == 0)
            strcpy(msg, "User details updated successfully.");

        else if (res == 1)
            strcpy(msg, "User not found.");

        else
            strcpy(msg, "Main admin cannot be modified.");

        write(sockfd, msg, sizeof(msg));
    }

    else if (query_num == SHOW_ALL_USERS)
    {
        show_all_users(sockfd, file_fds->user_info_fd);
    }

    else if (query_num == SHOW_USER_TRANSACTIONS)
    {
        int user_id;
        read(sockfd, &user_id, sizeof(int));

        show_user_transactions(file_fds->transactions_fd, sockfd, user_id);
    }

    else if (query_num == SHOW_ALL_BOOKS_TO_BE_RETURNED)
    {
        show_books_to_be_returned(file_fds->transactions_fd, sockfd);   
    }
    
    else if (query_num == SHOW_ALL_BOOKS)
    {
        show_all_books(sockfd, file_fds->book_info_fd);
    }

    else if (query_num == FILTER_BOOKS_BY_TITLE)
    {
        char phrase[30];
        read(sockfd, phrase, sizeof(phrase));

        for (int i = 0; i < 30; ++i)
            phrase[i] = tolower(phrase[i]);

        filter_books_by_title(sockfd, file_fds->book_info_fd, phrase);
    }

    else if (query_num == FILTER_BOOKS_BY_AUTHOR)
    {
        char author[30];
        read(sockfd, author, sizeof(author));

        for (int i = 0; i < 30; ++i)
            author[i] = tolower(author[i]);

        filter_books_by_author(sockfd, file_fds->book_info_fd, author);
    }

    else if (query_num == ISSUE_BOOK)
    {
        int book_id, user_id, copies;
        read(sockfd, &book_id, sizeof(int));
        read(sockfd, &user_id, sizeof(int));
        read(sockfd, &copies, sizeof(int));

        int res = issue_book(file_fds->user_info_fd, file_fds->book_info_fd, file_fds->transactions_fd, book_id, user_id, copies);

        if (res == 1)
            strcpy(msg, "Invalid book ID or user ID entered.");

        else if (res == 2)
            strcpy(msg, "Sufficient copies not available.");

        else
            strcpy(msg, "Book issued successfully.");

        write(sockfd, msg, sizeof(msg));
    }

    else if (query_num == RETURN_BOOK)
    {
        int book_id, user_id;
        read(sockfd, &book_id, sizeof(int));
        read(sockfd, &user_id, sizeof(int));

        int res = return_book(file_fds->book_info_fd, file_fds->transactions_fd, book_id, user_id);

        if (res == 1)
            strcpy(msg, "Invalid book ID or user ID entered.");

        else
            strcpy(msg, "Book returned successfully.");

        write(sockfd, msg, sizeof(msg));
    }
}