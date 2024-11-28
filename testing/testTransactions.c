#include <stdio.h>
#include <stdlib.h>
#include "../include/manage_users.h"
#include "../include/manage_transactions.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int my_init_function();





void test_show_user_transactions() {
   
   
    my_init_function(); // Initialize test environment

    int transactions_fd = open("./data/transactions.dat", O_RDWR);


    // Check if the file was opened successfully
    if (transactions_fd == -1) {
        perror("Error opening transactions file");
        // Handle the error, e.g., exit the test or log the error
        exit(1); // Or use an appropriate error handling mechanism
    } else {
        // printf("Transactions file opened successfully.\n");
    }

    int pipearr[2];
    pipe(pipearr);
    int pipein = pipearr[0];  // Read end
    int pipeout = pipearr[1]; // Write end

    // Sample user transactions data to test with
    struct transaction_info transactions = {1, 1, 1};
    write(transactions_fd, &transactions, sizeof(struct transaction_info));

    show_user_transactions(transactions_fd, pipeout, 1);

    struct transaction_info read_transaction;
    read(pipein, &read_transaction, sizeof(struct transaction_info));

    // Assert the transactions belongs to user 1
    CU_ASSERT_EQUAL(read_transaction.user_id, 1);

    close(transactions_fd);
}

void test_show_books_to_be_returned() {
    my_init_function();

    int transactions_fd = open("./data/transactions.dat", O_RDWR);


    // Check if the file was opened successfully
    if (transactions_fd == -1) {
        perror("Error opening transactions file");
        // Handle the error, e.g., exit the test or log the error
        exit(1); // Or use an appropriate error handling mechanism
    } else {
        // printf("Transactions file opened successfully.\n");
    }

    int pipearr[2];
    pipe(pipearr);
    int pipein = pipearr[0];  // Read end
    int pipeout = pipearr[1]; // Write end

    // Sample transactions with books borrowed
    struct transaction_info transaction1 = {1, 1, 1};
    struct transaction_info transaction2 = {2, 1, 2};
    // printf("befoer wirte\n");
    write(transactions_fd, &transaction1, sizeof(struct transaction_info));
    write(transactions_fd, &transaction2, sizeof(struct transaction_info));

    // printf("after wirte\n");
    show_books_to_be_returned(transactions_fd, pipeout);
    // printf("after show_Books\n");

    struct transaction_info read_transaction;
    int count = 0;
    int k = lseek(transactions_fd,0,SEEK_SET);

    while (read(pipein, &read_transaction, sizeof(struct transaction_info)) > 0) {
        if(read_transaction.user_id == -1) break;
        CU_ASSERT_EQUAL(read_transaction.user_id, 1);
        // printf("inside loop\n");
        count++;
    }
    // printf("loop done\n");
    // Assert two books need to be returned
    // printf("the count is %d\n",count);
    CU_ASSERT_EQUAL(count, 2);

    close(transactions_fd);
    // printf("Done\n");
}

void test_is_book_borrowed() {
    my_init_function();

    int transactions_fd = open("./data/transactions.dat", O_RDWR);


    // Check if the file was opened successfully
    if (transactions_fd == -1) {
        perror("Error opening transactions file");
        // Handle the error, e.g., exit the test or log the error
        exit(1); // Or use an appropriate error handling mechanism
    } else {
        // printf("Transactions file opened successfully.\n");
    }

    // Add a transactions for a borrowed book
    struct transaction_info transactions = {1, 1, 1};
    write(transactions_fd, &transactions, sizeof(struct transaction_info));

    // Assert book with ID 1 is borrowed
    CU_ASSERT_TRUE(is_book_borrowed(transactions_fd, 1));

    // Assert book with ID 2 is not borrowed
    CU_ASSERT_FALSE(is_book_borrowed(transactions_fd, 2));

    close(transactions_fd);
}

void test_has_borrowed_book() {
    my_init_function();

    int transactions_fd = open("./data/transactions.dat", O_RDWR);


    // Check if the file was opened successfully
    if (transactions_fd == -1) {
        perror("Error opening transactions file");
        // Handle the error, e.g., exit the test or log the error
        exit(1); // Or use an appropriate error handling mechanism
    } else {
        // printf("Transactions file opened successfully.\n");
    }

    // Add transactions for a user
    struct transaction_info transaction1 = {1, 1, 1};
    struct transaction_info transaction2 = {2, 2, 1};

    // printf("about to write\n");
    write(transactions_fd, &transaction1, sizeof(struct transaction_info));
    write(transactions_fd, &transaction2, sizeof(struct transaction_info));
    // printf("written\n");
    // Assert user 1 has borrowed a book
    CU_ASSERT_TRUE(has_borrowed_book(transactions_fd, 1));

    // Assert user 3 has not borrowed any book
    CU_ASSERT_FALSE(has_borrowed_book(transactions_fd, 3));

    close(transactions_fd);
}



int my_init_function()
{
	int user_info_fd = open("./data/user_info.dat", O_WRONLY | O_TRUNC);


   

	if (user_info_fd == -1)
	{
		perror("error in user file open\n");
	}
	close(user_info_fd);
	user_info_fd = open("./data/user_info.dat", O_RDWR);
	int user_id = 1;
	write(user_info_fd, &user_id, sizeof(int));

	int book_info_fd = open("./data/book_info.dat", O_WRONLY | O_TRUNC);
	close(book_info_fd);

	int transactions_fd = open("./data/transactions.dat", O_WRONLY | O_TRUNC);
	close(transactions_fd);

	book_info_fd = open("./data/book_info.dat", O_RDWR);
	int book_id = 1;
	write(book_info_fd, &book_id, sizeof(int));

	struct user_info main_admin;

	// Main admin credentials

	strcpy(main_admin.username, "admin_username");
	strcpy(main_admin.password, "admin_password");
	strcpy(main_admin.user_type, "admin");

	add_user(user_info_fd, &main_admin);
	close(user_info_fd);
	close(book_info_fd);

	// // printf("init done\n");
	return 0;
}

int main()
{
    // printf("Running tests\n");
    if (CU_initialize_registry() != CUE_SUCCESS) {
        fprintf(stderr, "Failed to initialize CUnit registry\n");
        return CU_get_error();
    }
    // printf("CUnit registry initialized\n");

    // // Suite for user-related functionalities
    // CU_pSuite users_suite = CU_add_suite("users_suite", my_init_function, my_init_function);
    // if (users_suite == NULL) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    // // printf("Users suite created\n");

    // // Add user-related tests to the suite
    // if ((CU_add_test(users_suite, "admin_present", test_admin_present) == NULL) ||
    //     (CU_add_test(users_suite, "add_remove_user", test_add_remove_user) == NULL) ||
    //     (CU_add_test(users_suite, "show_all_users", test_show_all_users) == NULL) ||
    //     (CU_add_test(users_suite, "modify_user", test_modify_user) == NULL)) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // Suite for transactions-related functionalities
    CU_pSuite transactions_suite = CU_add_suite("transactions_suite", my_init_function, my_init_function);
    if (transactions_suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    // printf("Transactions suite created\n");

    // Add transactions-related tests to the suite
    if ((CU_add_test(transactions_suite, "show_user_transactions", test_show_user_transactions) == NULL) ||
        (CU_add_test(transactions_suite, "show_books_to_be_returned", test_show_books_to_be_returned) == NULL) ||
        (CU_add_test(transactions_suite, "is_book_borrowed", test_is_book_borrowed) == NULL) ||
        (CU_add_test(transactions_suite, "has_borrowed_book", test_has_borrowed_book) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run tests
    CU_basic_set_mode(CU_BRM_VERBOSE); // Verbose mode for detailed output
    CU_basic_run_tests();

    // Cleanup
    CU_cleanup_registry();
    // printf("Tests completed\n");
    return 0;
}
