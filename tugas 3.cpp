#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define TABLE_SIZE 10

struct node {
    char data[100];
    struct node *next;
};

struct HashTable {
    node** table;
    int size;
};

HashTable* createHashTable(int size) {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->size = size;
    hashTable->table = (node**)malloc(sizeof(node*) * size);
    for (int i = 0; i < size; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}


int hashFunctionFirstChar(char data[]) {
    int hashKey = -1;
    char firstChar;
    firstChar = data[0];
    hashKey = tolower(firstChar) - 97;
    return hashKey;
}

int divisionHash(int key, int tableSize) {
    return key % tableSize;
}

int foldingHash(int key, int tableSize) {
    int part1 = key / 1000; 
    int part2 = key % 1000; 
    return (part1 + part2) % tableSize;
}

int midSquareHash(int key, int tableSize) {
    int square = key * key;
   
    char squareStr[20]; 
    sprintf(squareStr, "%d", square);
    int length = strlen(squareStr);

    int start = (length / 2) - 1; 
    char middle[3]; 
    strncpy(middle, squareStr + start, 2);
    middle[2] = '\0';
    return atoi(middle) % tableSize;
}

int digitExtractionHash(int key, int tableSize) {
    char keyStr[20];
    sprintf(keyStr, "%d", key);

    char extractedDigits[3];
    extractedDigits[0] = keyStr[0]; 
    extractedDigits[1] = keyStr[strlen(keyStr) - 1]; 
    extractedDigits[2] = '\0';
   
    return atoi(extractedDigits) % tableSize;
}

int rotationHash(int key, int tableSize) {
    char keyStr[20];
    sprintf(keyStr, "%d", key);
    int length = strlen(keyStr);
    int rotatedKey = 0;
    for (int i = 0; i < length; i++) {
        rotatedKey += (int)keyStr[i];
    }
    return rotatedKey % tableSize;
}

int linearProbing(int hashKey, int i, int tableSize) {
    return (hashKey + i) % tableSize;
}


int rehashing(int key, int i, int tableSize) {
    return (key + i * i) % tableSize;
}


void insertChaining(HashTable* hashTable, int hashKey, char data[]) {
    node* newNode = (node*)malloc(sizeof(node));
    strcpy(newNode->data, data);
    newNode->next = NULL;

    if (hashTable->table[hashKey] == NULL) {
        hashTable->table[hashKey] = newNode;
    }

    else {
        node* curr = hashTable->table[hashKey];
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = newNode;
    }
}

void viewHashTable(HashTable* hashTable) {
 
    for (int i = 0; i < hashTable->size; i++) {
        if (hashTable->table[i] != NULL) {
            printf("Hash Table Index [%d] :\n", i);
            node* curr = hashTable->table[i];
            while (curr != NULL) {
                printf("%s ", curr->data);
                curr = curr->next;
            }
            printf("\n");
        }
    }
}

int main() {
    int hashMethod, collisionMethod;
    int tableSize;

    printf("Masukkan ukuran tabel hash: ");
    scanf("%d", &tableSize);

    HashTable* hashTable = createHashTable(tableSize);

    printf("Pilih metode hash function:\n");
    printf("1. First Character\n");
    printf("2. Division\n");
    printf("3. Folding\n");
    printf("4. Mid-Square\n");
    printf("5. Digit Extraction\n");
    printf("6. Rotation\n");
    printf("Masukkan pilihan: ");
    scanf("%d", &hashMethod);

    printf("Pilih metode collision handling:\n");
    printf("1. Linear Probing\n");
    printf("2. Rehashing\n");
    printf("3. Chaining\n");
    printf("Masukkan pilihan: ");
    scanf("%d", &collisionMethod);

    char data[100];
    do {
        printf("Input Data: ");
        scanf("%s", data); getchar();

        char input[100];
        strcpy(input, data);
        for (int i = 0; input[i]; i++) {
            input[i] = tolower(input[i]);
        }
        if (strcmp(input, "exit") == 0) break;

        int hashKey;
        switch (hashMethod) {
        case 1:
            hashKey = hashFunctionFirstChar(data);
            break;
        case 2:
            hashKey = divisionHash(atoi(data), tableSize);
            break;
        case 3:
            hashKey = foldingHash(atoi(data), tableSize);
            break;
        case 4:
            hashKey = midSquareHash(atoi(data), tableSize);
            break;
        case 5:
            hashKey = digitExtractionHash(atoi(data), tableSize);
            break;
        case 6:
            hashKey = rotationHash(atoi(data), tableSize);
            break;
        default:
            printf("Pilihan tidak valid.\n");
            return 1;
        }

        switch (collisionMethod) {
        case 1: 
        {
            int i = 0;
            int newHashKey = hashKey % tableSize;
            while (hashTable->table[newHashKey] != NULL && i < tableSize) {
                i++;
                newHashKey = linearProbing(hashKey, i, tableSize);
            }
            if (i == tableSize) {
                printf("Tabel sudah penuh, tidak dapat menambahkan data.\n");
            }
            else {
                insertChaining(hashTable, newHashKey, data); 
            }
            break;
        }
        case 2: 
        {
            int i = 0;
            int newHashKey = hashKey % tableSize;
            while (hashTable->table[newHashKey] != NULL && i < tableSize) {
                i++;
                newHashKey = rehashing(hashKey, i, tableSize);
                newHashKey %= tableSize;
            }
            if (i == tableSize) {
                printf("Tabel sudah penuh, tidak dapat menambahkan data.\n");
            }
            else {
                insertChaining(hashTable, newHashKey, data); 
            }
            break;
        }
        case 3: 
            insertChaining(hashTable, hashKey % tableSize, data);
            break;
        default:
            printf("Pilihan tidak valid.\n");
            return 1;
        }
    } while (true);

    viewHashTable(hashTable);

    getchar();
    return 0;
}
