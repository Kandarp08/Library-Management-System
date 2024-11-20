#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "../include/manage_transactions.h"
#include "../include/manage_books.h"
#include "../include/manage_users.h"

int issue_book(int user_info_fd, int book_info_fd, int transactions_fd, int book_id, int user_id, int copies)
{
    struct book_info book;
    book.book_id = book_id;
    int res_book = find_book(book_info_fd, book_id, &book);
    int res_user = find_user(user_info_fd, user_id);

    if (res_book != 0 || res_user != 0)
        return 1;

    else if (book.copies_available < copies)
        return 2;

    book.copies_available -= copies;
    modify_book(book_info_fd, &book);

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(transactions_fd, F_SETLKW, &lock);

    lseek(transactions_fd, 0, SEEK_END);

    struct transaction_info transaction = {book_id, user_id, copies};
    write(transactions_fd, &transaction, sizeof(transaction));

    lock.l_type = F_UNLCK;
    fcntl(transactions_fd, F_SETLKW, &lock);

    return 0;
}

int return_book(int book_info_fd, int transactions_fd, int book_id, int user_id)
{
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(transactions_fd, F_SETLKW, &lock);  

    lseek(transactions_fd, 0, SEEK_SET);
    int flag = 1;

    while (1)
    {
        struct transaction_info transaction;
        int res = read(transactions_fd, &transaction, sizeof(transaction));

        if (res == 0)
            break;

        if (transaction.book_id == book_id && transaction.user_id == user_id)
        {
            lseek(transactions_fd, -sizeof(transaction), SEEK_CUR);
            flag = 0;

            transaction.book_id = -1;
            transaction.user_id = -1;
            write(transactions_fd, &transaction, sizeof(transaction));
            
            struct book_info book;
            book.book_id = book_id;
            find_book(book_info_fd, book_id, &book);
            book.copies_available += transaction.copies_borrowed;

            modify_book(book_info_fd, &book);
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(transactions_fd, F_SETLKW, &lock);

    return flag;
}

void show_user_transactions(int transactions_fd, int sockfd, int user_id)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(transactions_fd, F_SETLKW, &lock);  

    lseek(transactions_fd, 0, SEEK_SET);
    struct transaction_info transaction;

    while (1)
    {
        int res = read(transactions_fd, &transaction, sizeof(transaction));

        if (res == 0)
            break;

        if (transaction.user_id == user_id)
            write(sockfd, &transaction, sizeof(transaction));
    }

    transaction.user_id = -1;
    write(sockfd, &transaction, sizeof(transaction));

    lock.l_type = F_UNLCK;
    fcntl(transactions_fd, F_SETLKW, &lock);
}

void show_books_to_be_returned(int transactions_fd, int sockfd)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(transactions_fd, F_SETLKW, &lock);  

    lseek(transactions_fd, 0, SEEK_SET);
    struct transaction_info transaction;

    while (1)
    {
        int res = read(transactions_fd, &transaction, sizeof(transaction));

        if (res == 0)
            break;

        if (transaction.user_id != -1 && transaction.book_id != -1)
            write(sockfd, &transaction, sizeof(transaction));
    }

    transaction.user_id = -1;
    write(sockfd, &transaction, sizeof(transaction));

    lock.l_type = F_UNLCK;
    fcntl(transactions_fd, F_SETLKW, &lock);
}

int is_book_borrowed(int transactions_fd, int book_id)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(transactions_fd, F_SETLKW, &lock);  

    lseek(transactions_fd, 0, SEEK_SET);
    struct transaction_info transaction;

    while (1)
    {
        int res = read(transactions_fd, &transaction, sizeof(transaction));

        if (res == 0)
            break;

        if (transaction.book_id == book_id)
            return 1;
    }

    lock.l_type = F_UNLCK;
    fcntl(transactions_fd, F_SETLKW, &lock);

    return 0;
}

int has_borrowed_book(int transactions_fd, int user_id)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(transactions_fd, F_SETLKW, &lock);  

    lseek(transactions_fd, 0, SEEK_SET);
    struct transaction_info transaction;

    while (1)
    {
        int res = read(transactions_fd, &transaction, sizeof(transaction));

        if (res == 0)
            break;

        if (transaction.user_id == user_id)
            return 1;
    }

    lock.l_type = F_UNLCK;
    fcntl(transactions_fd, F_SETLKW, &lock);

    return 0;
}