#include "headers.h"
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void select_foods(char restaurantname[50],float gst,MenuItem menu_items[], int total_items, float ratings, int tot_person, Restaurant restaurants[], int num_restaurants, char city[50]) {
    FILE *cart = fopen("cart.csv", "w");
    if (!cart) {
        printf("Error opening the cart file!\n");
        return;
    }
    char item_name[50];
    fprintf(cart,"%s,%.1f\n",restaurantname,gst);
    printf("Enter food item names (type 'end' to finish):\n");

    while (1) {
        printf("\nEnter the food item name: ");
        fgets(item_name, sizeof(item_name), stdin);

        if (item_name[strlen(item_name) - 1] == '\n') {
            item_name[strlen(item_name) - 1] = '\0';
        }
        if (strcmp(item_name, "end") == 0) {
            printf("Your order Placed Successfully %c", 1);
            break;
        }

        int found = 0;
        for (int j = 0; j < total_items; j++) {
            if (strcasecmp(item_name, menu_items[j].item_name) == 0) {
                int qty=1;
                printf("Enter the total quantity you want to buy:");
                scanf("%d",&qty);
                clear_input_buffer();
                fprintf(cart, "%s,%d,%d\n", menu_items[j].item_name, menu_items[j].price,qty);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("\nItem Not found\n");
        } else {
            printf("%s Added to the cart successfully\n", item_name);
        }
    }
    fclose(cart);
    cart_detail(menu_items,total_items, ratings,tot_person, restaurantname,restaurants,num_restaurants, city);
}

void select_restaurants(Restaurant restaurants[], int num_restaurants, char city[50]) {
    char rest[50];
    clear_input_buffer();

    while (1) {
        printf("\nChoose the restaurants from the above list: ");
        fgets(rest, sizeof(rest), stdin);

        if (rest[strlen(rest) - 1] == '\n') {
            rest[strlen(rest) - 1] = '\0';
        }

        int found = 0;
        for (int i = 0; i < num_restaurants; i++) {
            if (strcmp(rest, restaurants[i].restaurant_name) == 0) {
                found = 1;
                printf("\nMenu:\n");
                for (int j = 0; j < restaurants[i].total_items; j++) {
                    printf("%s - $%d\n", restaurants[i].menu_items[j].item_name, restaurants[i].menu_items[j].price);
                }
                select_foods(restaurants[i].restaurant_name,restaurants[i].gst,restaurants[i].menu_items, restaurants[i].total_items, restaurants[i].ratings, restaurants[i].reviewed_person,restaurants, num_restaurants, city);
                break;
            }
        }

        if (found) {
            break;
        } else {
            printf("Restaurant not found! Try again.\n");
        }
    }
}

void print_food_by_restaurants(Restaurant restaurant, char food[50]) {
    for (int i = 0; i < restaurant.total_items; i++) {
        if (strcmp(food, restaurant.menu_items[i].item_name) == 0) {
            printf("%s\n%s - %d\n\n", restaurant.restaurant_name, restaurant.menu_items[i].item_name, restaurant.menu_items[i].price);
        }
    }
}

void select_restaurants_by_food(Restaurant restaurants[], int num_restaurants, char city[50]) {
    clear_input_buffer();
    char food[50];
    char rest[50];
    int food_found = 0;
    int founded = 0;
    FILE *cart_food = fopen("cart.csv", "w");
    if (!cart_food) {
        printf("Error opening the cart file!\n");
        return;
    }

    printf("Enter the food name: ");
    fgets(food, sizeof(food), stdin);

    if (food[strlen(food) - 1] == '\n') {
        food[strlen(food) - 1] = '\0';
    }

    while (1) {
        for (int i = 0; i < num_restaurants; i++) {
            for (int j = 0; j < restaurants[i].total_items; j++) {
                if (strcmp(food, restaurants[i].menu_items[j].item_name) == 0) {
                    food_found = 1;
                    print_food_by_restaurants(restaurants[i], food);
                }
            }
        }

        if (food_found) {
            printf("Now choose the restaurant to order %s: ", food);
            fgets(rest, sizeof(rest), stdin);

            if (rest[strlen(rest) - 1] == '\n') {
                rest[strlen(rest) - 1] = '\0';
            }

            int found = 0;
            for (int i = 0; i < num_restaurants; i++) {
                if (strcmp(rest, restaurants[i].restaurant_name) == 0) {
                    found = 1;
                    fprintf(cart_food,"%s,%.1f\n",restaurants[i].restaurant_name,restaurants[i].gst);
                    printf("Item added to the Cart Successfully!!!\n");
                    for (int j = 0; j < restaurants[i].total_items; j++) {
                        if (strcmp(food, restaurants[i].menu_items[j].item_name) == 0) {
                            founded = 1;
                            fprintf(cart_food, "%s,%d,%d\n", restaurants[i].menu_items[j].item_name, restaurants[i].menu_items[j].price,1);
                            if (founded){
                                fclose(cart_food);
                                cart_detail(restaurants[i].menu_items,restaurants[i].total_items, restaurants[i].ratings, restaurants[i].reviewed_person, restaurants[i].restaurant_name, restaurants, num_restaurants,city);
                            }
                        }
                    }
                    break;
                }
            }
            if (!found) {
                printf("Restaurant not found or The spelling is not correct! Try again!\n\n");
                continue;
            } else {
                break;
            }
        } else {
            printf("Food not found! Please enter a valid food name.\n");
            printf("Enter the food name: ");
            fgets(food, sizeof(food), stdin);
            if (food[strlen(food) - 1] == '\n') {
                food[strlen(food) - 1] = '\0';
            }
        }

        //fclose(cart_food);
        //cart_detail(restaurants[i].menu_items,restaurants[i].total_items);
    }
}

int compare_ratings(const void *a, const void *b) {
    const Restaurant *restaurant_a = (const Restaurant *)a;
    const Restaurant *restaurant_b = (const Restaurant *)b;
    return (restaurant_b->ratings > restaurant_a->ratings) - (restaurant_b->ratings < restaurant_a->ratings);
}

void printtype(Restaurant restaurants[], int num_restaurants, const char user_req[], char city[50]) {
    int found = 0;
    for (int i = 0; i < num_restaurants; i++) {
        if (strcmp(restaurants[i].type, user_req) == 0 || strcmp(user_req, "all") == 0) {
            printf("%s\n", restaurants[i].restaurant_name);
            found = 1;
        }
    }

    if (found) {
        select_restaurants(restaurants, num_restaurants, city);
    } else {
        printf("No restaurants found for the type: %s\n", user_req);
    }
}

void compare_type(Restaurant restaurants[], int num_restaurants, char city[50]) {
    char user_req[10];
    printf("\nEnter veg for VEG Restaurants\nEnter non-veg for NON-VEG Restaurants\nEnter all to see ALL the restaurants\n\n");
    printf("Enter which type of restaurants you want: ");
    scanf("%s", user_req);

    printtype(restaurants, num_restaurants, user_req, city);
}

void choose(Restaurant restaurants[], int num_restaurants, char city[50]) {
    int option;
    while (1) {
        printf("1. For restaurants based on Ratings\n2. For restaurants based on VEG or NON-VEG\n3. For searching restaurants by your preferred food\n4. For selecting the restaurants near to you.\n5. For Restaurants and foods based on Mood\n\n");
        printf("Enter your Option: ");
        scanf("%d", &option);

        if (option == 1) {
            qsort(restaurants, num_restaurants, sizeof(Restaurant), compare_ratings);
            printf("\nTop 5 restaurants based on ratings.\n");
            for (int i = 0; i < 5 && i < num_restaurants; i++) {
                printf("Restaurant: %s\nRatings: %.1f\n\n", restaurants[i].restaurant_name, restaurants[i].ratings);
            }
            select_restaurants(restaurants, num_restaurants, city);
            break;
        } 
        else if (option == 2) {
            compare_type(restaurants, num_restaurants, city);
            break;
        } else if (option == 3) {
            select_restaurants_by_food(restaurants, num_restaurants,city);
            break;}
        else if (option == 4){
            dij_main(city);
            select_restaurants(restaurants, num_restaurants, city);
            break;
        }
        else if (option == 5){
            mood_main(restaurants, num_restaurants, city);
            break;
        }
        else {
            printf("Invalid option. Please try again.\n");
            clear_input_buffer();
        }
    }
}

int rest_file_open(char city[50]) {
    FILE *file;
    Restaurant restaurants[MAX_RESTAURANTS];
    int num_restaurants = 0;
    char line[256];

    file = fopen("new_menu.txt", "r");

    if (file == NULL) {
        printf("Error Opening the File!\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%49[^,],%19[^,],%f,%d,%f,%9s",
               restaurants[num_restaurants].restaurant_name,
               restaurants[num_restaurants].password,
               &restaurants[num_restaurants].ratings,
               &restaurants[num_restaurants].reviewed_person,
               &restaurants[num_restaurants].gst,
               restaurants[num_restaurants].type);

        fgets(line, sizeof(line), file);
        sscanf(line, "%d", &restaurants[num_restaurants].total_items);

        for (int i = 0; i < restaurants[num_restaurants].total_items; i++) {
            fgets(line, sizeof(line), file);
            sscanf(line, "%79[^,],%d",
                   restaurants[num_restaurants].menu_items[i].item_name,
                   &restaurants[num_restaurants].menu_items[i].price);

            //printf("Parsed menu item: %s, %d\n", restaurants[num_restaurants].menu_items[i].item_name,
              // restaurants[num_restaurants].menu_items[i].price);
        }

        num_restaurants++;
        //if (num_restaurants >= MAX_RESTAURANTS) {
            //break;
        //}

        fgets(line, sizeof(line), file); 
        fgets(line, sizeof(line), file);
    }
    //printf("Revied Person %d", restaurants[10].reviewed_person);

    fclose(file);

    choose(restaurants, num_restaurants,city);

    return 0;
}
