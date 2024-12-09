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

void test_admin_present()
{ // test if after init the main admin is present or not
	// init done before this test already
	//  my_init_function()
	int fd = open("./data/user_info.dat", O_RDWR);
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

void test_add_remove_user()
{						//
	my_init_function(); // reinit everything for the new test
	int user_fd = open("./data/user_info.dat", O_RDWR);
	int transactions_fd = open("./data/transactions.dat", O_RDWR);
	struct user_info newuser;
	strcpy(newuser.username, "new_user");
	strcpy(newuser.password, "new_pass");
	strcpy(newuser.user_type, "member");
	int ret = add_user(user_fd, &newuser);
	CU_ASSERT_EQUAL(add_user(user_fd, &newuser), 1);
	// we know that in this scenario, the userid will be 2
	int user_id = 2;
	CU_ASSERT_EQUAL(find_user(user_fd, user_id), 0);
	CU_ASSERT_EQUAL(add_user(user_fd, &newuser), 1); // can't add the same user again
	CU_ASSERT_EQUAL(remove_user(user_fd, user_id, transactions_fd), 0);
	CU_ASSERT_EQUAL(find_user(user_fd, user_id), 1);				  // can't find user after removal
	CU_ASSERT_NOT_EQUAL(remove_user(user_fd, 1, transactions_fd), 0); // can't remove main admin
	close(user_fd);
	close(transactions_fd);
}

void test_show_all_users()
{
	my_init_function(); // reinitialize everything
	int user_fd = open("./data/user_info.dat", O_RDWR);
	int pipearr[2];
	pipe(pipearr);
	int pipein = pipearr[0];  // read end
	int pipeout = pipearr[1]; // write end
	struct user_info newuser;
	strcpy(newuser.username, "new_user");
	strcpy(newuser.password, "new_pass");
	strcpy(newuser.user_type, "member");
	add_user(user_fd, &newuser);
	show_all_users(pipeout, user_fd);
	struct user_info user;
	read(pipein, &user, sizeof(struct user_info));

	CU_ASSERT_EQUAL(user.user_id, 1); // first id must be 1
	read(pipein, &user, sizeof(struct user_info));
	CU_ASSERT_EQUAL(user.user_id, 2); // 2nd id must be 2
	read(pipein, &user, sizeof(struct user_info));
	CU_ASSERT_EQUAL(user.user_id, -1);
}

void test_modify_user()
{
	my_init_function();
	int user_fd = open("./data/user_info.dat", O_RDWR);
	struct user_info newuser;
	strcpy(newuser.username, "new_user");
	strcpy(newuser.password, "new_pass");
	strcpy(newuser.user_type, "member");
	CU_ASSERT_EQUAL(add_user(user_fd, &newuser), 0);
	strcpy(newuser.username, "pratster");
	CU_ASSERT_EQUAL(modify_user(user_fd, &newuser), 0);
	int pipearr[2];
	pipe(pipearr);
	int pipein = pipearr[0];  // read end
	int pipeout = pipearr[1]; // write end
	
	show_all_users(pipeout, user_fd);
	
	struct user_info user;
	read(pipein, &user, sizeof(struct user_info));
	read(pipein, &user, sizeof(struct user_info));

	CU_ASSERT_EQUAL(strcmp(user.username, "pratster"), 0);
	close(user_fd);
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

	int transactions_fd = open("./data/transaction.dat", O_WRONLY | O_TRUNC);
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

	// printf("init done\n");
	return 0;
}

int main()
{
	printf("running test\n");
	CU_initialize_registry();
	printf("initialized registry\n");

	// CU_pSuite CU_add_suite(const char* strName, CU_InitializeFunc pInit, CU_CleanupFunc pClean)
	CU_InitializeFunc pInit = my_init_function;
	CU_pSuite suite1 = CU_add_suite("users_suite", pInit, pInit);
	printf("suite1 created\n");
	// Creates a new test collection (suite) having the specified name
	//, initialization function, and cleanup function.
	// These functions take no arguments and should return zero if they are completed successfully (non-zero otherwise)
	// -- from documentation of CUnit

	// CU_pTest CU_add_test(CU_pSuite pSuite, const char* strName, CU_TestFunc pTestFunc)
	CU_pTest test11 = CU_add_test(suite1, "admin_present", test_admin_present);
	CU_pTest test12 = CU_add_test(suite1, "add_remove_user", test_add_remove_user);
	CU_pTest test13 = CU_add_test(suite1, "show_all_users", test_show_all_users);
	CU_pTest test14 = CU_add_test(suite1, "modify_user", test_modify_user);

	// Creates a new test having the specified name and test function, and registers it with the specified suite.
	// The suite must already have been created using CU_add_suite().
	// The current implementation does not support the creation of tests independent of a registered suite.
	// The test's name must be unique among all tests added to a single suite.
	// The test function cannot be NULL, and points to a function to be called when the test is run.
	// Test functions have neither arguments nor return values.
	// A pointer to the new test is returned. -- from documentation

	CU_basic_run_tests();
	CU_cleanup_registry();
	printf("user tests done\n");
	return 0;
}
