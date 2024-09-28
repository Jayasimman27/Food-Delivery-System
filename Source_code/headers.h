// headers.h
#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

#define V 50 
#define TOP 5 
#define MAX_RESTAURANTS 20

#define MAX_CARD_NUMBER_LENGTH 16
#define MAX_EXPIRY_DATE_LENGTH 5
#define MAX_CVV_LENGTH 3

#define MAX_ITEMS 25
#define MAX_ITEM_NAME_LENGTH 50
#define MAX_MOOD_LENGTH 20
#define MAX_CART_ITEMS 50


typedef struct {
    char name[MAX_ITEM_NAME_LENGTH];
    int price;
    char mood[MAX_MOOD_LENGTH];
} Item;

typedef struct {
    char name[MAX_ITEM_NAME_LENGTH];
    Item items[MAX_ITEMS];
    int total_items;
} Rest;

typedef struct {
    char rest_name[MAX_ITEM_NAME_LENGTH];
    char item_name[MAX_ITEM_NAME_LENGTH];
    int price;
    int qty;
} CartItem;


typedef struct {
    char cardNumber[20]; 
    char expiryDate[10]; 
    char cvv[5];         
    double amount;
} PaymentDetails;

// Structure definitions
typedef struct {
    char username[50];
    char password[50];
    char city[50];
} user;

typedef struct {
    char item_name[50];
    int price;
} MenuItem;

typedef struct {
    char restaurant_name[50];
    MenuItem menu_items[25];
    char password[20];
    int reviewed_person;
    float ratings;
    float gst;
    int total_items;
    char type[10];
} Restaurant;

typedef struct {
    char restaurant_name[50];
    char item_names[50][50];
    int gst;
    int price[50];
    int qty[50];
    int item_total[50];
    int item_count;
} cart_details;

void clear_input_buffer();
// Function prototypes from login_user.c
int validate_password(char password[]);
int user_register(FILE *file, user users[], int *record);
int user_login(user users[], int record);
int choose_user(user users[], int record, FILE *file_a, int *user_found);
int file_open();

// Function prototypes from selection.c
void select_foods(char restaurantname[50], float gst, MenuItem menu_items[], int total_items, float ratings, int tot_person, Restaurant restaurants[],int num_restaurants, char city[50]);
void select_restaurants(Restaurant restaurants[], int num_restaurants, char city[50]);
void print_food_by_restaurants(Restaurant restaurant, char food[50]);
void select_restaurants_by_food(Restaurant restaurants[], int num_restaurants, char city[50]);
int compare_ratings(const void *a, const void *b);
void printtype(Restaurant restaurants[], int num_restaurants, const char user_req[], char city[50]);
void compare_type(Restaurant restaurants[], int num_restaurants, char city[50]);
void choose(Restaurant restaurants[], int num_restaurants, char city[50]);
int rest_file_open(char city[50]);

// Function prototypes from dijkstra.c
int minDistance(int dist[], bool sptSet[]);
void dijkstra(int graph[V][V], int src, int dest[]);
int dij_main(char city[50]);

int rest_log_reg();
int choose_restaurant(Restaurant restaurants[], int num_restaurants);
int cart_detail(MenuItem menu_items[], int total_items, float ratings, int tot_person, char restaurantname[50], Restaurant restaurants[], int num_restaurants, char city[50]);
int add_items(MenuItem menu_items[], int total_items, cart_details* cart, cart_details carts);
int delete_items_cart(cart_details* cart, cart_details carts);
int change_qty(cart_details* cart, cart_details carts);
int save_cart(cart_details* cart);
int view_cart(cart_details* cart);
int bill_generation(cart_details cart,float ratings, int tot_person, char restaurantname[50],Restaurant restaurants[],int num_restaurants, char city[50]);
void write_data(Restaurant restaurants[], int num_restaurants);

//int processPayment(PaymentDetails payment, double billamount);
int validateCardDetails(PaymentDetails payment);
int makeTransaction(PaymentDetails payment);
//void getInput(char* prompt, char* input, int length);
void getPaymentDetails(PaymentDetails *payment);
int payment_main(int amount, float ratings, int tot_person, char restaurantname[50],Restaurant restaurants[],int num_restaurants, char city[50]);

void read_menu_data(const char *file_name);
void display_menu_based_on_mood(const char *mood);
Item* get_menu_item_by_number(int number, const char *mood, int *rest_index);
int is_valid_mood(const char *mood);
void add_to_cart(const char *rest_name, const char *item_name, int price);
void save_cart_to_csv(const char *file_name);
int mood_main(Restaurant restaurants[], int num_restaurants, char city[50]);
int dist_main(char destination[50], char source[50]);
int review_main(float ratings, int tot_person, char restaurantname[50], Restaurant restaurants[],int num_restaurants, char city[50]);
#endif // HEADERS_H
