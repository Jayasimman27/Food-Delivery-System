#include "headers.h"
#include <conio.h>

void get_password(char *password, int max_len) {
    int i = 0;
    char ch;

    while (i < max_len - 1 && (ch = _getch()) != '\r') { 
        if (ch == 8) { 
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}


int validate_password(char password[]) {
    int length = strlen(password);
    int uppercase = 0, lowercase = 0, digit = 0, special_char = 0;

    if (length < 8) {
        printf("Password must be at least 8 characters long.\n");
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) {
            uppercase = 1;
        } else if (islower(password[i])) {
            lowercase = 1;
        } else if (isdigit(password[i])) {
            digit = 1;
        } else if (!isalnum(password[i])) {
            special_char = 1;
        }
    }

    if (!uppercase || !lowercase || !digit || !special_char) {
        printf("Password must contain at least one uppercase letter, one lowercase letter, one digit, and one special character.\n");
        return 0;
    }
    return 1;
}

int user_register(FILE *file, user users[], int *record) {
    char username[50];
    char pass[50],confirm_pass[50];
    char city[50];

    printf("Enter Your Username: ");
    scanf("%s", username);

    for (int i = 0; i < *record; i++) {
        if (strcmp(username, users[i].username) == 0) {
            printf("User already exists. Try another username!\n");
            return 0;
        }
    }

    int valid_password = 0;
    while (!valid_password) {
        printf("Enter Your Password: ");
        get_password(pass, sizeof(pass));

        if (!validate_password(pass)) {
            printf("Please enter a valid password.\n");
        } else {
            valid_password = 1;
        }
    }

    while (1 && valid_password == 1) {
        printf("Confirm Your Password: ");
        get_password(confirm_pass, sizeof(confirm_pass));

        if (strcmp(pass, confirm_pass) == 0) {
            printf("Enter your City:");
            clear_input_buffer();
            fgets(city,sizeof(city),stdin);

            if (city[strlen(city) - 1] == '\n') {
                city[strlen(city) - 1] = '\0';
            }
            
            break; 
        } else {
            printf("Passwords do not match. Please try again.\n");
        }
    }

    fprintf(file, "\n%s,%s,%s", username, pass,city);

    strcpy(users[*record].username, username);
    strcpy(users[*record].password, pass);

    (*record)++;
    return 1; 
}

int user_login(user users[], int record) {
    char name[50];
    char password[50];
    int user_login = 0;
    printf("\nEnter Your Username: ");
    scanf("%s", name);
    printf("Enter Your Password: ");
    get_password(password, sizeof(password));

    for (int i = 0; i < record; i++) {
        if (strcmp(name, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            printf("\nUser Logged in Successfully!\n");
            rest_file_open(users[i].city);
            return 1; 
        }
    }

    return 0;
}

int choose_user(user users[], int record, FILE *file_a, int *user_found) {
    int number;
    printf("1. User Login\n2. User Registration\n\n");
    printf("Enter number to proceed: ");
    scanf("%d", &number);

    if (number == 1) {
        if (user_login(users, record)) {
            *user_found = 1;
        } else {
            printf("\nUser Not Found or Incorrect Password. Please Try Again!!\n");
            choose_user(users, record, file_a, user_found);
        }
    } else if (number == 2) {
        int success = 0;
        while (!success) {
            if (user_register(file_a, users, &record)) {
                printf("\nUser registered successfully!\n");
                success = 1;
                *user_found = 1;
            }
        }
    } else {
        printf("Invalid choice. Please enter 1 or 2.\n");
    }
    return 0;
}

int file_open() {
    FILE *file;
    file = fopen("Login.csv", "r");

    if (file == NULL) {
        printf("File doesn't exist!\n");
        return 1;
    }

    user users[100];
    int record = 0;
    int user_found = 0;

    do {
        int read = fscanf(file, "%49[^,],%49[^,],%49[^,\n]\n", users[record].username, users[record].password,users[record].city);
        if (read == 3) {
            record++;
        }

        if (ferror(file)) {
            printf("Error reading the file\n");
            fclose(file);
            return 1; 
        }
    } while (!feof(file) && record < 100);

    fclose(file);
    //printf("%s",users[1].city);
    if (record == 0) {
        printf("No users found in the file.\n");
        return 1;
    }

    FILE *file_a;
    file_a = fopen("Login.csv", "a+");
    
    if (file_a == NULL) {
        printf("Error opening file for registration.\n");
        return 1;
    }

    choose_user(users, record, file_a, &user_found);
    fclose(file_a);
    return user_found;
}