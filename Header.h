#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>
#include <time.h>
#define CMP_EQ(a, b) ((a) == (b))
#define CMP_LT(a, b) ((a) < (b))
#define CMP_GT(a, b) ((a) > (b))


typedef int T;

typedef struct _Node {
    T data;
    struct _Node* left;
    struct _Node* right;
    struct _Node* parent;
} Node;

typedef struct _QNode {
    struct _QNode* next;
    Node* data;
} QNode;

typedef struct _Queue {
    QNode* head;
    QNode* tail;
    int size;
} Queue;

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->tail = NULL;
    queue->head = NULL;
    queue->size = 0;
    return queue;
}

void push(Queue* queue, Node* data) {
    QNode* qnode = (QNode*)malloc(sizeof(QNode));
    qnode->next = NULL;
    qnode->data = data;

    if (queue->size == 0) {
        queue->tail = qnode;
        queue->head = qnode;
    }
    else {
        queue->tail->next = qnode;
        queue->tail = qnode;
    }
    queue->size++;
}
Node* pop(Queue* queue) {
    if (queue->head == NULL) {
        printf("Queue is empty\n");
        exit(0);
    }
    QNode* temp = queue->head;
    queue->head = temp->next;
    Node* data = temp->data;
    free(temp);
    queue->size--;
    return data;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//создание узла в памяти 
Node* getFreeNode(T value, Node* parent) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->left = NULL;
    temp->right = NULL;
    temp->data = value;
    temp->parent = parent;
    return temp;
}

//добавление нового узла в дерево
void insert(Node** root, T value) {
    Node* temp = NULL;
    if (*root == NULL) {
        *root = getFreeNode(value, NULL);
        return;
    }

    temp = *root;
    while (temp) {  //пока не дошли до пустого узла
        if (CMP_GT(value, temp->data)) {
            if (temp->right) { //если правый потомок не NULL
                temp = temp->right; //двигаемся вправо
                continue;
            }
            else {  //если правый потомок NULL
                temp->right = getFreeNode(value, temp);
                return;
            }
        }
        else if (CMP_LT(value, temp->data)) {
            if (temp->left) {  //если левый потомок не NULL
                temp = temp->left; //двигаемся влево
                continue;
            }
            else { //если левый потомок NULL
                temp->left = getFreeNode(value, temp);
                return;
            }
        }
        else {
            printf("\nThe same numbers");
            exit(0);
        }
    }
}


Node* findMaxNode(Node* root) {
    Node* current = root;
    while (current->right)
        current = current->right;
    return current;
}
Node* findMinNode(Node* root) {
    Node* current = root;
    while (current->left)
        current = current->left;
    return current;
}

Node* getNodeByValue(Node* root, T value) {
    while (root) {
        if (CMP_GT(root->data, value)) {
            root = root->left;
            continue;
        }
        else if (CMP_LT(root->data, value)) {
            root = root->right;
            continue;
        }
        else {
            return root;
        }
    }
    return NULL;
}

void removeNodeByPtr(Node* target) {
    if (target->left && target->right) { //если удаляемый узел имеет два потомка
        Node* localMax = findMaxNode(target->left); //находим максимум поддерева удаляемого узла
        target->data = localMax->data; //заменяем значение в удаляемом узле на максимальное
        removeNodeByPtr(localMax);  //удалеяем найденный максимальный узел 
        return;
    }
    else if (target->left) {  //если удаляемый узел имеет только левого потомка
        if (target == target->parent->left) { //если уд. узел является левым для своего родителя
            target->parent->left = target->left; //заменяем
        }
        else {
            target->parent->right = target->left; //заменяем
        }
    }
    else if (target->right) { //если удаляемый узел имеет только правого потомка
        if (target == target->parent->right) { //если уд. узел является правым для своего родителя
            target->parent->right = target->right;
        }
        else {
            target->parent->left = target->right;
        }
    }
    else {                                           //если уд. узел не имеет потомков
        if (target == target->parent->left) {
            target->parent->left = NULL;
        }
        else {
            target->parent->right = NULL;
        }
    }
    free(target);
}

void removeNodeByValue(Node* root, T value) {
    Node* target = getNodeByValue(root, value);
    removeNodeByPtr(target);
}


void prym_print(Node* root) {
    if (root) {
        printf("%d ", root->data);
        prym_print(root->left);
        prym_print(root->right);
    }
}

void sim_print(Node* root) {
    if (root) {
        sim_print(root->left);
        printf("%d ", root->data);
        sim_print(root->right);
    }
}

void obr_print(Node* root) {
    if (root) {
        obr_print(root->left);
        obr_print(root->right);
        printf("%d ", root->data);
    }
}
void printTree(Node* root) {
    printf("\n\n");
    if (root == NULL) {
        printf("Tree is empty");
        exit(0);
    }
    Queue* queue = createQueue();
    push(queue, root);
    int count = 0;
    while (queue->size != 0) {
        int size = queue->size;
        printf("-%d-  ", count);
        for (int i = 0; i < size; i++) {
            Node* temp = pop(queue);
            if (temp->parent == NULL)
                printf("[%d]NULL  ", temp->data);
            else if (temp == temp->parent->left)
                printf("[%d]L%d  ", temp->data, temp->parent->data);
            else
                printf("[%d]R%d  ", temp->data, temp->parent->data);

            if (temp->left)
                push(queue, temp->left);
            if (temp->right)
                push(queue, temp->right);
        }
        printf("\n");
        count++;
    }
    free(queue);
}


