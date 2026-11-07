# Roles

Project: Secure Multi-Client Warehouse Inventory System (C, single main.c)

We split the work into three parts so each of us can build and test our own piece without waiting on the others. Function names and signatures follow the README exactly, so the parts fit together at the end.

## Deepa Rani: Menus and Inventory

Everything the client sees and uses after opening the program.

- readLine() and readInt()
- addItem(), viewInventory(), searchItem(), updateItem(), removeItem()
- mainMenu() and clientMenu()
- Checking input: SKUs must be unique, quantities must be valid, and a wrong menu choice shouldn't crash anything
- Testing: two clients only see their own inventory, and invalid menu input is handled

Since this is the part people actually interact with, it's what we'll show in the demo. readLine() and readInt() are needed by the others too, so they should be done first.

## Mubahir: Ciphers and Hashing

The security side of the project.

- vigenereEncrypt() and vigenereDecrypt()
- railFenceEncrypt() and railFenceDecrypt()
- doubleEncrypt() and doubleDecrypt() (decrypt runs in reverse order: Rail Fence first, then Vigenere)
- makeSalt() and hashPassword() (one-way hash, output as hex text)
- Testing: encrypt then decrypt gives back the original for short text, long text, spaces and symbols, and the same password with a different salt gives a different hash

## Jawad: Storage, Accounts and Merging

The part that connects everything into one working program.

- Structs, globals and constants (Client, Item, MAX_CLIENTS, etc.)
- saveCreds() / loadCreds(), saveProfiles() / loadProfiles(), saveInventory() / loadInventory()
- registerClient(), loginClient(), logoutClient()
- viewProfile() and updateProfile()
- main(), merging everything into the final main.c, and the README
- Testing: creds.dat and profiles.dat are unreadable, duplicate usernames are rejected, wrong passwords are refused, missing data files don't break startup

## How we'll work

1. Day 1, all together: agree on the structs, globals and function signatures. Jawad puts them in a shared starter file.
2. Everyone writes quick dummy versions (stubs) of the functions they depend on, for example a doubleEncrypt() that does nothing, so each person can compile and test right away.
3. Each person works in their own .c file with a small temporary main().
4. At the end we paste everything into one main.c, with a marked section for each part (/* ===== CIPHERS ===== */ and so on), swap the stubs for the real code, and run the full testing checklist together.

## Who needs what from whom

- Jawad's registerClient() and loginClient() use Deepa's readLine() and readInt().
- Jawad's save/load code and login use Mubahir's doubleEncrypt(), doubleDecrypt() and hashPassword().
- Deepa's inventory functions call Jawad's saveInventory() and loadInventory().
