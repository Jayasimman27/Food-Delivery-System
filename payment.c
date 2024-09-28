#include "headers.h"
#include <unistd.h>

int validate_upi_id(const char* upi_id) {
    int at_pos = -1;
    int len = strlen(upi_id);

    for (int i = 0; i < len; i++) {
        if (upi_id[i] == '@') {
            at_pos = i;
            break;
        }
    }

    if (at_pos == -1 || at_pos == 0 || at_pos == len - 1) {
        return 0;
    }

    for (int i = 0; i < at_pos; i++) {
        if (!isalnum(upi_id[i]) && upi_id[i] != '.' && upi_id[i] != '_' && upi_id[i] != '-') {
            return 0;
        }
    }

    for (int i = at_pos + 1; i < len; i++) {
        if (!isalnum(upi_id[i])) {
            return 0;
        }
    }

    return 1;
}

int upi_payment(int amount) {
    char entered_upi_id[50];
    int entered_amount;

    printf("Enter your UPI ID: ");
    fgets(entered_upi_id, sizeof(entered_upi_id), stdin);
    entered_upi_id[strcspn(entered_upi_id, "\n")] = 0;

    if (!validate_upi_id(entered_upi_id)) {
        printf("Invalid UPI ID format.\n");
        return 0;
    }

    printf("Enter the amount to pay: ");
    scanf("%d", &entered_amount);
    clear_input_buffer();

    if (entered_amount == amount) {
        printf("Processing transaction...\n");
        sleep(3);

        printf("Payment of %d successful via UPI ID: %s\n", amount, entered_upi_id);
        return 1;
    } else {
        printf("Payment failed. Please check the amount.\n");
        return 0;
    }
}

void getPaymentDetails(PaymentDetails* payment) {
    printf("Enter card number: ");
    fgets(payment->cardNumber, sizeof(payment->cardNumber), stdin);
    payment->cardNumber[strcspn(payment->cardNumber, "\n")] = 0;
    //clear_input_buffer();

    printf("Enter expiry date (MM/YY): ");
    fgets(payment->expiryDate, sizeof(payment->expiryDate), stdin);
    payment->expiryDate[strcspn(payment->expiryDate, "\n")] = 0;
    //clear_input_buffer();

    printf("Enter CVV: ");
    fgets(payment->cvv, sizeof(payment->cvv), stdin);
    payment->cvv[strcspn(payment->cvv, "\n")] = 0;
    //clear_input_buffer();

    printf("Enter amount: ");
    scanf("%lf", &payment->amount);
    //clear_input_buffer();
}

int validateCardDetails(PaymentDetails payment) {
    if (strlen(payment.cardNumber) != MAX_CARD_NUMBER_LENGTH) {
        printf("Invalid card number length.\n");
        return 0;
    }

    if (strlen(payment.expiryDate) != MAX_EXPIRY_DATE_LENGTH || payment.expiryDate[2] != '/') {
        printf("Invalid expiry date format.\n");
        return 0;
    }

    if (strlen(payment.cvv) != MAX_CVV_LENGTH) {
        printf("Invalid CVV length.\n");
        return 0;
    }
    return 1;
}

int makeTransaction(PaymentDetails payment) {
    printf("Processing transaction...\n");
    sleep(3);
    printf("Transaction successful for amount: %.2f\n", payment.amount);
    return 1;
}

void handle_payment_method(int payment_method, int total, int* success, int* cancel) {
    PaymentDetails payment;
    switch (payment_method) {
        case 1:
            if (upi_payment(total)) {
                *success = 1;
            } else {
                printf("Payment failed. Please try again.\n");
            }
            break;
        case 2:
            getPaymentDetails(&payment);
            if (validateCardDetails(payment)) {
                if (payment.amount != total) {
                    printf("Entered amount does not match the bill amount.\n");
                } else if (makeTransaction(payment)) {
                    *success = 1;
                } else {
                    printf("Payment failed.\n");
                }
            } else {
                printf("Invalid card details.\n");
            }
            break;
        case 3:
            printf("Cash on Delivery selected. Please keep the exact amount ready.\n");
            *success = 1;
            break;
        case 4:
            printf("Payment cancelled.\n");
            *cancel = 1;
            break;
        default:
            printf("Invalid payment method selected. Please choose a valid option.\n");
            break;
    }
}

int payment_main(int amount, float ratings, int tot_person, char restaurantname[50], Restaurant restaurants[],int num_restaurants, char city[50]) {
    int payment_method;
    int success = 0;
    int cancel = 0;

    while (!success && !cancel) {
        printf("\nChoose your payment method:\n");
        printf("1. UPI\n");
        printf("2. Credit/Debit Card\n");
        printf("3. Cash on Delivery\n");
        printf("4. Cancel Payment\n");
        printf("Enter your choice: ");
        scanf("%d", &payment_method);
        clear_input_buffer();

        handle_payment_method(payment_method, amount, &success, &cancel);

        if (cancel) {
            printf("Exiting payment process.\n");
            break;
        }
    }

    if (success) {
        printf("Payment completed successfully.\n");
        review_main(ratings, tot_person, restaurantname, restaurants,num_restaurants,city);
    } else {
        printf("Payment not completed.\n");
    }

    return 0;
}
