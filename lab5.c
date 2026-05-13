#include <stdio.h>
#include <stdlib.h>

// 1. определение структуры узла
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

// структура для очереди 
typedef struct QueueNode {
    Node* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode *front, *rear;
} Queue;

// функции для работы с деревом

Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 2. функция вставки BST
Node* insert(Node* root, int key) {
    if (root == NULL) return createNode(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);

    return root;
}

// 4. функция поиска
Node* search(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    
    if (key < root->key)
        return search(root->left, key);
    
    return search(root->right, key);
}

// 3.Обходы дерева DFS глуб

void preorder(Node* root) { // NLR прямой
    if (root) {
        printf("%d ", root->key);
        preorder(root->left);
        preorder(root->right);
    }
}

void inorder(Node* root) { // LNR цент.
    if (root) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

void postorder(Node* root) { // LRN обрат.
    if (root) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->key);
    }
}

// обход в ширину (BFS) через очередь

void enqueue(Queue* q, Node* n) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->treeNode = n;
    temp->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

Node* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    QueueNode* temp = q->front;
    Node* n = temp->treeNode;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return n;
}

void BFS(Node* root) {
    if (root == NULL) return;
    Queue q = {NULL, NULL};
    enqueue(&q, root);
    while (q.front != NULL) {
        Node* current = dequeue(&q);
        printf("%d ", current->key);
        if (current->left) enqueue(&q, current->left);
        if (current->right) enqueue(&q, current->right);
    }
}

// меню

int main() {
    Node* root = NULL;
    int choice, val;

    printf("Лабораторная работа №5: Бинарные деревья поиска\n");

    while (1) {
        printf("\nМеню:\n");
        printf("1. Добавить элемент\n");
        printf("2. Поиск элемента\n");
        printf("3. Прямой обход (NLR)\n");
        printf("4. Центрированный обход (LNR)\n");
        printf("5. Обратный обход (LRN)\n");
        printf("6. Обход в ширину (BFS)\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf("Введите ключ для вставки: ");
                scanf("%d", &val);
                root = insert(root, val);
                break;
            case 2:
                printf("Введите ключ для поиска: ");
                scanf("%d", &val);
                if (search(root, val)) printf("Результат: Элемент %d найден!\n", val);
                else printf("Результат: Элемент не найден.\n");
                break;
            case 3:
                printf("Preorder (NLR): "); preorder(root); printf("\n");
                break;
            case 4:
                printf("Inorder (LNR): "); inorder(root); printf("\n");
                break;
            case 5:
                printf("Postorder (LRN): "); postorder(root); printf("\n");
                break;
            case 6:
                printf("BFS (по уровням): "); BFS(root); printf("\n");
                break;
            case 0:
                printf("Выход из программы...\n");
                return 0;
            default:
                printf("Неверный ввод. Попробуйте снова.\n");
        }
    }
    return 0;
}
