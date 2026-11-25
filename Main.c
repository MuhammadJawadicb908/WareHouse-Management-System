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

/* shared stuff - Jawad */

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

/* Mubashir (LOVE YOU)*/
void vigenereEncrypt(char s[], char key[]);
void vigenereDecrypt(char s[], char key[]);
int getRail(int i, int rails);
void railFenceEncrypt(char s[], int rails);
void railFenceDecrypt(char s[], int rails);
void doubleEncrypt(char s[]);
void doubleDecrypt(char s[]);
void makeSalt(char username[], int id, char salt[]);
void hashPassword(char pwd[], char salt[], char outHex[]);

/* Deepa */
void readLine(char buf[], int size);
int readInt();
int findItem(int sku);
void addItem();
void viewInventory();
void searchItem();
void updateItem();
void removeItem();
void mainMenu();
void clientMenu();

/* Jawad */
int readFileLine(FILE *fp, char buf[], int size);
int findClient(char username[]);
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


/* Mubashir's part - ciphers and hashing */

#define CIPHER_MAX 1000   /* longest text the Rail Fence can handle (no malloc) */

/* shifts each printable character (ASCII 32-126) forward by the matching key character */
void vigenereEncrypt(char s[], char key[]) {
    int keyLength = strlen(key);
    int i;

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] < 32 || s[i] > 126) {
            continue;                           /* not printable: leave it */
        }
        int letter = s[i] - 32;                 /* character as a number 0-94 */
        int shift  = key[i % keyLength] - 32;   /* % repeats the key */
        s[i] = (letter + shift) % 95 + 32;      /* % 95 wraps round so it stays printable */
    }
}

/* reverses the shift; + 95 stops the number going negative */
void vigenereDecrypt(char s[], char key[]) {
    int keyLength = strlen(key);
    int i;

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] < 32 || s[i] > 126) {
            continue;
        }
        int letter = s[i] - 32;
        int shift  = key[i % keyLength] - 32;
        s[i] = (letter - shift + 95) % 95 + 32;
    }
}

/* which rail position i sits on; with 3 rails the pattern is 0,1,2,1,0,1,2,1,... */
int getRail(int i, int rails) {
    int cycle = 2 * (rails - 1);
    int place = i % cycle;

    if (place < rails) {
        return place;           /* going down */
    }
    return cycle - place;       /* coming back up */
}

/* reads the text rail by rail: "HELLOWORLD", 3 rails -> "HOLELWRDLO" */
void railFenceEncrypt(char s[], int rails) {
    char result[CIPHER_MAX];
    int length = strlen(s);
    int next = 0;
    int rail, i;

    if (rails < 2 || length >= CIPHER_MAX) {
        return;
    }

    for (rail = 0; rail < rails; rail++) {
        for (i = 0; i < length; i++) {
            if (getRail(i, rails) == rail) {
                result[next] = s[i];
                next++;
            }
        }
    }
    result[length] = '\0';
    strcpy(s, result);
}

/* same loops as encrypt, but puts each character back at its position */
void railFenceDecrypt(char s[], int rails) {
    char result[CIPHER_MAX];
    int length = strlen(s);
    int next = 0;
    int rail, i;

    if (rails < 2 || length >= CIPHER_MAX) {
        return;
    }

    for (rail = 0; rail < rails; rail++) {
        for (i = 0; i < length; i++) {
            if (getRail(i, rails) == rail) {
                result[i] = s[next];
                next++;
            }
        }
    }
    result[length] = '\0';
    strcpy(s, result);
}

/* Vigenere first, then Rail Fence */
void doubleEncrypt(char s[]) {
    vigenereEncrypt(s, KEY);
    railFenceEncrypt(s, RAILS);
}

/* reverse order: Rail Fence first, then Vigenere */
void doubleDecrypt(char s[]) {
    railFenceDecrypt(s, RAILS);
    vigenereDecrypt(s, KEY);
}

/* salt = client id + first 8 letters of the username, e.g. "1ali"; fits in salt[20] */
void makeSalt(char username[], int id, char salt[]) {
    sprintf(salt, "%d%.8s", id, username);
}

/* scrambles a number so a one-character change alters the whole hash */
unsigned int stir(unsigned int n) {
    n = n ^ (n >> 16);
    n = n * 0x45d9f3b;
    n = n ^ (n >> 16);
    n = n * 0x45d9f3b;
    n = n ^ (n >> 16);
    return n;
}

/* one-way hash of salt + password, written as 16 hex characters */
void hashPassword(char pwd[], char salt[], char outHex[]) {
    unsigned int first  = 5381;
    unsigned int second = 7919;
    int i;

    for (i = 0; salt[i] != '\0'; i++) {
        first  = first  * 31 + salt[i];         /* overflow wraps round, losing information: can't be reversed */
        second = second * 37 + salt[i];
    }
    for (i = 0; pwd[i] != '\0'; i++) {
        first  = first  * 31 + pwd[i];
        second = second * 37 + pwd[i];
    }

    first  = stir(first);
    second = stir(second ^ first);

    sprintf(outHex, "%08x%08x", first, second);
}


/* Deepa's part - menus and inventory */

/* reads a line and removes the newline */
void readLine(char buf[], int size) {
    int c;

    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    if (strchr(buf, '\n') == NULL) {
        c = getchar();
        while (c != '\n' && c != EOF) {
            c = getchar();
        }
    }
    buf[strcspn(buf, "\n")] = '\0';
}

/* reads a number, returns -1 if the input is not a number */
int readInt() {
    char line[50];
    int n;

    readLine(line, 50);
    if (sscanf(line, "%d", &n) != 1) {
        return -1;
    }
    return n;
}

/* returns the position of an SKU, or -1 if not found */
int findItem(int sku) {
    int i;

    for (i = 0; i < itemCount; i++) {
        if (items[i].sku == sku) {
            return i;
        }
    }
    return -1;
}

/* adds an item after checking sku, name and quantity */
void addItem() {
    int sku, qty;
    char name[40];

    if (itemCount >= MAX_ITEMS) {
        printf("Inventory is full.\n");
        return;
    }

    printf("Enter SKU (a positive number): ");
    sku = readInt();
    if (sku <= 0) {
        printf("Invalid SKU.\n");
        return;
    }
    if (findItem(sku) != -1) {
        printf("This SKU already exists.\n");
        return;
    }

    printf("Enter item name: ");
    readLine(name, 40);
    if (strlen(name) == 0) {
        printf("Name cannot be empty.\n");
        return;
    }

    printf("Enter quantity (0 or more): ");
    qty = readInt();
    if (qty < 0) {
        printf("Invalid quantity.\n");
        return;
    }

    items[itemCount].sku = sku;
    strcpy(items[itemCount].name, name);
    items[itemCount].quantity = qty;
    itemCount++;

    saveInventory(clients[currentClient].id);
    printf("Item added.\n");
}

/* prints all items as a table */
void viewInventory() {
    int i;

    if (itemCount == 0) {
        printf("Your inventory is empty.\n");
        return;
    }

    printf("\n%-10s %-40s %-10s\n", "SKU", "Name", "Quantity");
    for (i = 0; i < itemCount; i++) {
        printf("%-10d %-40s %-10d\n", items[i].sku, items[i].name, items[i].quantity);
    }
}

/* search by SKU or by part of the name */
void searchItem() {
    int choice, sku, i;
    int found = 0;
    char part[40];

    printf("1. Search by SKU\n");
    printf("2. Search by name\n");
    printf("Choice: ");
    choice = readInt();

    if (choice == 1) {
        printf("Enter SKU: ");
        sku = readInt();
        i = findItem(sku);
        if (i == -1) {
            printf("Item not found.\n");
        } else {
            printf("SKU: %d | Name: %s | Quantity: %d\n",
                   items[i].sku, items[i].name, items[i].quantity);
        }
    } else if (choice == 2) {
        printf("Enter part of the name: ");
        readLine(part, 40);
        if (strlen(part) == 0) {
            printf("Nothing entered.\n");
            return;
        }
        for (i = 0; i < itemCount; i++) {
            if (strstr(items[i].name, part) != NULL) {
                printf("SKU: %d | Name: %s | Quantity: %d\n",
                       items[i].sku, items[i].name, items[i].quantity);
                found = 1;
            }
        }
        if (found == 0) {
            printf("No item matches.\n");
        }
    } else {
        printf("Invalid choice.\n");
    }
}

/* changes the quantity of an item */
void updateItem() {
    int sku, i, qty;

    printf("Enter SKU of the item: ");
    sku = readInt();
    i = findItem(sku);
    if (i == -1) {
        printf("Item not found.\n");
        return;
    }

    printf("Current quantity is %d. Enter new quantity: ", items[i].quantity);
    qty = readInt();
    if (qty < 0) {
        printf("Invalid quantity.\n");
        return;
    }

    items[i].quantity = qty;
    saveInventory(clients[currentClient].id);
    printf("Quantity updated.\n");
}

/* deletes an item and shifts the rest left */
void removeItem() {
    int sku, i, pos;

    printf("Enter SKU of the item to remove: ");
    sku = readInt();
    pos = findItem(sku);
    if (pos == -1) {
        printf("Item not found.\n");
        return;
    }

    for (i = pos; i < itemCount - 1; i++) {
        items[i] = items[i + 1];
    }
    itemCount--;

    saveInventory(clients[currentClient].id);
    printf("Item removed.\n");
}

/* menu shown after login */
void clientMenu() {
    int choice = 0;

    while (choice != 8) {
        printf("\n===== Client Menu =====\n");
        printf("1. Add item\n");
        printf("2. View inventory\n");
        printf("3. Search item\n");
        printf("4. Update quantity\n");
        printf("5. Remove item\n");
        printf("6. View profile\n");
        printf("7. Update profile\n");
        printf("8. Logout\n");
        printf("Choice: ");
        choice = readInt();

        if (choice == 1) {
            addItem();
        } else if (choice == 2) {
            viewInventory();
        } else if (choice == 3) {
            searchItem();
        } else if (choice == 4) {
            updateItem();
        } else if (choice == 5) {
            removeItem();
        } else if (choice == 6) {
            viewProfile();
        } else if (choice == 7) {
            updateProfile();
        } else if (choice != 8) {
            printf("Invalid choice.\n");
        }
    }
}

/* register, login or exit */
void mainMenu() {
    int choice = 0;

    while (choice != 3) {
        printf("\n===== Warehouse System =====\n");
        printf("1. Register\n");
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


/* Jawad's part - storage and accounts */

int readFileLine(FILE *fp, char buf[], int size) {
    return 0;
}

int findClient(char username[]) {
    return -1;
}

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


/* main - Jawad */

int main() {
    loadCreds();
    loadProfiles();
    mainMenu();
    saveCreds();
    saveProfiles();
    return 0;
}
