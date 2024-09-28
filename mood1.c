#include "headers.h"

Rest rests[MAX_RESTAURANTS];
CartItem cart[MAX_CART_ITEMS];
int num_rests = 0;
int cart_size = 0;

void read_menu_data(const char *file_name) {
    FILE *file;
    char line[150];

    file = fopen(file_name, "r");

    if (file == NULL) {
        printf("Error opening the file!\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "") == 0) continue;

        char *comma_position = strchr(line, ',');
        if (comma_position != NULL) {
            *comma_position = '\0';
        }

        strcpy(rests[num_rests].name, line);

        fgets(line, sizeof(line), file);
        int total_items;
        sscanf(line, "%d", &total_items);
        rests[num_rests].total_items = total_items;

        for (int i = 0; i < total_items; i++) {
            fgets(line, sizeof(line), file);
            line[strcspn(line, "\n")] = '\0';
            sscanf(line, "%49[^,],%d,%19s", rests[num_rests].items[i].name, &rests[num_rests].items[i].price, rests[num_rests].items[i].mood);
        }

        num_rests++;
        if (num_rests >= MAX_RESTAURANTS) {
            break;
        }
    }

    fclose(file);
}

void display_menu_based_on_mood(const char *mood) {
    printf("Menu for %s mood:\n", mood);

    int found = 0;
    int item_count = 1;
    for (int i = 0; i < num_rests; i++) {
        int rest_has_mood = 0;
        for (int j = 0; j < rests[i].total_items; j++) {
            if (strcmp(rests[i].items[j].mood, mood) == 0) {
                if (!rest_has_mood) {
                    printf("\n%s:\n", rests[i].name);
                    rest_has_mood = 1;
                }
                printf("%d. %s - $%d\n", item_count, rests[i].items[j].name, rests[i].items[j].price);
                item_count++;
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No items found for this mood.\n");
    }
}

Item* get_menu_item_by_number(int number, const char *mood, int *rest_index) {
    int item_count = 1;
    for (int i = 0; i < num_rests; i++) {
        for (int j = 0; j < rests[i].total_items; j++) {
            if (strcmp(rests[i].items[j].mood, mood) == 0) {
                if (item_count == number) {
                    *rest_index = i;
                    return &rests[i].items[j];
                }
                item_count++;
            }
        }
    }
    return NULL; 
}

int is_valid_mood(const char *mood) {
    const char *valid_moods[] = {"Comfort", "Celebration", "Healthy", "Spicy", "Refresh", "Relax"};
    for (int i = 0; i < 6; i++) {
        if (strcmp(mood, valid_moods[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_to_cart(const char *rest_name, const char *item_name, int price) {
    for (int i = 0; i < cart_size; i++) {
        if (strcmp(cart[i].item_name, item_name) == 0) {
            cart[i].qty++;
            return;
        }
    }

    strcpy(cart[cart_size].rest_name, rest_name);
    strcpy(cart[cart_size].item_name, item_name);
    cart[cart_size].price = price;
    cart[cart_size].qty = 1;
    cart_size++;
}

void save_cart_to_csv(const char *file_name) {
    FILE *file = fopen(file_name, "w");

    if (file == NULL) {
        printf("Error opening the file for writing!\n");
        exit(1);
    }

    if (cart_size > 0) {
        fprintf(file, "%s,5.3\n", cart[0].rest_name); // Assuming the GST is 5.3%
    }

    for (int i = 0; i < cart_size; i++) {
        fprintf(file, "%s,%d,%d\n", cart[i].item_name, cart[i].price, cart[i].qty);
    }

    fclose(file);
}

int mood_main(Restaurant restaurants[], int num_restaurants, char city[50]) {
    read_menu_data("menu_data.txt");

    char selected_mood[MAX_MOOD_LENGTH];
    printf("Bored of the Traditional Menus!!! \nEnter Your Mood and Get the Menu :-) :\n");
    clear_input_buffer();
    do {
        printf("Select Your Mood Right Now:\n- Comfort\n- Celebration\n- Healthy\n- Spicy\n- Refresh\n- Relax\n");
        printf("Your Current Mood: ");
        fgets(selected_mood, MAX_MOOD_LENGTH, stdin);
        selected_mood[strcspn(selected_mood, "\n")] = '\0';

        if (!is_valid_mood(selected_mood)) {
            printf("Invalid mood. Please select a valid mood.\n");
        }
    } while (!is_valid_mood(selected_mood));

    display_menu_based_on_mood(selected_mood);

    int selection;
    printf("Enter the number of the item you'd like to select further selection you can do later: ");
    scanf("%d", &selection);

    if (selection == 0) {
        printf("No items selected.\n");
        return 0;
    }

    int rest_index;
    Item* selected_item = get_menu_item_by_number(selection, selected_mood, &rest_index);

    if (selected_item != NULL) {
        printf("\n%s from %s - $%d is added to the cart successfully.\n", selected_item->name, rests[rest_index].name, selected_item->price);
        add_to_cart(rests[rest_index].name, selected_item->name, selected_item->price);
        save_cart_to_csv("cart.csv");

        for (int i =0; i <num_restaurants; i++){
            if (strcmp(restaurants[i].restaurant_name, rests[rest_index].name) == 0){
                cart_detail(restaurants[i].menu_items,restaurants[i].total_items,restaurants[i].ratings, restaurants[i].reviewed_person,restaurants[i].restaurant_name,restaurants, num_restaurants, city);
            }
        } 
    }
    
    else {
        printf("Invalid selection.\n");
    }

    return 0;
}
