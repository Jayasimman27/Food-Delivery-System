#include "headers.h"
#include <time.h>

int coupon_validator(int* offer){
    char coupon[][20] = {"HELLO20", "NEW10", "25OFFER"};
    int offers[20] = {20,10,25};
    char enter[50];
    printf("\nDo you Have any coupon Codes to Enter, Type Coupon Code or enter 'end' :");

    fgets(enter,sizeof(enter),stdin);
    enter[strcspn(enter, "\n")] = 0;

    for (int i = 0; i < 3; i++){
        if (strcmp(enter, coupon[i]) == 0){
            *offer = offers[i];
        }
        else if (strcmp(enter, "end") == 0){
            *offer = 0;
        }
    }
}

int bill_generation(cart_details cart, float ratings, int tot_person, char restaurantname[50],Restaurant restaurants[],int num_restaurants, char city[50]) {
    int subtotal = 0;
    int gst = 0;
    int total = 0, tot = 0;
    int offers = 0;
    coupon_validator(&offers);

    // Get the current date and time
    time_t t;
    struct tm *tm_info;
    char date_time[25];

    time(&t);
    tm_info = localtime(&t);

    strftime(date_time, 25, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("Generated Bill Details\n\n");

    printf("\n**********************************************************\n");
    printf("\t\t\t\t%s\n", cart.restaurant_name);
    printf("************************************************************\n");
    printf("Date and Time: %s\n", date_time);
    printf("\n");

    printf("S.No.\t Item\t\t\t\t Quantity\t Price\t Total\n");
    printf("-------------------------------------------------------------------\n");
    for (int i = 0; i < cart.item_count; i++) {
        printf("%d\t %-30s %-8d\t %-6d\t %d\n", i + 1, cart.item_names[i], cart.qty[i], cart.price[i], cart.item_total[i]);
        subtotal += cart.item_total[i];
    }
    gst = (int)(cart.gst * 0.01 * subtotal);
    tot = subtotal + gst;
    total = tot - (int )(0.01 * offers * tot);

    printf("\n\n");
    printf("\t\t\t\t\t Subtotal : %d\n", subtotal);
    printf("\t\t\t\t\t GST : %d%% - %d\n", cart.gst, gst);
    printf("\t\t\t\t Coupon Offers : %d%% - %d\n",offers, (int)(0.01 * offers * tot));
    printf("\t\t\t\t\t Total : %d\n", total);
    
    payment_main(total,ratings,tot_person,restaurantname,restaurants,num_restaurants,city);
    return total;
}
