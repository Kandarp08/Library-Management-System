The Library Management System(LMS) is a terminal application that can be used for the management of a library. It contains various features to handle users, books, and their transactions. 

<h2>File Structure</h2>

The project contains the following directories:

1. data - Contains .dat files which store data regarding various users, books, and transactions.

2. include - Contains header files for the C programs.

3. obj - Contains the object files after compilation of the C programs.

4. src - Contains the source code in C language.

5. testing - Contains the source code for unit testing.

<h2>Setup Instructions</h2>

To setup and test the LMS locally, follow these instructions:

1. Fork the repository to your desired folder. Go to the root directory of the project.

2. Compile the server code by executing the command ```make server``` and the client code by ```make client```.

3. Start the server by ```./server```. In another terminal, start the client application by ```./client```.

4. From the client side, log into the sytem as an admin by entering the following credentials:

    Login as (admin/member): <b>admin</b>

    Enter username: <b>admin_username</b>

    Enter password: <b>admin_password</b>

5. You should see a list of features that are available to the admin. Add new admins or members using the feature "<b>Add new user</b>". These users can now use the application by logging in through their respective credentials.

<h2>Testing Instructions</h2>

Unit tests for the LMS are written using CUnit. After setting up the LMS, it can be unit tested in the following manner:

1. First, install CUnit using the instructions provided in the README inside the <b>testing</b> directory.

2. The source code for unit tests can be found in the <b>testing</b> directory.

3. To compile the unit tests, run the commmand ```make -f Makefile_test test_users test_books test_transactions``` in the root directory of the project.

4. To run the respective tests, use the commands

    ```./test_users```, ```./test_books```, and ```./test_transactions``` in the root directory of the project.