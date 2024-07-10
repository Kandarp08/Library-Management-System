struct user_info
{
    int user_id;
    char username[30];
    char password[30];
    char user_type[10];
};

struct book_info
{
    int book_id;
    char title[30];
    char author[30];
    char isbn[13];
    int copies_available;
};

struct transaction_info
{
    int book_id;
    int user_id;
    int copies_borrowed;
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

void print_utils(char user_type[]);

void send_request_to_server(int sockfd, int query_num, int user_id, char user_type[]);