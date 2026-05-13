#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

//баз. функции

Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int key) {
    if (root == NULL) return createNode(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;
}

// Min и Max
Node* findMin(Node* node) {
    Node* current = node;
    while (current && current->left != NULL) current = current->left;
    return current;
}

Node* findMax(Node* node) {
    Node* current = node;
    while (current && current->right != NULL) current = current->right;
    return current;
}

// высота дерева
int getHeight(Node* node) {
    if (node == NULL) return 0;
    int leftH = getHeight(node->left);
    int rightH = getHeight(node->right);
    return (leftH > rightH ? leftH : rightH) + 1;
}

// удаление узла
Node* deleteNode(Node* root, int key) {
    if (root == NULL) return root;

    if (key < root->key) root->left = deleteNode(root->left, key);
    else if (key > root->key) root->right = deleteNode(root->right, key);
    else {
        // узел с одним потомком или без них
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        // узел с двумя потомками: ищу преемника (минимум в правом поддереве)
        Node* temp = findMin(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

// Binary
void savePreorder(Node* root, FILE* fp) {
    if (root != NULL) {
        fwrite(&(root->key), sizeof(int), 1, fp);
        savePreorder(root->left, fp);
        savePreorder(root->right, fp);
    }
}

void saveToFile(Node* root, char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return;
    savePreorder(root, fp);
    fclose(fp);
    printf("Дерево сохранено в %s\n", filename);
}

Node* loadFromFile(char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return NULL;
    Node* root = NULL;
    int val;
    while (fread(&val, sizeof(int), 1, fp)) {
        root = insert(root, val);
    }
    fclose(fp);
    return root;
}

// полная очистка памяти
void freeTree(Node* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// main
int main() {
    Node* root = NULL;
    int choice, val;
    char* filename = "tree.bin"; 

    while (1) {
        printf("1. Добавить  2. Удалить  3. Min/Max  4. Высота  5. Сохранить  6. Загрузить  0. Выход\nВыбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: printf("Ключ: "); scanf("%d", &val); root = insert(root, val); break;
            case 2: printf("Ключ для удаления: "); scanf("%d", &val); root = deleteNode(root, val); break;
            case 3: 
                if (root) printf("Min: %d, Max: %d\n", findMin(root)->key, findMax(root)->key);
                break;
            case 4: printf("Высота дерева: %d\n", getHeight(root)); break;
            case 5: saveToFile(root, filename); break;
            case 6: freeTree(root); root = loadFromFile(filename); break;
            case 0: freeTree(root); return 0;
        }
    }
}