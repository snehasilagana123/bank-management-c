#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int acc_no;
    char name[100];
    float balance;
};

void create_account();
void view_accounts();

int main() {
    int choice;

    do {
        printf("\n--- Bank Management System ---\n");
        printf("1. Create Account\n");
        printf("2. View Accounts\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_account();
                break;
            case 2:
                view_accounts();
                break;
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);

    return 0;
}

void create_account() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "ab");

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc.acc_no);
    printf("Enter Name: ");
    scanf(" %[^\n]", acc.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);
    printf("Account created successfully!\n");
}

void view_accounts() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb");

    if (!fp) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- Account List ---\n");
    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("Acc No: %d, Name: %s, Balance: %.2f\n", acc.acc_no, acc.name, acc.balance);
    }

    fclose(fp);
}
