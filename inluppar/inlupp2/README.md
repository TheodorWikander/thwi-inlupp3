# Assignment 2: Webstore

## Build and test
make test       # build and run unit tests
make memtest    # build and run tests under valgrind
make run        # build and run the program
make stdrun     # build and run the program with webstoretest.txt as input file           
make stdmemtest # build and run program with webstoretest.txt as input file under valgrind
make coverage   # produces coverage report (gcov)
make clean      # removes files built by makefile commands

Requires gcc, make, CUnit (libcunit) and Valgrind

## Coverage:
Measured with gcov ('make coverage') on the unit tests. Line coverage is "lines excecuted" and branch coverage is "Taken at least once"

store.c
    - line coverage: 99.69%
    - branch coverage: 85.62%

hash_table.c 
    - line coverage: 91.78%
    - branch coverage: 75%

iterator.c
    - line coverage: 97.14%
    - branch coverage: 78.57%

linked_list.c
    - line coverage: 67.65%
    - branch coverage: 35%

list_iterator.c
    - line coverage: 67.86%
    - branch coverage: 50%

utils.c
    - line coverage: 30.14%
    - branch coverage: 31.25%

The user interface (ui/ and main.c) is not unit tested as stated in the assignment. It is checked by running webstoretest.txt ('make stdrun' and 'make stdmemrun')

## Coverage comment
The store only uses part of the linked_list API. A magnitude of functions from linked_list such as 'list_head', 'list_last', 'list_prepend' and insert/remove are never used by our program which explains the lower numbers for linked_list.c and list_iterator.c. The data structures come with their own tests. 

The remaining branches in store.c are mostly assert statements, build to crash the program if a precondition is broken or individual parts of combined conditions. 

The input functions in utils.c are not part of the unit tests, except for one simple test to test the hash function added last minute. They rest are exercised by running 'webstoretest.txt'.

The user interface is not unit tested, as stated in the assignment. It is checked with 'make stdrun' and 'make stdmemtest'

## Usage
    ./store_app

The menu is shown at start. Action codes are displayed by the menu (For example [A]) and valid action codes are: A L D E S P C R + - = O Q. Both upper and lower case characters are acepted as action codes. no files are needed and data is not saved between runs. 

## Design 
Modules: generic_structures/, generic_utils/, store/ (webstore logic), ui/ (text interface), store_tests/ (CUnit)
- merch_db: name -> merch; shelf_db: shelf -> merch; carts_db: id -> cart.
- The store owns all memory; strings are copied on the way in.
- Checkout takes items from shelves in alphabetical order and removes empty shelves.
- Removing or renaming a merchandise updates all carts.
- All carts can never hold more than the total stock for the merchandise so that every cart can always be checked out

## Decisions and known limitations
- merch_stock returns 0 for an unknown merchandise; cart_cost returns -1 for an unknown cart.
- Prices are int and must be >= 0.
- Undo and persistence are not implemented.
- `ask_question_int` does not detect numbers outside the range of `int`.
- If input ends in the middle of a question the program exits with an error
  message without freeing the store (so Valgrind reports leaks in that case only).
- `ioopm_list_iterator_remove` is an unimplemented stub (documented in its header).
- See more under "edits to utils"
- Empty input is rejected and asked again for all strings (names, descriptions and
  confirmations), since `ask_question_string` requires a non-empty line.
- Removing 0 items from a cart succeeds and changes nothing.
- The hash table has a fixed number of buckets (17), and `ioopm_store_merch_name_at`sorts on every call, so listing is slow for very large numbers of merchandise.
- Sums that overflow `int` (e.g. the cost of a cart) are not detected.

## Component sources
- Hash table: Nova's Assignment 1
- Linked list: Nova's Assignment 1
- Utils: Victor's C bootstrap labs 

## Edits to Utils
- Added `string_hash`, `string_eq`, `int_hash` and `int_eq` (needed by the hash table).
- Added the field `char_value` to the `answer_t` union, and used it in `ask_question_char`.
- `ask_question` now exits with an error message if input ends before an
  acceptable answer was read, instead of looping forever.
- Removed `is_shelf` and `ask_question_shelf` (see below).

## Shelf name validition

The shelf format (one capital letter A-Z followed by two digits) is checked in two places on purpose:

- `store.c` (`ioopm_store_replenish`) is the safety net. The backend rejects
  invalid shelf names no matter who calls it, so it can never store an invalid shelf.
- `ui.c` (static functions) checks the format right after the input is read
  and asks again, as the assignment requires for format errors. This keeps the
  input lines in sync with the actions' expected order.

The two functions came from the two original code bases (utils from Victor, the
store from Nova), which were written separately. They were kept as separate
layers instead of merged, and moved out of `utils` to keep the generic modules separate and unknowing about the webstores structure as stated in the assignment.
