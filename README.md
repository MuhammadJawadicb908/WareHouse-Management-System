# Secure Multi-Client Warehouse Inventory System

A console program written in C that lets several businesses share one warehouse system while keeping each client's data private. Every client has their own login and their own inventory. Personal details and login data are stored in encrypted form, so the stored files are not readable even if someone opens them directly.

## The problem

When multiple businesses share one warehouse, the system managing their inventory has to keep each client's data private from the others. If login details and personal information are not protected, a single breach could expose every client's data at once, not just one.

## Our approach

Each client gets a separate account. Sensitive data is protected with encryption and careful file organisation, so that even if the storage itself were accessed, the data inside would not be readable.

## Features

1. **Multiple accounts.** Each client has their own username, password and inventory, kept separate from every other client.
2. **Encrypted personal data.** Name, contact details and business details are encrypted before they are written to disk.
3. **Secure login.** Passwords are never stored in readable form. A password is combined with a salt, hashed (one way, cannot be reversed), and the hash is then encrypted. At login the entered password is hashed again and compared with the stored value.
4. **Double cipher stacking.** Sensitive data passes through two different ciphers in sequence: a Vigenere cipher (substitution) followed by a Rail Fence cipher (transposition). An attacker would have to break two independent methods instead of one.
5. **File management.** Data is stored in separate files by type. Login data, personal profiles and each client's inventory are kept apart, so access to one file does not expose the others. Inventory files can be encrypted too, but this is optional because inventory is less sensitive than login or personal data.

## Constraints

The project uses only what has been covered in the course so far.

| Allowed | Used for |
|---|---|
| `stdio.h` | input and output, file handling (`fopen`, `fclose`, `fprintf`, `fscanf`, `fgets`, `sprintf`, `sscanf`, `remove`) |
| `string.h` | `strlen`, `strcmp`, `strcpy`, `strncpy`, `strcat`, `strcspn`, `strstr` |
| `math.h` | available if needed, not required by the current design |
| User defined functions | every feature below is a function written by us |

Not used: `stdlib.h`, `ctype.h`, `time.h`, `conio.h` or any other header. Where a normal program would use them, we write the small piece ourselves:

- No `rand()` or `time()`, so salts are generated from the username and client ID instead of randomly.
- No `ctype.h`, so character checks are written with comparisons such as `c >= 'a' && c <= 'z'`.
- No `exit()` or `atoi()`, so the program ends by returning from `main()` and numbers are read with `fgets` and `sscanf`.
- No `malloc()`, so all storage uses fixed size arrays and structs.

## Data structures

```c
typedef struct {
    int  id;
    char username[30];
    char salt[20];
    char storedHash[40];     /* hash after double encryption */
    char name[50];
    char contact[50];
    char business[50];
} Client;

typedef struct {
    int  sku;
    char name[40];
    int  quantity;
} Item;
```

Global state: `clients[MAX_CLIENTS]`, `clientCount`, `currentClient` (set to -1 when nobody is logged in) and `items[MAX_ITEMS]`, `itemCount` for the logged in client.

## Files on disk

| File | Contents | Protection |
|---|---|---|
| `creds.dat` | client ID, username, salt, hashed password | hash is double encrypted |
| `profiles.dat` | name, contact, business | whole record double encrypted |
| `inv_<id>.dat` | one file per client, items and quantities | optional encryption |

## Planned functions

**Ciphers**

| Function | Purpose |
|---|---|
| `void vigenereEncrypt(char s[], char key[])` | shifts each printable character (ASCII 32 to 126) by the matching key character |
| `void vigenereDecrypt(char s[], char key[])` | reverses the shift |
| `void railFenceEncrypt(char s[], int rails)` | rearranges characters in a zig-zag over the rails |
| `void railFenceDecrypt(char s[], int rails)` | rebuilds the original order |
| `void doubleEncrypt(char s[])` | Vigenere, then Rail Fence |
| `void doubleDecrypt(char s[])` | Rail Fence decrypt, then Vigenere decrypt (reverse order) |

**Passwords**

| Function | Purpose |
|---|---|
| `void makeSalt(char username[], int id, char salt[])` | builds a salt from the username and client ID |
| `void hashPassword(char pwd[], char salt[], char outHex[])` | one way hash of salt plus password, written as hex text |

**Input helpers**

| Function | Purpose |
|---|---|
| `void readLine(char buf[], int size)` | reads a line with `fgets` and removes the newline with `strcspn` |
| `int readInt()` | reads a line and converts it with `sscanf`, so bad input does not break the program |

**Storage**

| Function | Purpose |
|---|---|
| `void saveCreds()` / `void loadCreds()` | writes and reads `creds.dat` |
| `void saveProfiles()` / `void loadProfiles()` | writes and reads `profiles.dat` |
| `void saveInventory(int id)` / `void loadInventory(int id)` | writes and reads `inv_<id>.dat` |

**Accounts**

| Function | Purpose |
|---|---|
| `void registerClient()` | checks limits and duplicates, creates the credentials, profile and an empty inventory file |
| `int loginClient()` | hashes the entered password and compares it with the stored hash, sets `currentClient` |
| `void logoutClient()` | clears `currentClient` and the loaded inventory |

**Inventory and profile**

| Function | Purpose |
|---|---|
| `void addItem()` | adds an item with a unique SKU and a valid quantity |
| `void viewInventory()` | shows the logged in client's items in a table |
| `void searchItem()` | finds items by SKU or by part of the name |
| `void updateItem()` / `void removeItem()` | changes a quantity or deletes an item |
| `void viewProfile()` / `void updateProfile()` | shows or edits the decrypted personal details |

**Menus**

| Function | Purpose |
|---|---|
| `void mainMenu()` | register, login or exit |
| `void clientMenu()` | inventory and profile options after login |
| `int main()` | loads files, runs the menus in a loop, saves on exit |

## How to build and run

```
gcc main.c -o warehouse
./warehouse
```

On Windows with MinGW the program is started with `warehouse.exe`. Run it from the folder where you want the `.dat` files to be created.

## Testing checklist

- Encrypt then decrypt returns the original text, for short text, long text, spaces and symbols.
- The same password with a different salt gives a different hash.
- Open `creds.dat` and `profiles.dat` in a text editor and confirm nothing is readable.
- Two clients each see only their own inventory.
- Duplicate usernames, empty fields and a full client list are rejected.
- A wrong password is refused, and the program does not crash on invalid menu input.
- A missing data file starts the program with an empty list.

## Limitations

This is a learning project. The ciphers and the hash are written by hand to practise C and are not a substitute for real cryptography. A real system would use a standard password hashing method such as bcrypt or Argon2, a random salt and a tested encryption library. The password is also visible while it is typed, because hiding it needs headers we have not covered.

## Expected outcome

A working prototype where each client can log in securely and manage their own inventory, with personal data and credentials protected by layered encryption, and a file layout that limits the damage from any single breach.