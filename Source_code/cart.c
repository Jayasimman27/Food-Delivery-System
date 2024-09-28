#include "headers.h"

/* typedef struct {
    char restaurant_name[50];
    char item_names[50][50];
    int gst;
    int price[50];
    int qty[50];
    int item_total[50];
    int item_count;
} cart_details; */

int save_cart(cart_details* cart) {
    FILE *cart_file = fopen("cart.csv", "w");
    if (!cart_file) {
        printf("Error opening the cart file!\n");
        return 1;
    }

    fprintf(cart_file, "%s,%.1f\n", cart->restaurant_name, (float)cart->gst);
    for (int i = 0; i < cart->item_count; i++) {
        fprintf(cart_file, "%s,%d,%d\n", cart->item_names[i], cart->price[i], cart->qty[i]);
    }
    fclose(cart_file);
}

int change_qty(cart_details* cart, cart_details carts) {
    char item_name[50];
    int new_qty;

    printf("Enter the item name to change quantity: ");
    fgets(item_name, sizeof(item_name), stdin);
    if (item_name[strlen(item_name) - 1] == '\n') {
        item_name[strlen(item_name) - 1] = '\0';
    }

    printf("\nEnter the new quantity: ");
    scanf("%d", &new_qty);
    clear_input_buffer();

    int found = 0;
    for (int i = 0; i < cart->item_count; i++) {
        if (strcasecmp(item_name, cart->item_names[i]) == 0) {
            cart->qty[i] = new_qty;
            cart->item_total[i] = cart->price[i] * new_qty;
            found = 1;
            break;
        }
    }
    //save_cart(&carts);
    if (!found) {
        printf("Item not found in the cart\n");
        return 1;
    }
}


int delete_items_cart(cart_details* cart, cart_details carts) {
    char del[50];
    printf("\nEnter the food item to delete: ");
    fgets(del, sizeof(del), stdin);

    if (del[strlen(del) - 1] == '\n') {
        del[strlen(del) - 1] = '\0';
    }

    for (int i = 0; i < cart->item_count; i++) {
        if (strcasecmp(del, cart->item_names[i]) == 0) {
            // Shift remaining items to fill the gap
            for (int j = i; j < cart->item_count - 1; j++) {
                strcpy(cart->item_names[j], cart->item_names[j + 1]);
                cart->price[j] = cart->price[j + 1];
                cart->qty[j] = cart->qty[j + 1];
                cart->item_total[j] = cart->item_total[j + 1];
            }
            cart->item_count--;
            printf("%s removed from the cart successfully\n", del);
            return 0;
        }
    }
    //save_cart(&carts);
    printf("Item not found in the cart\n");
    return 1;
}

int add_items(MenuItem menu_items[], int total_items, cart_details* cart, cart_details carts) {
    char item[50];
    printf("\nEnter the item name to be added from the menu: ");
    fgets(item, sizeof(item), stdin);

    if (item[strlen(item) - 1] == '\n') {
        item[strlen(item) - 1] = '\0';
    }

    for (int i = 0; i < total_items; i++) {
        if (strcasecmp(item, menu_items[i].item_name) == 0) {
            int qty = 1;
            printf("Enter the total quantity you want to buy: ");
            scanf("%d", &qty);
            clear_input_buffer();

            strcpy(cart->item_names[cart->item_count], menu_items[i].item_name);
            cart->price[cart->item_count] = menu_items[i].price;
            cart->qty[cart->item_count] = qty;
            cart->item_total[cart->item_count] = menu_items[i].price * qty;
            cart->item_count++;
            
            printf("%s Added to the cart successfully\n", item);
            return 0;
        }
    }
    printf("%s",cart->item_names[cart->item_count]);
    //save_cart(&carts);
    printf("Item not found in the menu\n");
    return 1;
}

int display_total_items(MenuItem menu_items[],int total_items){
    printf("The Total Items available in the menu!");
    printf("Menu:-\n");
    for (int i = 0; i < total_items; i++){
        printf("%s - %d\n",menu_items[i].item_name,menu_items[i].price);
    }
}

int view_cart(cart_details* cart) {
    if (cart->item_count == 0) {
        printf("The cart is empty.\n");
        return 1;
    }

    printf("\n\t\t Cart \n");
    for (int i = 0; i < cart->item_count; i++) {
        printf("Item: %s, Price: %d, Quantity: %d, Total: %d\n",
               cart->item_names[i], cart->price[i], cart->qty[i], cart->item_total[i]);
    }
    printf("\n");
    return 0;
}

void display_menu() {
    printf("\n====== Menu ======\n");
    printf("1. Add Item to Cart\n");
    printf("2. Delete Item from Cart\n");
    printf("3. Change Quantity of Item\n");
    printf("4. Display Menu Items\n");
    printf("5. View Cart\n");
    printf("6. To Confirm and proceed for Bill Generation..\n");
    printf("==================\n");
}

int cart_detail(MenuItem menu_items[], int total_items, float ratings, int tot_person, char restaurantname[50], Restaurant restaurants[],int num_restaurants, char city[50]) {
    FILE *fp;
    char line[256];
    cart_details cart;
    cart.item_count = 0;

    fp = fopen("cart.csv", "r");
    if (fp == NULL) {
        perror("Unable to open file");
        return 1;
    }

    if (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%d", cart.restaurant_name, &cart.gst);
    }

    while (fgets(line, sizeof(line), fp)) {
        char item_name[50];
        int price, qty;
        
        sscanf(line, "%[^,],%d,%d", item_name, &price, &qty);

        strcpy(cart.item_names[cart.item_count], item_name);
        cart.price[cart.item_count] = price;
        cart.qty[cart.item_count] = qty;
        cart.item_total[cart.item_count] = price * qty;
        cart.item_count++;
    }
    int choice;
    do {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer(); // Clear input buffer

        switch (choice) {
            case 1:
                add_items(menu_items, total_items, &cart, cart);
                save_cart(&cart);
                //printf("Updated Cart Details");
                //view_cart(&cart);
                break;
            case 2:
                delete_items_cart(&cart,cart);
                save_cart(&cart);
                break;
            case 3:
                change_qty(&cart,cart);
                save_cart(&cart);
                break;
            case 4:
                display_total_items(menu_items,total_items);
                break;
            case 5:
                view_cart(&cart);
                break;
            case 6:
                //printf("Generated Bill Details\n");
                bill_generation(cart, ratings, tot_person,restaurantname,restaurants,num_restaurants,city);
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);

fclose(fp);
}
