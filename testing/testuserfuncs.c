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

void test_admin_present()
{ // test if after init the main admin is present or not
	int fd = open("../data/user_info.dat", 0666);
	struct user_info clientinfo;
	// clientinfo.user_id = 1;
	strcpy(clientinfo.username, "admin_username");
	strcpy(clientinfo.password, "admin_password");
	strcpy(clientinfo.user_type, "admin");
	CU_ASSERT_EQUAL(authenticate_user(fd, &clientinfo), AUTHENTICATION_SUCCESS);
	strcpy(clientinfo.password, "uncle_john");
	CU_ASSERT_EQUAL(authenticate_user(fd, &clientinfo), AUTHENTICATION_FAILURE);
	close(fd);
}

void test_add_remove_member()
{ //
	int user_fd = open("../data/user_info.dat", 0666);
	int transactions_fd = open("../data/transactions.dat", 0666);
	struct user_info newuser;
	strcpy(newuser.username, "new_user");
	strcpy(newuser.password, "new_pass");
	strcpy(newuser.user_type, "member");
	int ret = add_user(user_fd, &newuser);
	printf("ret is %d\n", ret);
	CU_ASSERT_EQUAL(add_user(user_fd, &newuser), 1);
	// we know that in this scenario, the userid will be 2
	int user_id = 2;
	CU_ASSERT_EQUAL(find_user(user_fd, user_id), 0);
	CU_ASSERT_EQUAL(add_user(user_fd, &newuser), 1); // can't add the same user again
	CU_ASSERT_EQUAL(remove_user(user_fd, user_id, transactions_fd), 0);
	close(user_fd);
	close(transactions_fd);
}

int my_init_function()
{
	printf("started init\n");
	int user_info_fd = open("./data/user_info.dat", O_WRONLY);
	close(user_info_fd);
	user_info_fd = open("./data/user_info.dat", O_RDWR);
	printf("user file opened\n");
	int user_id = 1;
	write(user_info_fd, &user_id, sizeof(int));

	int book_info_fd = open("./data/book_info.dat", O_WRONLY);
	close(book_info_fd);
	book_info_fd = open("./data/book_info.dat", O_RDWR);
	printf("book file opened\n");
	int book_id = 1;
	write(book_info_fd, &book_id, sizeof(int));

	struct user_info main_admin;

	// Main admin credentials

	strcpy(main_admin.username, "admin_username");
	strcpy(main_admin.password, "admin_password");
	strcpy(main_admin.user_type, "admin");

	printf("copying done\n");
	add_user(user_info_fd, &main_admin);
	printf("user added\n");
	close(user_info_fd);
	close(book_info_fd);

	printf("Data files initialized successfully.\n");
	printf("Admin account added to database.\n\n");
	return 0;
}

int main()
{
	printf("running test\n");
	CU_initialize_registry();
	printf("initialized registry\n");

	// CU_pSuite CU_add_suite(const char* strName, CU_InitializeFunc pInit, CU_CleanupFunc pClean)
	CU_InitializeFunc pInit = my_init_function;
	CU_pSuite suite1 = CU_add_suite("auth_suite", pInit, NULL);
	printf("suite1 created\n");
	// Creates a new test collection (suite) having the specified name
	//, initialization function, and cleanup function.
	// These functions take no arguments and should return zero if they are completed successfully (non-zero otherwise)

	// CU_pTest CU_add_test(CU_pSuite pSuite, const char* strName, CU_TestFunc pTestFunc)
	CU_pTest test11 = CU_add_test(suite1, "admin_present", test_admin_present);
	CU_pTest test12 = CU_add_test(suite1, "add_member", test_add_remove_member);
	// Creates a new test having the specified name and test function, and registers it with the specified suite.
	// The suite must already have been created using CU_add_suite().
	// The current implementation does not support the creation of tests independent of a registered suite.
	// The test's name must be unique among all tests added to a single suite.
	// The test function cannot be NULL, and points to a function to be called when the test is run.
	// Test functions have neither arguments nor return values.
	// A pointer to the new test is returned.

	CU_basic_run_tests();
	CU_cleanup_registry();
	return 0;
}
