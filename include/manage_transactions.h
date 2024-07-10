struct transaction_info
{
    int book_id;
    int user_id;
    int copies_borrowed;
};

int issue_book(int user_info_fd, int book_info_fd, int transactions_fd, int book_id, int user_id, int copies);

int return_book(int book_info_fd, int transactions_fd, int book_id, int user_id);

void show_user_transactions(int transactions_fd, int sockfd, int user_id);

void show_books_to_be_returned(int transactions_fd, int sockfd);