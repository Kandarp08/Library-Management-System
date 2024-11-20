#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#include "../include/manage_books.h"
#include "../include/manage_transactions.h"

void add_book(int book_info_fd, struct book_info *book)
{
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(book_info_fd, F_SETLKW, &lock);

    int book_id;
    lseek(book_info_fd, 0, SEEK_SET);
    read(book_info_fd, &book_id, sizeof(int));

    book->book_id = book_id;
    int new_book_id = book->book_id + 1;

    lseek(book_info_fd, 0, SEEK_SET);
    write(book_info_fd, &new_book_id, sizeof(int));
    
    lseek(book_info_fd, 0, SEEK_END);
    write(book_info_fd, book, sizeof(struct book_info));
    
    lock.l_type = F_UNLCK;
    fcntl(book_info_fd, F_SETLKW, &lock);
}

int remove_book(int book_info_fd, int book_id, int transactions_fd)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(book_info_fd, F_SETLKW, &lock);

    lseek(book_info_fd, sizeof(int), SEEK_SET);

    struct book_info book_details;

    while (1)
    {
        int res = read(book_info_fd, &book_details, sizeof(book_details));

        if (res == 0)
            break;

        if (book_details.book_id == book_id)
        {
            int is_borrowed = is_book_borrowed(transactions_fd, book_id);

            if (is_borrowed)
            {
                lock.l_type = F_UNLCK;
                fcntl(book_info_fd, F_SETLKW, &lock);

                return 2;
            }

            lseek(book_info_fd, -sizeof(book_details), SEEK_CUR);

            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = 0;
            lock.l_len = sizeof(book_details);

            fcntl(book_info_fd, F_SETLKW, &lock);

            book_details.book_id = -1;
            write(book_info_fd, &book_details, sizeof(book_details));
            
            lock.l_type = F_UNLCK;
            fcntl(book_info_fd, F_SETLKW, &lock);

            return 0;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(book_info_fd, F_SETLKW, &lock);

    return 1;
}

int modify_book(int book_info_fd, struct book_info *book)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(book_info_fd, F_SETLKW, &lock);

    lseek(book_info_fd, sizeof(int), SEEK_SET);

    struct book_info book_details;

    while (1)
    {
        int res = read(book_info_fd, &book_details, sizeof(book_details));

        if (res == 0)
            break;

        if (book_details.book_id == book->book_id)
        {
            lseek(book_info_fd, -sizeof(book_details), SEEK_CUR);
            
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = 0;
            lock.l_len = sizeof(book_details);

            fcntl(book_info_fd, F_SETLKW, &lock);

            write(book_info_fd, book, sizeof(struct book_info));
            
            lock.l_type = F_UNLCK;
            fcntl(book_info_fd, F_SETLKW, &lock);

            return 0;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(book_info_fd, F_SETLKW, &lock);

    return 1;
}

void show_all_books(int sockfd, int book_info_fd)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(book_info_fd, F_SETLKW, &lock);

    lseek(book_info_fd, sizeof(int), SEEK_SET);

    struct book_info book;

    while (1)
    {
        int res = read(book_info_fd, &book, sizeof(book));

        if (res == 0)
            break;

        else if (book.book_id == -1)
            continue;

        write(sockfd, &book, sizeof(book));
    }

    lock.l_type = F_UNLCK;
    fcntl(book_info_fd, F_SETLKW, &lock);

    book.book_id = -1;
    write(sockfd, &book, sizeof(book));
}

void filter_books_by_title(int sockfd, int book_info_fd, char phrase[])
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(book_info_fd, F_SETLKW, &lock);

    lseek(book_info_fd, sizeof(int), SEEK_SET);

    struct book_info book;

    while (1)
    {
        int res = read(book_info_fd, &book, sizeof(book));

        if (res == 0)
            break;

        else if (book.book_id == -1)
            continue;

        char title_lowercase[30];
        strcpy(title_lowercase, book.title);

        for (int i = 0; i < 30; ++i)
            title_lowercase[i] = tolower(title_lowercase[i]);

        if (strstr(title_lowercase, phrase) != NULL)
            write(sockfd, &book, sizeof(book));
    }

    lock.l_type = F_UNLCK;
    fcntl(book_info_fd, F_SETLKW, &lock);

    book.book_id = -1;
    write(sockfd, &book, sizeof(book));    
}

void filter_books_by_author(int sockfd, int book_info_fd, char author[])
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(book_info_fd, F_SETLKW, &lock);

    lseek(book_info_fd, sizeof(int), SEEK_SET);

    struct book_info book;

    while (1)
    {
        int res = read(book_info_fd, &book, sizeof(book));

        if (res == 0)
            break;

        else if (book.book_id == -1)
            continue;

        char author_lowercase[30];
        strcpy(author_lowercase, book.author);

        for (int i = 0; i < 30; ++i)
            author_lowercase[i] = tolower(author_lowercase[i]);

        if (!strcmp(author_lowercase, author))
            write(sockfd, &book, sizeof(book));
    }

    lock.l_type = F_UNLCK;
    fcntl(book_info_fd, F_SETLKW, &lock);

    book.book_id = -1;
    write(sockfd, &book, sizeof(book));    
}

int find_book(int book_info_fd, int book_id, struct book_info *book_ptr)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fcntl(book_info_fd, F_SETLKW, &lock);

    lseek(book_info_fd, sizeof(int), SEEK_SET);
    struct book_info book;

    while (1)
    {
        int res = read(book_info_fd, &book, sizeof(book));

        if (res == 0)
            break;

        if (book.book_id == book_id)
        {
            lock.l_type = F_UNLCK;
            fcntl(book_info_fd, F_SETLKW, &lock);

            strcpy(book_ptr->title, book.title);
            strcpy(book_ptr->author, book.author);
            strcpy(book_ptr->isbn, book.isbn);
            book_ptr->copies_available = book.copies_available;
            return 0;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(book_info_fd, F_SETLKW, &lock);

    return 1;
}