struct file_fd
{
    int user_info_fd;
    int book_info_fd;
    int transactions_fd;
};

#define ADD_BOOK 1
#define REMOVE_BOOK 2
#define MODIFY_BOOK 3

#define ADD_USER 4
#define DELETE_USER 5
#define SHOW_ALL_USERS 6
#define SHOW_USER_TRANSACTIONS 7
#define SHOW_ALL_BOOKS_TO_BE_RETURNED 8

#define SHOW_ALL_BOOKS 9
#define FILTER_BOOKS_BY_TITLE 10
#define FILTER_BOOKS_BY_AUTHOR 11
#define ISSUE_BOOK 12
#define RETURN_BOOK 13
#define MODIFY_USER 14

void serve_client_request(int sockfd, struct file_fd *file_fds, int query_num);