struct book_info
{
    int book_id;
    char title[30];
    char author[30];
    char isbn[13];
    int copies_available;
};

void add_book(int book_info_fd, struct book_info *book);

int remove_book(int book_info_fd, int book_id, int transactions_fd);

int modify_book(int book_info_fd, struct book_info *book);

void show_all_books(int sockfd, int book_info_fd);

void filter_books_by_title(int sockfd, int book_info_fd, char phrase[]);

void filter_books_by_author(int sockfd, int book_info_fd, char author[]);

int find_book(int book_info_fd, int book_id, struct book_info *book);