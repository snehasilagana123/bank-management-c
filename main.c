#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------- Structures -------------------
struct User {
    char username[50];
    char password[50];
};

struct Account {
    int accNumber;
    char name[50];
    float balance;
};

// ------------------- Registration -------------------
void registerUser() {
    FILE *fp = fopen("users.dat", "ab");
    struct User user;

    printf("\n====== Registration ======\n");
    printf("Enter Username: ");
    scanf("%s", user.username);
    printf("Enter Password: ");
    scanf("%s", user.password);

    fwrite(&user, sizeof(user), 1, fp);
    fclose(fp);

    printf("Registration successful! You can now login.\n");
}

// ------------------- Login -------------------
int login() {
    FILE *fp = fopen("users.dat", "rb");
    if (fp == NULL) {
        printf("No registered users found. Please register first.\n");
        return 0;
    }

    struct User user, input;
    int found = 0;

    printf("\n====== Login ======\n");
    printf("Username: ");
    scanf("%s", input.username);
    printf("Password: ");
    scanf("%s", input.password);

    while (fread(&user, sizeof(user), 1, fp)) {
        if (strcmp(input.username, user.username) == 0 &&
            strcmp(input.password, user.password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Incorrect username or password.\n");
        return 0;
    }
}

// ------------------- Bank Functions -------------------
void createAccount() {
    FILE *fp = fopen("accounts.dat", "ab");
    struct Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accNumber);
    printf("Enter Account Holder Name: ");
    scanf("%s", acc.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("Account created successfully.\n");
}

void viewAccount() {
    FILE *fp = fopen("accounts.dat", "rb");
    struct Account acc;
    int accNum, found = 0;

    printf("\nEnter Account Number to View: ");
    scanf("%d", &accNum);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNum) {
            printf("Account Number: %d\n", acc.accNumber);
            printf("Name: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found.\n");

    fclose(fp);
}

void depositMoney() {
    FILE *fp = fopen("accounts.dat", "rb+");
    struct Account acc;
    int accNum, found = 0;
    float amount;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNum) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            acc.balance += amount;

            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            found = 1;
            printf("Deposit successful. New balance: %.2f\n", acc.balance);
            break;
        }
    }

    if (!found)
        printf("Account not found.\n");

    fclose(fp);
}

void withdrawMoney() {
    FILE *fp = fopen("accounts.dat", "rb+");
    struct Account acc;
    int accNum, found = 0;
    float amount;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNum) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            if (acc.balance >= amount) {
                acc.balance -= amount;

                fseek(fp, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);
                printf("Withdrawal successful. New balance: %.2f\n", acc.balance);
            } else {
                printf("Insufficient balance.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found.\n");

    fclose(fp);
}

void deleteAccount() {
    FILE *fp = fopen("accounts.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Account acc;
    int accNum, found = 0;

    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &accNum);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNum) {
            found = 1;
        } else {
            fwrite(&acc, sizeof(acc), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (found)
        printf("Account deleted successfully.\n");
    else
        printf("Account not found.\n");
}

// ------------------- Main Menu -------------------
void bankMenu() {
    int choice;

    while (1) {
        printf("\n===== Bank Management Menu =====\n");
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Delete Account\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccount(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: deleteAccount(); break;
            case 6: return; // exit to main menu
            default: printf("Invalid choice!\n");
        }
    }
}

void mainMenu() {
    int choice, loggedIn = 0;

    while (1) {
        printf("\n===== Welcome =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: registerUser(); break;
            case 2: 
                loggedIn = login(); 
                if (loggedIn) bankMenu(); 
                break;
            case 3: printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

// ------------------- Main Function -------------------
int main() {
    mainMenu();
    return 0;
}
