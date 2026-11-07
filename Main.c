/*
  Warehouse Inventory System - starter file

  Rules:
  1. Do not change function names or their inputs. (IMMMAAAAAAAAAAAAAAAAA KILLLL YOUUU )
  2. Only write code inside your own section. (DOWNLOAD THE REPO IN YOUR OWN LOCAL COMPUTER FOR GODS SAKE)
  3. Only stdio.h and string.h are allowed. (BHAAI AUR HEADACHE NHI CHAHIYE)

  Build:  gcc main.c -o warehouse (SO BASICALLY YE PASTE KRO APNE TERMINAL MAI AND THEN JAB ./warehouse likhogy tou code run hoga)
*/

#include <stdio.h>
#include <string.h>

/* ========== SHARED PART (Jawad) ========== */

#define MAX_CLIENTS 50
#define MAX_ITEMS 100
#define KEY "WAREHOUSE"   /* key for the Vigenere cipher */
#define RAILS 3           /* number of rails for the Rail Fence cipher */

typedef struct {
    int id;
    char username[30];
    char salt[20];
    char storedHash[40];
    char name[50];
    char contact[50];
    char business[50];
} Client;

typedef struct {
    int sku;
    char name[40];
    int quantity;
} Item;

Client clients[MAX_CLIENTS];
int clientCount = 0;
int currentClient = -1;    /* -1 means nobody is logged in */

Item items[MAX_ITEMS];     /* items of the logged in client */
int itemCount = 0;

/* List of all functions (so any part can call any other part) */

/* Mubahir (LOVE YOU)*/
void vigenereEncrypt(char s[], char key[]);
void vigenereDecrypt(char s[], char key[]);
void railFenceEncrypt(char s[], int rails);
void railFenceDecrypt(char s[], int rails);
void doubleEncrypt(char s[]);
void doubleDecrypt(char s[]);
void makeSalt(char username[], int id, char salt[]);
void hashPassword(char pwd[], char salt[], char outHex[]);

/* Deepa */
void readLine(char buf[], int size);
int readInt();
void addItem();
void viewInventory();
void searchItem();
void updateItem();
void removeItem();
void mainMenu();
void clientMenu();

/* Jawad */
void saveCreds();
void loadCreds();
void saveProfiles();
void loadProfiles();
void saveInventory(int id);
void loadInventory(int id);
void registerClient();
int loginClient();
void logoutClient();
void viewProfile();
void updateProfile();


/* ========== CIPHERS AND HASHING (Mubahir) ========== */
/* These do nothing for now. Replace each one with real code. */

void vigenereEncrypt(char s[], char key[]) {
}

void vigenereDecrypt(char s[], char key[]) {
}

void railFenceEncrypt(char s[], int rails) {
}

void railFenceDecrypt(char s[], int rails) {
}

void doubleEncrypt(char s[]) {
    /* Vigenere first, then Rail Fence */
}

void doubleDecrypt(char s[]) {
    /* Reverse order: Rail Fence first, then Vigenere */
}

void makeSalt(char username[], int id, char salt[]) {
    strcpy(salt, "salt");
}

void hashPassword(char pwd[], char salt[], char outHex[]) {
    /* Temporary: just copies the password. Max 32 characters. */
    strncpy(outHex, pwd, 32);
    outHex[32] = '\0';
}


/* ========== MENUS AND INVENTORY (Deepa) ========== */

/* Simple working versions so everyone can test. Replace if you want. */

void readLine(char buf[], int size) {
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';
}

int readInt() {
    char line[50];
    int n;
    readLine(line, 50);
    if (sscanf(line, "%d", &n) != 1) {
        return -1;
    }
    return n;
}

void addItem() {
    printf("addItem: not done yet\n");
}

void viewInventory() {
    printf("viewInventory: not done yet\n");
}

void searchItem() {
    printf("searchItem: not done yet\n");
}

void updateItem() {
    printf("updateItem: not done yet\n");
}

void removeItem() {
    printf("removeItem: not done yet\n");
}

void clientMenu() {
    printf("clientMenu: not done yet\n");
}

void mainMenu() {
    int choice = 0;

    while (choice != 3) {
        printf("\n1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Choice: ");
        choice = readInt();

        if (choice == 1) {
            registerClient();
        } else if (choice == 2) {
            if (loginClient() == 1) {
                clientMenu();
                logoutClient();
            }
        } else if (choice != 3) {
            printf("Invalid choice.\n");
        }
    }
}


/* ========== STORAGE AND ACCOUNTS (Jawad) ========== */

void saveCreds() {
}

void loadCreds() {
}

void saveProfiles() {
}

void loadProfiles() {
}

void saveInventory(int id) {
}

void loadInventory(int id) {
}

void registerClient() {
    printf("registerClient: not done yet\n");
}

int loginClient() {
    printf("loginClient: not done yet\n");
    return 0;
}

void logoutClient() {
    currentClient = -1;
    itemCount = 0;
}

void viewProfile() {
    printf("viewProfile: not done yet\n");
}

void updateProfile() {
    printf("updateProfile: not done yet\n");
}


/* ========== MAIN (Jawad) ========== */

int main() {
    loadCreds();
    loadProfiles();
    mainMenu();
    saveCreds();
    saveProfiles();
    return 0;
}
