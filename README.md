This Library Management System (LMS) is my project submission for the course EG 301-P Operating Systems Lab. The entire project is written in C language.

Some features of this project are:

1. Modular - The source code is broken down into various modules, and each module is implemented in a separate file.

2. Extensible - It is very easy to add more functionalities, both on the server as well as the client side.

The project contains the following directories:

1. data - Contains .dat files which store data regarding various users, books, and transactions.

2. include - Contains header files for the C programs.

3. obj - Contains the object files after compilation of the C programs.

4. src - Contains the source code in C language.

To setup the LMS locally, follow these instructions:

1. Fork the repository to your desired folder. Create a directory named "obj" in the root directory.

2. In the init() function of src/server.c, create the credentials of the main admin.

3. Run the command ```make server client``` in the root directory of the project.

4. Start the server by ```./server```. Terminate the process after initialization is successful.

5. Comment out the call to init() function in src/server.c and run ```make server``` again in the root directory of the project.

6. Start the server in one terminal through ```./server``` and the client in another terminal using ```./client```.

7. Login from the client terminal through the created credentials. All functionalities can now be used.