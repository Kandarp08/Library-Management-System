struct user_info
{
    int user_id;
    char username[30];
    char password[30];
    char user_type[10];
};

#define AUTHENTICATION_SUCCESS 0
#define AUTHENTICATION_FAILURE 1

int authenticate_user(int user_info_fd, struct user_info *client_info);

int add_user(int user_info_fd, struct user_info *client_info);

int remove_user(int user_info_fd, int user_id);

int modify_user(int user_info_fd, struct user_info *client_info);

void show_all_users(int sockfd, int user_info_fd);

int find_user(int user_info_fd, int user_id);