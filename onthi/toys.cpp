#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    int id;
    char toy[30];
} Child;

typedef struct Node {
    Child child;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(Child child) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->child = child;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insertNode(Node** root, Child child) {
    if (*root == NULL) {
        *root = createNode(child);
        return;
    }

    if (child.id < (*root)->child.id) {
        insertNode(&(*root)->left, child);
    }
    else if (child.id > (*root)->child.id) {
        insertNode(&(*root)->right, child);
    }
    else {
        printf("Duplicate ID: %d\n", child.id);
    }
}

void printInOrder(Node* root) {
    if (root != NULL) {
        printInOrder(root->left);
        printf("ID: %d, Toy: %s\n", root->child.id, root->child.toy);
        printInOrder(root->right);
    }
}

void readAFile(Node** root) {
    FILE* file = fopen("A.txt", "r");
    if (file == NULL) {
        printf("Cannot open file A.txt\n");
        return;
    }

    int id;
    char toy[30];
    while (fscanf(file, " %d %[^\n]", &id, toy) == 2) {
        Child child;
        child.id = id;
        strcpy(child.toy, toy);
        insertNode(root, child);
    }

    fclose(file);
}

Child promptChildInfo() {
    Child child;
    printf("Enter ID: ");
    scanf("%d", &child.id);
    printf("Enter toy: ");
    scanf(" %[^\n]", child.toy);
    return child;
}

void readAFileWithPrompt(Node** root) {
    FILE* file = fopen("A.txt", "r");
    if (file == NULL) {
        printf("Cannot open file A.txt\n");
        return;
    }

    int id;
    char toy[30];
    while (fscanf(file, " %d %[^\n]", &id, toy) >= 1) {
        if (strlen(toy) == 0) {
            Child child = promptChildInfo();
            child.id = id;
            insertNode(root, child);
        }
        else {
            Child child;
            child.id = id;
            strcpy(child.toy, toy);
            insertNode(root, child);
        }
    }

    fclose(file);
}

void readBFile(Child* array, int* count) {
    FILE* file = fopen("B.txt", "r");
    if (file == NULL) {
        printf("Cannot open file B.txt\n");
        return;
    }

    int id;
    char toy[30];
    while (fscanf(file, " %d %[^\n]", &id, toy) == 2) {
        Child child;
        child.id = id;
        strcpy(child.toy, toy);
        array[*count] = child;
        (*count)++;
        printf("ID: %d, Toy: %s\n", child.id, child.toy);
    }

    fclose(file);
}
void searchAndDelete(Node** root, Child child) {
    if (*root == NULL) {
        return;
    }

    if (child.id < (*root)->child.id) {
        searchAndDelete(&(*root)->left, child);
    }
    else if (child.id > (*root)->child.id) {
        searchAndDelete(&(*root)->right, child);
    }
    else {
        printf("Duplicate found - ID: %d, Toy: %s\n", (*root)->child.id, (*root)->child.toy);
        Node* temp = *root;
        if ((*root)->left == NULL) {
            *root = (*root)->right;
        }
        else if ((*root)->right == NULL) {
            *root = (*root)->left;
        }
        else {
            Node* minRight = (*root)->right;
            while (minRight->left != NULL) {
                minRight = minRight->left;
            }
            (*root)->child = minRight->child;
            searchAndDelete(&(*root)->right, minRight->child);
        }
        free(temp);
    }
}

void mergeArrays(Node** root, Child* array, int count) {
    for (int i = 0; i < count; i++) {
        insertNode(root, array[i]);
    }
}

void printToyStats(Node* root) {
    if (root != NULL) {
        printToyStats(root->right);
        printf("Toy: %s, Count: %d\n", root->child.toy, 1);
        printToyStats(root->left);
    }
}

int main() {
    Node* rootA = NULL;
    Child arrayB[MAX_SIZE];
    int countB = 0;

    int choice;
    do {
        printf("Menu:\n");
        printf("1. Read file A\n");
        printf("2. Read file B\n");
        printf("3. Search and delete\n");
        printf("4. Merge arrays\n");
        printf("5. Print toy stats\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                readAFile(&rootA);
                break;
            case 2:
                readBFile(arrayB, &countB);
                break;
            case 3:
                if (rootA == NULL) {
                    printf("File A has not been read yet.\n");
                }
                else {
                    int id;
                    printf("Enter ID to search and delete: ");
                    scanf("%d", &id);
                    Child child;
                    child.id = id;
                    searchAndDelete(&rootA, child);
                }
                break;
            case 4:
                mergeArrays(&rootA, arrayB, countB);
                break;
            case 5:
                if (rootA == NULL) {
                    printf("File A has not been read yet.\n");
                }
                else {
                    printf("Toy stats:\n");
                    printToyStats(rootA);
                }
                break;
            case 0:
                break;
            default:
                printf("Invalid option.\n");
                break;
        }

        printf("\n");
    } while (choice != 0);

    return 0;
}
