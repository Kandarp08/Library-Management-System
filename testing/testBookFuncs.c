#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/manage_books.h"
int my_init_function();
void test_add_book()
{   my_init_function();


    int book_info_fd = open("./data/book_info.dat", O_RDWR);
    if (book_info_fd < 0)
    {
        perror("open failed for './data/book_info.dat'");
        exit(1);
    }

    
    struct book_info new_book;
    strcpy(new_book.title, "Gulliver Travels");
    strcpy(new_book.author, "Jonathan Swift");
    strcpy(new_book.isbn, "00001");
    new_book.copies_available = 5;

    struct book_info retrieved_book;
    int ret  = find_book(book_info_fd, 2, &retrieved_book);
    // printf("the return value is %d\n", ret);
    add_book(book_info_fd, &new_book);
    ret  = find_book(book_info_fd, 2, &retrieved_book);
    if(ret){
        // printf("the return value is %d\n", ret);
        exit(0);
    }

    // // printf("mismatch retrieved: %d, new: %d\n",  retrieved_book.book_id, new_book.book_id);
    // CU_ASSERT_EQUAL(retrieved_book.book_id, new_book.book_id);
    // printf("mismatch retrieved: %s, new: %s\n",  retrieved_book.title, new_book.title);
    CU_ASSERT_STRING_EQUAL(retrieved_book.title, new_book.title);
    CU_ASSERT_STRING_EQUAL(retrieved_book.author, new_book.author);
    CU_ASSERT_STRING_EQUAL(retrieved_book.isbn, new_book.isbn);
    CU_ASSERT_EQUAL(retrieved_book.copies_available, new_book.copies_available);

    // lseek(book_info_fd, sizeof(int), SEEK_SET);
    // struct book_info debug_book;
    // read(book_info_fd, &debug_book, sizeof(struct book_info));
    // // printf("title: %s %d\n", debug_book.title, debug_book.book_id);
    // read(book_info_fd, &debug_book, sizeof(struct book_info));
    // // printf("title: %s %d\n", debug_book.title, debug_book.book_id);
    close(book_info_fd);
    // printf("test_add_book completed\n");
}

void test_find_book()
{   my_init_function();
    int book_info_fd = open("./data/book_info.dat", O_RDWR);
    if (book_info_fd < 0)
    {
        perror("open failed for './data/book_info.dat'");
        exit(1);
    }

    struct book_info retrieved_book;
    int book_id_to_find = 1;

    int result = find_book(book_info_fd, book_id_to_find, &retrieved_book);

    CU_ASSERT_EQUAL(result, 0); 
    // // printf("findmismatch retrieved %d. book id to find %d\n", retrieved_book.book_id, book_id_to_find);
    // CU_ASSERT_EQUAL(retrieved_book.book_id, book_id_to_find);
    char str[40] = "The Invisible Man";
    // printf("retrieved %s. book id to find %s\n", retrieved_book.title, str);
    CU_ASSERT_STRING_EQUAL(retrieved_book.title, "The Invisible Man");
    lseek(book_info_fd, sizeof(int), SEEK_SET);
    struct book_info debug_book;
    read(book_info_fd, &debug_book, sizeof(struct book_info));
    // printf("---title: %s %d\n", debug_book.title, debug_book.book_id);
    // read(book_info_fd, &debug_book, sizeof(struct book_info));
    // // printf("---title: %s %d\n", debug_book.title, debug_book.book_id);

    close(book_info_fd);
    // printf("test_find_book completed\n");
}

int my_init_function()
{
   
    int book_info_fd = open("./data/book_info.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (book_info_fd < 0)
    {
        perror("failed to initialize book_info.dat");
        return -1;
    }

    int book_id = 1; 
    int val = 2;
    lseek(book_info_fd, 0, SEEK_SET);
    write(book_info_fd, &val, sizeof(int));

    struct book_info initial_book;
    initial_book.book_id = book_id;
    strcpy(initial_book.title, "The Invisible Man");
    strcpy(initial_book.author, "H.G. WELLS");
    strcpy(initial_book.isbn, "00000");
    initial_book.copies_available = 5;

    lseek(book_info_fd, 0, SEEK_END);
    write(book_info_fd, &initial_book, sizeof(struct book_info));

    close(book_info_fd);

    // printf("my_init_function completed\n");
    return 0;
}
int my_cleanup_function(){
    int fd = open("./data/book_info.dat", O_WRONLY | O_TRUNC, 0644);
    if( fd == -1 ) return 1;
    //write a 1 b4 closing
    int val=1;
    write(fd,&val,sizeof(int));
    close(fd);
    return 0;
}

int main()
{
    CU_initialize_registry();
    CU_InitializeFunc pInit = my_init_function;
    CU_CleanupFunc pClean = my_cleanup_function;
    CU_pSuite suite = CU_add_suite("book management suite",pInit,pClean);
    CU_add_test(suite, "test find book", test_find_book);
    CU_add_test(suite, "test add book", test_add_book);
    

   
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
