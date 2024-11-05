#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "../include/client_utils.h"

void print_utils(char user_type[])
{
    printf("\nWelcome to the Library!\n\n");

    printf("*****************************\n");
    printf("Please select a valid option: \n\n");

    printf("0. Logout\n");

    if (!strcmp(user_type, "admin"))
    {
        printf("1. Add Book\n");
        printf("2. Remove Book\n");
        printf("3. Modify Book\n");
        printf("4. Add User\n");
        printf("5. Remove User\n");
        printf("6. Update user details\n");
        printf("7. Show all users\n");
        printf("8. Show user transactions\n");
        printf("9. Show unreturned books\n");
        printf("10. Show all books\n");
        printf("11. Filter books by title\n");
        printf("12. Filter books by author\n");
        printf("13. Issue book\n");
        printf("14. Return book\n\n");
    }

    else
    {
        printf("1. Update profile\n");
        printf("2. Show borrowed books\n");
        printf("3. Show all books\n");
        printf("4. Filter books by title\n");
        printf("5. Filter books by author\n\n");
    }
}

void send_request_to_server(int sockfd, int query_num, int user_id, char user_type[])
{
    int actual_query;
    char msg[100];

    if (!strcmp(user_type, "admin"))
    {
        if (query_num == 1)
        {
            actual_query = ADD_BOOK;
            write(sockfd, &actual_query, sizeof(int));

            struct book_info book;

            printf("Please enter the book details:\n");
            printf("Title: ");
            scanf("%[^\n]%*c", book.title);
            printf("Author: ");
            scanf("%[^\n]%*c", book.author);
            printf("ISBN: ");
            scanf("%[^\n]%*c", book.isbn);
            printf("Copies available: ");
            scanf("%d", &book.copies_available);

            write(sockfd, &book, sizeof(book));
            read(sockfd, msg, sizeof(msg));

            printf("%s\n", msg);
        }

        else if (query_num == 2)
        {
            actual_query = REMOVE_BOOK;
            write(sockfd, &actual_query, sizeof(int));

            int book_id;
            printf("Please enter book ID of book to be removed: ");
            scanf("%d", &book_id);

            write(sockfd, &book_id, sizeof(int));
            read(sockfd, msg, sizeof(msg));

            printf("%s\n", msg);
        }
        
        else if (query_num == 3)
        {
            actual_query = MODIFY_BOOK;
            write(sockfd, &actual_query, sizeof(int));

            struct book_info book;

            printf("Please enter book ID of book to be updated: ");
            scanf("%d%*c", &book.book_id);

            printf("Please enter the new book details:\n");
            printf("Title: ");
            scanf("%[^\n]%*c", book.title);
            printf("Author: ");
            scanf("%[^\n]%*c", book.author);
            printf("ISBN: ");
            scanf("%[^\n]%*c", book.isbn);
            printf("Copies available: ");
            scanf("%d", &book.copies_available);

            write(sockfd, &book, sizeof(book));
            read(sockfd, msg, sizeof(msg));

            printf("%s\n", msg);
        }

        else if (query_num == 4)
        {
            actual_query = ADD_USER;
            write(sockfd, &actual_query, sizeof(int));

            struct user_info user;

            printf("Enter username: ");
            scanf("%[^\n]%*c", user.username);
            printf("Enter password: ");
            scanf("%[^\n]%*c", user.password);
            printf("Enter user type (admin / member): ");
            scanf("%[^\n]%*c", user.user_type);

            write(sockfd, &user, sizeof(user));
            read(sockfd, msg, sizeof(msg));

            printf("%s\n", msg);
        }

        else if (query_num == 5)
        {
            actual_query = DELETE_USER;
            write(sockfd, &actual_query, sizeof(int));

            int user_id;
            printf("Please enter user ID of user to be removed: ");
            scanf("%d", &user_id);

            write(sockfd, &user_id, sizeof(int));
            read(sockfd, msg, sizeof(msg));

            printf("%s\n", msg);
        }

        else if (query_num == 6)
        {
            actual_query = MODIFY_USER;
            write(sockfd, &actual_query, sizeof(int));

            struct user_info user;

            printf("Please enter user ID of user to be updated: ");
            scanf("%d%*c", &user.user_id);

            printf("Please enter the new user details:\n");
            printf("Username: ");
            scanf("%[^\n]%*c", user.username);
            printf("Password: ");
            scanf("%[^\n]%*c", user.password);
            printf("User type (admin/member): ");
            scanf("%[^\n]%*c", user.user_type);

            write(sockfd, &user, sizeof(user));
            read(sockfd, msg, sizeof(msg));

            printf("%s\n", msg);
        }

        else if (query_num == 7)
        {
            actual_query = SHOW_ALL_USERS;
            write(sockfd, &actual_query, sizeof(int));

            printf("\n");
            struct user_info user;

            while (1)
            {
                read(sockfd, &user, sizeof(user));

                if (user.user_id == -1)
                    break;

                printf("User ID: %d\n", user.user_id);
                printf("Username: %s\n", user.username);
                printf("User type: %s\n\n", user.user_type);
            }
        }

        else if (query_num == 8)
        {
            actual_query = SHOW_USER_TRANSACTIONS;
            write(sockfd, &actual_query, sizeof(int));

            int user_id;
            printf("Enter user ID of member: ");
            scanf("%d", &user_id);

            write(sockfd, &user_id, sizeof(int));

            struct transaction_info transaction;

            while (1)
            {
                read(sockfd, &transaction, sizeof(transaction));

                if (transaction.user_id == -1)
                    break;

                printf("User ID: %d\n", transaction.user_id);
                printf("Book ID: %d\n", transaction.book_id);
                printf("Copies Borrowed: %d\n\n", transaction.copies_borrowed);
            }
        }

        else if (query_num == 9)
        {
            actual_query = SHOW_ALL_BOOKS_TO_BE_RETURNED;
            write(sockfd, &actual_query, sizeof(int));

            struct transaction_info transaction;

            while (1)
            {
                read(sockfd, &transaction, sizeof(transaction));

                if (transaction.user_id == -1)
                    break;

                printf("User ID: %d\n", transaction.user_id);
                printf("Book ID: %d\n", transaction.book_id);
                printf("Copies Borrowed: %d\n\n", transaction.copies_borrowed);
            }
        }

        else if (query_num == 10)
        {
            actual_query = SHOW_ALL_BOOKS;
            write(sockfd, &actual_query, sizeof(int));

            printf("\n");
            struct book_info book;

            while (1)
            {
                read(sockfd, &book, sizeof(book));

                if (book.book_id == -1)
                    break;

                printf("Book ID: %d\n", book.book_id);
                printf("Title: %s\n", book.title);
                printf("Author: %s\n", book.author);
                printf("ISBN: %s\n", book.isbn);
                printf("Copies available: %d\n\n", book.copies_available);
            }
        }

        else if (query_num == 11)
        {
            actual_query = FILTER_BOOKS_BY_TITLE;
            write(sockfd, &actual_query, sizeof(int));

            char phrase[30];
            printf("Enter title: ");
            scanf("%[^\n]%*c", phrase);

            write(sockfd, phrase, sizeof(phrase));

            printf("\n");
            struct book_info book;

            while (1)
            {
                read(sockfd, &book, sizeof(book));

                if (book.book_id == -1)
                    break;

                printf("Book ID: %d\n", book.book_id);
                printf("Title: %s\n", book.title);
                printf("Author: %s\n", book.author);
                printf("ISBN: %s\n", book.isbn);
                printf("Copies available: %d\n\n", book.copies_available);
            }
        }

        else if (query_num == 12)
        {
            actual_query = FILTER_BOOKS_BY_AUTHOR;
            write(sockfd, &actual_query, sizeof(int));

            char author[30];
            printf("Enter author name: ");
            scanf("%[^\n]%*c", author);

            write(sockfd, author, sizeof(author));

            printf("\n");
            struct book_info book;

            while (1)
            {
                read(sockfd, &book, sizeof(book));

                if (book.book_id == -1)
                    break;

                printf("Book ID: %d\n", book.book_id);
                printf("Title: %s\n", book.title);
                printf("Author: %s\n", book.author);
                printf("ISBN: %s\n", book.isbn);
                printf("Copies available: %d\n\n", book.copies_available);
            }
        }

        else if (query_num == 13)
        {
            actual_query = ISSUE_BOOK;
            write(sockfd, &actual_query, sizeof(int));

            int book_id, user_id, copies;
            printf("Enter book ID to issue: ");
            scanf("%d", &book_id);
            printf("Enter user ID of member: ");
            scanf("%d", &user_id);
            printf("Enter number of copies: ");
            scanf("%d", &copies);

            write(sockfd, &book_id, sizeof(int));
            write(sockfd, &user_id, sizeof(int));
            write(sockfd, &copies, sizeof(int));

            read(sockfd, msg, sizeof(msg));
            printf("%s\n", msg);
        }

        else if (query_num == 14)
        {
            actual_query = RETURN_BOOK;
            write(sockfd, &actual_query, sizeof(int));

            int book_id, user_id;
            printf("Enter book ID to return: ");
            scanf("%d", &book_id);
            printf("Enter user ID of member: ");
            scanf("%d", &user_id);

            write(sockfd, &book_id, sizeof(int));
            write(sockfd, &user_id, sizeof(int));

            read(sockfd, msg,  sizeof(msg));
            printf("%s\n", msg);
        }
    }

    else
    {
        if (query_num == 1)
        {
            actual_query = MODIFY_USER;
            write(sockfd, &actual_query, sizeof(int));

            struct user_info user;
            user.user_id = user_id;
            strcpy(user.user_type, user_type);

            printf("Please enter the new user details:\n");
            printf("Username: ");
            scanf("%[^\n]%*c", user.username);
            printf("Password: ");
            scanf("%[^\n]%*c", user.password);

            write(sockfd, &user, sizeof(user));
            read(sockfd, msg, sizeof(msg));

            printf("%s\n", msg);
        }

        else if (query_num == 2)
        {
            actual_query = SHOW_USER_TRANSACTIONS;
            write(sockfd, &actual_query, sizeof(int));

            write(sockfd, &user_id, sizeof(int));

            struct transaction_info transaction;

            while (1)
            {
                read(sockfd, &transaction, sizeof(transaction));

                if (transaction.user_id == -1)
                    break;

                printf("Book ID: %d\n", transaction.book_id);
                printf("Copies Borrowed: %d\n\n", transaction.copies_borrowed);
            }
        }
        
        else if (query_num == 3)
        {
            actual_query = SHOW_ALL_BOOKS;
            write(sockfd, &actual_query, sizeof(int));

            printf("\n");
            struct book_info book;

            while (1)
            {
                read(sockfd, &book, sizeof(book));

                if (book.book_id == -1)
                    break;

                printf("Book ID: %d\n", book.book_id);
                printf("Title: %s\n", book.title);
                printf("Author: %s\n", book.author);
                printf("ISBN: %s\n", book.isbn);
                printf("Copies available: %d\n\n", book.copies_available);
            }
        }

        else if (query_num == 4)
        {
            actual_query = FILTER_BOOKS_BY_TITLE;
            write(sockfd, &actual_query, sizeof(int));

            char phrase[30];
            printf("Enter title: ");
            scanf("%[^\n]%*c", phrase);

            write(sockfd, phrase, sizeof(phrase));

            printf("\n");
            struct book_info book;

            while (1)
            {
                read(sockfd, &book, sizeof(book));

                if (book.book_id == -1)
                    break;

                printf("Book ID: %d\n", book.book_id);
                printf("Title: %s\n", book.title);
                printf("Author: %s\n", book.author);
                printf("ISBN: %s\n", book.isbn);
                printf("Copies available: %d\n\n", book.copies_available);
            }
        }

        else if (query_num == 5)
        {
            actual_query = FILTER_BOOKS_BY_AUTHOR;
            write(sockfd, &actual_query, sizeof(int));

            char author[30];
            printf("Enter author name: ");
            scanf("%[^\n]%*c", author);

            write(sockfd, author, sizeof(author));

            printf("\n");
            struct book_info book;

            while (1)
            {
                read(sockfd, &book, sizeof(book));

                if (book.book_id == -1)
                    break;

                printf("Book ID: %d\n", book.book_id);
                printf("Title: %s\n", book.title);
                printf("Author: %s\n", book.author);
                printf("ISBN: %s\n", book.isbn);
                printf("Copies available: %d\n\n", book.copies_available);
            }
        }   
    }
}