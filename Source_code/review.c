#include "headers.h"
int review_main(float ratings, int tot_person, char restaurantname[50], Restaurant restaurants[],int num_restaurants, char city[50]){
    float rating_cust;
    float final_rating;

    printf("Please Enter the rating of the restaurant:");
    scanf("%f",&rating_cust);
    final_rating = ((tot_person * ratings) + rating_cust)/(tot_person + 1);
    //printf(" Final Rating :%f", final_rating);21

    for (int i = 0; i < num_restaurants; i++){
        if (strcmp(restaurantname,restaurants[i].restaurant_name) == 0){
            restaurants[i].ratings = final_rating;
            restaurants[i].reviewed_person+=1;

            printf("Updated review Successfully");
            dist_main(city,restaurantname);
        }
    }
    write_data(restaurants,num_restaurants);
}