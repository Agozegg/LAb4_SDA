#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct car {
    int id;
    int year;
    char brand[50];
    char model[50];
    int price;
} car;

typedef struct tree_node {
    car info;
    struct tree_node *left, *right;
} tree_node;

typedef struct queue_node {
    tree_node *node;
    struct queue_node *next;
} queue_node;

typedef struct queue {
    queue_node *front, *back;
} queue;

typedef struct stack_node {
    tree_node *node;
    struct stack_node *next;
} stack_node;

typedef struct stack {
    stack_node *top;
} stack;

char comparison_key[20] = "id";

queue queue_init() {
    queue q = {NULL, NULL};
    return q;
}

void queue_push_back(queue *q, tree_node *n) {
    queue_node *temp = (queue_node*)malloc(sizeof(queue_node));
    temp->node = n;
    temp->next = NULL;
    if (!q->back) {
        q->front = q->back = temp;
        return;
    }
    q->back->next = temp;
    q->back = temp;
}

tree_node* queue_pop_front(queue *q) {
    if (!q->front) return NULL;
    queue_node *temp = q->front;
    tree_node *n = temp->node;
    q->front = q->front->next;
    if (!q->front) q->back = NULL;
    free(temp);
    return n;
}

int queue_is_empty(queue *q) {
    return q->front == NULL;
}

stack stack_init() {
    stack s = {NULL};
    return s;
}

void stack_push(stack *s, tree_node *n) {
    stack_node *temp = (stack_node*)malloc(sizeof(stack_node));
    temp->node = n;
    temp->next = s->top;
    s->top = temp;
}

tree_node* stack_pop(stack *s) {
    if (!s->top) return NULL;
    stack_node *temp = s->top;
    tree_node *n = temp->node;
    s->top = s->top->next;
    free(temp);
    return n;
}

int stack_is_empty(stack *s) {
    return s->top == NULL;
}

void car_print(car c) {
    printf("ID: %d\nYear: %d\nBrand: %s\nModel: %s\nPrice: %d\n\n", c.id, c.year, c.brand, c.model, c.price);
}

int car_compare(car c1, car c2) {
    if (strcmp(comparison_key, "id") == 0) return c1.id - c2.id;
    else if (strcmp(comparison_key, "year") == 0) return c1.year - c2.year;
    else if (strcmp(comparison_key, "price") == 0) return c1.price - c2.price;
    else if (strcmp(comparison_key, "brand") == 0) return strcmp(c1.brand, c2.brand);
    else if (strcmp(comparison_key, "model") == 0) return strcmp(c1.model, c2.model);
    return 0;
}

tree_node* tree_add_node(tree_node *root, car c) {
    if (!root) {
        tree_node *new_node = (tree_node*)malloc(sizeof(tree_node));
        new_node->info = c;
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    int cmp = car_compare(c, root->info);
    if (cmp < 0) root->left = tree_add_node(root->left, c);
    else if (cmp > 0) root->right = tree_add_node(root->right, c);
    return root;
}

tree_node* tree_find(tree_node *root, int id) {
    if (!root) return NULL;
    if (id == root->info.id) return root;
    return id < root->info.id ? tree_find(root->left, id) : tree_find(root->right, id);
}

void tree_display_inorder(tree_node *root) {
    if (root) {
        tree_display_inorder(root->left);
        car_print(root->info);
        tree_display_inorder(root->right);
    }
}

void tree_display_preorder(tree_node *root) {
    if (root) {
        car_print(root->info);
        tree_display_preorder(root->left);
        tree_display_preorder(root->right);
    }
}

void tree_display_postorder(tree_node *root) {
    if (root) {
        tree_display_postorder(root->left);
        tree_display_postorder(root->right);
        car_print(root->info);
    }
}

void tree_bfs(tree_node *root) {
    if (!root) return;
    queue q = queue_init();
    queue_push_back(&q, root);
    while (!queue_is_empty(&q)) {
        tree_node *current = queue_pop_front(&q);
        car_print(current->info);
        if (current->left) queue_push_back(&q, current->left);
        if (current->right) queue_push_back(&q, current->right);
    }
}

void tree_dfs(tree_node *root) {
    if (!root) return;
    stack s = stack_init();
    stack_push(&s, root);
    while (!stack_is_empty(&s)) {
        tree_node *current = stack_pop(&s);
        car_print(current->info);
       if (current->right) stack_push(&s, current->right);
        if (current->left) stack_push(&s, current->left);
    }
}

int tree_count_nodes(tree_node *root) {
    return root ? 1 + tree_count_nodes(root->left) + tree_count_nodes(root->right) : 0;
}

void tree_store_inorder(tree_node *root, car arr[], int *idx) {
    if (root) {
        tree_store_inorder(root->left, arr, idx);
        arr[(*idx)++] = root->info;
        tree_store_inorder(root->right, arr, idx);
    }
}

tree_node* tree_build_balanced(car arr[], int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    tree_node *new_node = (tree_node*)malloc(sizeof(tree_node));
    new_node->info = arr[mid];
    new_node->left = tree_build_balanced(arr, start, mid - 1);
    new_node->right = tree_build_balanced(arr, mid + 1, end);
    return new_node;
}

tree_node* tree_balance(tree_node *root) {
    int n = tree_count_nodes(root);
    if (n == 0) return NULL;
    car *arr = (car*)malloc(n * sizeof(car));
    int idx = 0;
    tree_store_inorder(root, arr, &idx);
    tree_node *new_root = tree_build_balanced(arr, 0, n - 1);
    free(arr);
    return new_root;
}

void tree_mirror(tree_node *root) {
    if (root) {
        tree_node *temp = root->left;
        root->left = root->right;
        root->right = temp;
        tree_mirror(root->left);
        tree_mirror(root->right);
    }
}

void tree_free(tree_node *root) {
    if (!root) return;
    tree_free(root->left);
    tree_free(root->right);
    free(root);
}

int main() {
    tree_node *root = NULL;
    int choice;

    do {
        printf("\n1) Add Car\n2) Display InOrder\n3) Search by ID\n4) Traversals\n");
        printf("5) Balance Tree\n6) Mirror Tree\n7) Clear Tree\n8) Change Key\n0) Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                car c;
                printf("ID: ");
                scanf("%d", &c.id);
                printf("Year: ");
                scanf("%d", &c.year);
                printf("Brand: ");
                scanf(" %49[^\n]", c.brand);
                printf("Model: ");
                scanf(" %49[^\n]", c.model);
                printf("Price: ");
                scanf("%d", &c.price);
                root = tree_add_node(root, c);
                break;
            }
            case 2:
                tree_display_inorder(root);
                break;
            case 3: {
                int id;
                printf("Enter ID to search: ");
                scanf("%d", &id);
                tree_node *found = tree_find(root, id);
                if (found) car_print(found->info);
                else printf("Not found!\n");
                break;
            }
            case 4: {
                int type;
                printf("1) InOrder\n2) PreOrder\n3) PostOrder\n4) DFS\n5) BFS\nChoice: ");
                scanf("%d", &type);
                switch (type) {
                    case 1: tree_display_inorder(root); break;
                    case 2: tree_display_preorder(root); break;
                    case 3: tree_display_postorder(root); break;
                    case 4: tree_dfs(root); break;
                    case 5: tree_bfs(root); break;
                    default: printf("Invalid traversal type!\n");
                }
                break;
            }
            case 5:
                root = tree_balance(root);
                printf("Tree balanced.\n");
                break;
            case 6:
                tree_mirror(root);
                printf("Tree mirrored.\n");
                break;
            case 7:
                tree_free(root);
                root = NULL;
                printf("Tree cleared.\n");
                break;
            case 8:
                printf("Enter key (id / year / price / brand / model): ");
                scanf(" %19[^\n]", comparison_key);
                printf("Key set to: %s\n", comparison_key);
                break;
            case 0:
                tree_free(root);
                printf("Goodbye!\n");
            break;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}

