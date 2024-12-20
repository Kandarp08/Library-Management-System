This file contains basic information about the CUnit framework and the installation instructions.

<h2>Overview of Cunit</h2>

test case: It is a sequence of actions necessary to verify a specific functionality or feature of the software. It specifies the prerequisites, post conditions, steps, and data required for feature verification.

For instance, in a test scenario where you have to check login functionality, one of the test cases can be:

Test Case: Check results when a valid Login Id and Password are entered.

Test suites are the logical grouping or collection of test cases to run a single job with different test scenarios. 

For instance, a test suite for product purchase has multiple test cases, like:

Test Case 1: Login

Test Case 2: Adding Products

Test Case 3: Checkout

Test Case 4: Logout


A typical sequence of steps for using the CUnit framework is:

Write functions for tests (and suite init/cleanup if necessary).

Initialize the test registry - CU_initialize_registry()

Add suites to the test registry - CU_add_suite()

Add tests to the suites - CU_add_test()

Run tests using an appropriate interface, e.g. CU_console_run_tests
Cleanup the test registry - CU_cleanup_registry


Install 
-------------------------------------------------------------------
To use CUnit framework in your project, you must install it first

```sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev```

Then in your test.c

#include <CUnit/CUnit.h>

And finally, you must add the flag –lcunit to the gcc command (at the end)

```gcc  -o test test.c  -lcunit```

<h2>Mocking</h2>


Mocking is primarily used in unit testing. An object under test may have dependencies on other (complex) objects. To isolate the behaviour of the object you want to test you replace the other objects by mocks that simulate the behaviour of the real objects. This is useful if the real objects are impractical to incorporate into the unit test.

In short, mocking is creating objects that simulate the behaviour of real objects.


