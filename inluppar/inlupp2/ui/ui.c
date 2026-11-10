#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "ui.h"
#include "../store/store.h"
#include "../generic_utils/utils.h"

/// @brief Checks the shelf name format (one capital letter A-Z followed by two digits)
/// @return True if theinput string is of a valid format, false outhervise
static bool is_shelf(char *str)
{
  if (strlen(str) != 3)
  {
    return false;
  }
  if (str[0] < 'A' || str[0] > 'Z')
  {
    return false;
  }
  return isdigit(str[1]) && isdigit(str[2]);
}

/// @brief Asks for a shelf name, repeating until is_shelf accepts the input
/// @param question the text to print
/// @return a newly allocated string (strdup), e.g. "A25". The caller must free it.
static char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, to_string).string_value;
}

/// @return true if the input is one of the valid action codes (upper and lower case are both accepted)
static bool is_menu_choice(char *s) {
    return strlen(s) >= 1 
        && strchr("ALDESPCR+-=OUQ", toupper((unsigned char) s[0])) != NULL;
}

/// @brief asks a yes/no question. Only answer starting with Y or y count as yes, all others as no
/// @return true if first character in answe is Y or y, false otherwise
static bool confirmed(char *question) {
    char *answer = ask_question_string(question);
    bool yes = (answer[0] == 'Y' || answer[0] == 'y');
    free(answer);
    return yes;
}

/// @brief Asks user whether to continue listing. Eveything that does NOT start with n/N will continue listing
/// @return False is the answer starts with N/n, true otherwise
static bool continue_listing(void) {
    char buf[255];
    println("Continue listing? (Enter = continue , n = back to menu)");
    read_string(buf, 255);
    return !(buf[0] == 'N' || buf[0] == 'n');
}

/// @brief Action A: add merchandise
static void ui_add_merch(store_t *store) {
    char *name = ask_question_string("Name: ");
    char *desc = ask_question_string("Description: ");
    int price = ask_question_int("Price: ");

    if (ioopm_store_add_merch(store, name, desc, price)) {
        println("Merchandise added.");
    }
    else {
        println("Could not add merchandise (duplicate name or negative price)");
    }
    free(name);
    free(desc);
}

/// @brief Action L: list merchandise
static void ui_list_merch(store_t *store) {
    int count = (int) ioopm_store_merch_count(store);
    if (count == 0) {
        println("No merchandise.");
        return;
    }
    for (int i = 0; i < count; ++i) {
        char *name = ioopm_store_merch_name_at(store, i);
        printf("%d. %s\n", i + 1, name);
        if ((i+1) % 20 == 0 && i+1 < count) {
            if (!continue_listing()) return;
        }
    }
}
/// @brief Action s: show stock for a merchandise
static void ui_show_stock(store_t *store) {
    char *name = ask_question_string("Name: ");

    if (!ioopm_store_has_merch(store, name)) {
        println ("No such merchandise");
    } else {
        int shelves = ioopm_store_shelf_count(store, name);
        if (shelves == 0) println ("No stock");
        for (int i = 0; i < shelves; ++i) {
            printf("%s: %d\n", ioopm_store_shelf_name(store, name, i),
                                ioopm_store_shelf_quantity(store, name, i));
        }
    }
    free(name);
}

/// @brief Action D: remove merchandise (and ask for confirmation)
static void ui_remove_merch(store_t *store) {
    char *name = ask_question_string("Name: ");
    bool yes = confirmed("Are you sure you want to remove it? (y/n)");

    if (!yes) {
        println("Action cancelled");
    } else if (ioopm_store_remove_merch(store, name)) {
        println("Merchandise removed");
    } else {
        println("No such merchandise");
    }
    free(name);
}

/// @brief Action E: edit merchandise (and ask for confirmation beforehand)
static void ui_edit_merch(store_t *store) {
    char *old_name = ask_question_string("Old name: ");
    char *new_name = ask_question_string("New name: ");
    char *new_desc = ask_question_string("New description: ");
    int new_price = ask_question_int("New price: ");

    bool yes = confirmed("Are you sure you want to edit merchandise? (y/n)");

    if (!yes) {
        println("Action cancelled");
    } else if (ioopm_store_edit_merch(store, old_name, new_name, new_desc, new_price)) {
        println("Merchandise edited");
    } else {
        println("Could not edit (no such merchandise, new name taken or negative price)");
    }
    free(old_name);
    free(new_name);
    free(new_desc);
}

/// @brief Action P: replenis shelf with a merchandise
static void ui_replenish(store_t *store) {
    char *shelf = ask_question_shelf("Shelf: ");
    char *name = ask_question_string("Name: ");
    int amount = ask_question_int("Amount: ");

    if (ioopm_store_replenish(store, shelf, name, amount)) {
        println("Stock increased");
    } else {
        println("Could not replenish (no such merchandise, invalid or used shelf or amount < 1)");
    }
    free(shelf);
    free(name);
}

/// @brief Action C: create cart
static void ui_create_cart(store_t *store) {
    int id = ioopm_store_cart_create(store);
    printf("Cart created with id %d.\n", id);
}

/// @brief Action R: remove cart
static void ui_remove_cart(store_t *store) {
    int id = ask_question_int("Cart id: ");
    bool yes = confirmed("Are you sure you want to remove cart? (y/n)");

    if (!yes) {
        println("Action cancelled");
    } else if (ioopm_store_cart_remove(store, id)) {
        printf("Cart %d. removed\n", id);
    } else {
        println("No such cart");
    }
}

/// @brief Action +: Add item to cart
static void ui_add_to_cart(store_t *store) {
    int id = ask_question_int("Cart id: ");
    char *name = ask_question_string("Name: ");
    int amount = ask_question_int("Amount: ");

    if (ioopm_store_cart_add(store, id, name, amount)) {
        printf("Succesfully added %d %s(s) to cart\n", amount, name);
    } else {
        printf("Could not add to cart (no such cart, no such merch, amount < 1 or amount > stock)\n");
    }
    free(name);
}

/// @brief Action -: Remove item from cart
static void ui_remove_from_cart(store_t *store) {
    int id = ask_question_int("Cart id: ");
    char *name = ask_question_string ("Name: ");
    int amount = ask_question_int("Amount: ");

    if (ioopm_store_cart_remove_item(store, id, name, amount)) {
        println("Successfully removed from cart");
    } else {
        println("Could not remove from cart (no such cart/merch, not in cart, amount < 1 or amount > amount in cart)");
    }
    free(name);
}

/// @brief Action =: Calculate the cost of a cart
static void ui_cart_cost(store_t *store) {
    int id = ask_question_int("Cart id: ");
    int cost = ioopm_store_cart_cost(store, id);

    if (cost < 0) {
        println("No such cart");
    } else {
        printf("Cost: %d\n", cost);
    }
}

/// @brief Action O: Checkout cart
static void ui_checkout(store_t *store) {
    int id = ask_question_int("Cart id: ");
    if (ioopm_store_checkout(store, id)) {
        println("Checkout complete");
    } else {
        println("No such cart");
    }
}


static void print_menu(void) {
    println("");
    println("[A]dd merchandise");
    println("[L]ist merchandise");
    println("[D]elete merchandise");
    println("[E]dit merchandise");
    println("[S]how stock");
    println("[P]replenish");
    println("[C]reate cart");
    println("[R]emove cart");
    println("[+] Add to cart");
    println("[-] Remove from cart");
    println("[=] Calculate cost");
    println("[O] Checkout");
    println("[U]ndo");
    println("[Q]uit");
    println("");
}

void ioopm_ui_run(void) {
    store_t *store = ioopm_store_create();
    bool running = true;

    while (running) {
        print_menu();
        char choice = ask_question_char("Choose an action: \n", is_menu_choice);

        switch(toupper((unsigned char) choice)) {
            case 'Q':
                if (confirmed("Are you sure you want to quit: (y/n)")) running = false;
                break;
            case 'A': ui_add_merch(store); break;
            case 'L': ui_list_merch(store); break;
            case 'S': ui_show_stock(store); break;
            case 'D': ui_remove_merch(store); break;
            case 'E': ui_edit_merch(store); break;
            case 'P': ui_replenish(store); break;
            case 'C': ui_create_cart(store); break;
            case 'R': ui_remove_cart(store); break;
            case '+': ui_add_to_cart(store); break;
            case '-': ui_remove_from_cart(store); break;
            case '=': ui_cart_cost(store); break;
            case 'O': ui_checkout(store); break;

            default:
                println("Not implemented yet");
                break;
        }
    }
    ioopm_store_destroy(store);
}







