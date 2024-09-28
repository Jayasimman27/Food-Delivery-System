#include <stdio.h>
#include <string.h>
#include "headers.h"

#define MAX_ITEMS 25

/* void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
} */

void print_data(Restaurant restaurants[], int num_restaurants){
    FILE *fp;
    fp = fopen("rest_menu.txt", "w");
    if (fp == NULL) {
        printf("Error opening the file!\n");
        return;
    }
    for (int i = 0; i < num_restaurants; i++) {
        fprintf(fp, "%s,%s,%.1f,%.1f,%s\n%d\n",
                restaurants[i].restaurant_name,
                restaurants[i].password,
                restaurants[i].ratings,
                restaurants[i].gst,
                restaurants[i].type,
                restaurants[i].total_items);

        for (int j = 0; j < restaurants[i].total_items; j++) {
            fprintf(fp, "%s,%d\n", 
                    restaurants[i].menu_items[j].item_name,
                    restaurants[i].menu_items[j].price);
        }
        fprintf(fp, "\n\n");
    }
    fclose(fp);
}

void add_menu_items(Restaurant *restaurant, Restaurant restaurants[], int num_restaurants){
    int total = restaurant->total_items;
    int total_items;
    char item[50];
    int price;
    printf("Enter the total items you are going to add to the restaurants:");
    scanf ("%d", &total_items);

    for (int i =  0; i < total_items; i ++){ 
        clear_input_buffer();

        printf("\nEnter the item %d:", i+1);
        fgets(item,sizeof(item),stdin);
        item[strcspn(item, "\n")] = '\0';

        printf("Enter the price of item %d:",i+1);
        scanf("%d", &price);

        strcpy(restaurant->menu_items[total + i].item_name, item);
        restaurant->menu_items[total + i].price = price;
        restaurant->total_items = restaurant->total_items+1;
        printf("%s with price %d is added to the menu.",item,price);
    }
    print_data(restaurants,num_restaurants);
}

void price_change(Restaurant *restaurant, Restaurant restaurants[], int num_restaurants) {
    char item[50];
    int item_found = 0;
    clear_input_buffer();
    
    printf("Enter the item name for which you've to change the price: ");
    fgets(item, sizeof(item), stdin);
    item[strcspn(item, "\n")] = '\0';

    for (int i = 0; i < restaurant->total_items; i++) {
        if (strcmp(item, restaurant->menu_items[i].item_name) == 0) {
            item_found = 1;
            if (item_found) {
                int new_price;
                printf("Enter the new price for the item: ");
                scanf("%d", &new_price);
                restaurant->menu_items[i].price = new_price;
                printf("\nPrice of %s has been updated to %d.\n", item, new_price);
                break; 
            }
        }
    }

    if (!item_found) {
        printf("Item not found in the menu.\n");
    }
    print_data(restaurants,num_restaurants);
}

void delete_items(Restaurant *restaurant, Restaurant restaurants[], int num_restaurants) {
    char item[50];
    int item_found = 0;

    clear_input_buffer();

    printf("Enter the item name you want to delete: ");
    fgets(item, sizeof(item), stdin);
    item[strcspn(item, "\n")] = '\0';

    for (int i = 0; i < restaurant->total_items; i++) {
        if (strcmp(item, restaurant->menu_items[i].item_name) == 0) {
            item_found = 1;
            for (int j = i; j < restaurant->total_items - 1; j++) {
                strcpy(restaurant->menu_items[j].item_name, restaurant->menu_items[j + 1].item_name);
                restaurant->menu_items[j].price = restaurant->menu_items[j + 1].price;
            }
            restaurant->total_items--;
            printf("\nItem '%s' has been deleted from the menu.\n", item);
            break; 
        }
    }

    if (!item_found) {
        printf("Item not found in the menu.\n");
    }

    print_data(restaurants,num_restaurants);
}

void change_gst(Restaurant *restaurant, Restaurant restaurants[], int num_restaurants){
    float gst;
    printf("\nEnter the gst: ");
    scanf("%f", &gst);
    restaurant->gst = gst;
    printf("\nThe new gst for your restaurant is %.1f percent",gst);
    print_data(restaurants,num_restaurants);
}

void change_type(Restaurant *restaurant, Restaurant restaurants[], int num_restaurants){
    char type[50];
    printf("\nEnter new type of your restaurant veg, non-veg, or contains both: ");
    scanf("%s", type);
    strcpy(restaurant->type, type);
    printf("\nThe new restaurant type for your restaurant is %s", type);
    print_data(restaurants, num_restaurants);
}


int login_restaurant(Restaurant restaurants[], int num_restaurants) {
    char name[50];
    char pass[20];
    int found = 0;
    int logged_in = 0;

    printf("Enter your restaurant name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    for (int i = 0; i < num_restaurants; i++) {
        if (strcmp(restaurants[i].restaurant_name, name) == 0) {
            found = 1;

            printf("Enter your password: ");
            scanf("%19s", pass);

            if (strcmp(restaurants[i].password, pass) == 0) {
                logged_in = 1;
                printf("Login successful.\n");

                if (logged_in){
                    int options;

                    while (1) {
                        printf("\n 1. For adding menu items:\n");
                        printf(" 2. To edit the existing menu item's price:\n");
                        printf(" 3. For deleting an item in the menu:\n");
                        printf(" 4. For Changing the gst:\n");
                        printf(" 5. For changing the Restaurant's type:\n");
                        printf(" 6. To Exit!\n");
                        printf("Enter the option:");
                        scanf("%d", &options);

                        switch (options) {
                            case 1:
                                add_menu_items(&restaurants[i], restaurants, num_restaurants);
                                break;
                            case 2:
                                price_change(&restaurants[i], restaurants, num_restaurants);
                                break;
                            case 3:
                                delete_items(&restaurants[i], restaurants, num_restaurants);
                                break;
                            case 4:
                                change_gst(&restaurants[i], restaurants, num_restaurants);
                                break;
                            case 5:
                                change_type(&restaurants[i], restaurants, num_restaurants);
                                break;
                            case 6:
                                return 0;
                            default:
                                printf("Invalid Option!\n");
                        }
                    }  
                }
            } 
            else {
                printf("Incorrect password. Login failed.\n");
            }
            break;
        }
    }
    if (!found) {
        printf("Restaurant not found.\n");
    }
    clear_input_buffer(); 
}

void write_data(Restaurant restaurants[], int num_restaurants){
    FILE *newfile;
    newfile = fopen("new_menu.txt", "w");
    if (newfile == NULL){
        printf("Error Opening the file!!");
    }
    for (int i = 0; i< num_restaurants; i++){
        fprintf(newfile,"%s,%s,%.1f,%d,%.1f,%s\n%d\n", restaurants[i].restaurant_name,restaurants[i].password,restaurants[i].ratings,restaurants[i].reviewed_person,restaurants[i].gst,restaurants[i].type,restaurants[i].total_items);
        for (int j = 0; j < restaurants[i].total_items; j++){
            fprintf(newfile,"%s,%d\n", restaurants[i].menu_items[j].item_name,restaurants[i].menu_items[j].price);
        }
        fprintf(newfile,"\n\n");
    }
    fclose(newfile);
}

int is_restaurant_existing(Restaurant restaurants[], int num_restaurants, const char* rest_name) {
    for (int i = 0; i < num_restaurants; i++) {
        if (strcmp(restaurants[i].restaurant_name, rest_name) == 0) {
            return 1;
        }
    }
    return 0;
}

void register_restaurant(Restaurant restaurants[], int *num_restaurants) {
    char rest_name[50];
    char rest_pass[20];
    char rest_confirm[20];
    int gst = 0;
    int rest_item = 0;
    char rest_type[50];

    printf("\nEnter your new restaurant name to register: ");
    fgets(rest_name, sizeof(rest_name), stdin);
    rest_name[strcspn(rest_name, "\n")] = '\0'; 

    if (is_restaurant_existing(restaurants, *num_restaurants, rest_name)) {
        printf("Restaurant with this name already exists. Registration failed.\n");
        return;
    }

    printf("Enter your password to login next time: ");
    scanf("%19s", rest_pass);

    while (1) {
        printf("Confirm your password: ");
        scanf("%19s", rest_confirm);

        if (strcmp(rest_pass, rest_confirm) == 0) {
            break;
        } else {
            printf("Passwords do not match. Please try again.\n");
        }
    }

    printf("Enter your gst for your restaurant: ");
    scanf("%f", &gst);

    printf("Enter the total items in the menu: ");
    scanf("%d", &rest_item);

    printf("Enter your restaurant type: ");
    scanf("%19s", rest_type);


    if (rest_item > MAX_ITEMS) {
        printf("Too many items. Maximum allowed is %d.\n", MAX_ITEMS);
        return;
    }

    char item_name[50];
    int price;
    char buffer[10]; 

    for (int i = 0; i < rest_item; i++) {
        fgets(buffer, sizeof(buffer), stdin);

        printf("Enter the name of item %d: ", i + 1);
        fgets(item_name, sizeof(item_name), stdin);
        item_name[strcspn(item_name, "\n")] = '\0';
        printf("Enter the price of item %d: ", i + 1);
        scanf("%d", &price);

        strcpy(restaurants[*num_restaurants].menu_items[i].item_name, item_name);
        restaurants[*num_restaurants].menu_items[i].price = price;
    }

    strcpy(restaurants[*num_restaurants].restaurant_name, rest_name);
    strcpy(restaurants[*num_restaurants].password, rest_pass);
    strcpy(restaurants[*num_restaurants].type,rest_type);
    restaurants[*num_restaurants].ratings = 0.0; 
    restaurants[*num_restaurants].gst = gst;
    restaurants[*num_restaurants].total_items = rest_item;

    (*num_restaurants)++;
    int total_restaurants = *num_restaurants;
    write_data(restaurants,  total_restaurants);
}

void print_restaurant_data(const Restaurant restaurants[], int num_restaurants) {
    for (int i = 0; i < num_restaurants; i++) {
        printf("Restaurant %d:\n", i + 1);
        printf("Name: %s\n", restaurants[i].restaurant_name);
        printf("Offer: %f\n", restaurants[i].gst);
        printf("Items:\n");
        for (int j = 0; j < restaurants[i].total_items; j++) {
            printf("  %s: %d\n", restaurants[i].menu_items[j].item_name, restaurants[i].menu_items[j].price);
        }
    }
}
int choose_restaurant(Restaurant restaurants[], int num_restaurants) {
    int number;
    printf("1. Restaurant Login\n2. Restaurant Registration\n\n");
    printf("Enter number to proceed: ");
    scanf("%d", &number);
    clear_input_buffer();

    if (number == 1) {
        login_restaurant(restaurants, num_restaurants);
    } else if (number == 2) {
        register_restaurant(restaurants, &num_restaurants);
    } else {
        printf("Invalid choice. Please enter 1 or 2.\n");
    }
    return 0;
}

int rest_log_reg() {  
    FILE *file;
    Restaurant restaurants[MAX_RESTAURANTS];
    int num_restaurants = 0;
    char line[100];

    file = fopen("new_menu.txt", "r");

    if (file == NULL) {
        printf("Error opening the file!\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%49[^,],%49[^,],%f,%f,%s",
               restaurants[num_restaurants].restaurant_name,
               restaurants[num_restaurants].password,
               &restaurants[num_restaurants].ratings,
               &restaurants[num_restaurants].gst,
               restaurants[num_restaurants].type);

        fgets(line, sizeof(line), file);
        sscanf(line, "%d", &restaurants[num_restaurants].total_items);

        for (int i = 0; i < restaurants[num_restaurants].total_items; i++) {
            fgets(line, sizeof(line), file);
            sscanf(line, "%49[^,],%d",
                   restaurants[num_restaurants].menu_items[i].item_name,
                   &restaurants[num_restaurants].menu_items[i].price);
        }

        num_restaurants++;
        if (num_restaurants >= MAX_RESTAURANTS) {
            break;
        }

        fgets(line, sizeof(line), file); 
        fgets(line, sizeof(line), file);
    }

    fclose(file);
    //login_restaurant(restaurants, num_restaurants);
    choose_restaurant(restaurants,num_restaurants);
    // register_restaurant(restaurants, &num_restaurants);
    // print_restaurant_data(restaurants, num_restaurants);
    // write_data(restaurants, num_restaurants);
    return 0;
}
