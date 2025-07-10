#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int value;
    struct node *left, *right;
    int height;
}Node;

Node* newNode(int x);
int bigger(int a,int b);
int GetHeight(Node* node);
Node* rotationLeft(Node* node);
Node* rotationRight(Node* node);
Node* rotationRightLeft(Node* node);
Node* rotationLeftRight(Node* node);
Node* balance(Node* root);
Node* insertNode(Node* root, int x);
Node* removeNode(Node* root, int key);
Node* searchNode(Node* root, int key);
void printTreeAVL(Node* root, int level);

typedef struct Tree {
    int numberNodes;// Numero de nos da arvove
    Node     *root;// No raiz da arvore
    int      depth;// profundidade da arvoce
} Tree;

Tree* createAVLTree();
void insertAVL(Tree* tree, int x);
void removeAVL(Tree* tree, int key);
void printAVL(Tree* tree);
void searchAVL(Tree* tree, int key);

void updateTreeMetrics(Tree* tree);

/**
 * @brief Cria um Node.
 * @param x O data a ser inserido.
 */
Node* newNode(int x){
    Node* new = malloc(sizeof(Node));

    if(new){
        new->value = x;
        new->left = NULL;
        new->right = NULL;
        new->height = 0;
    }
    else{
        printf("\nErro ao alocar memória!\n");
        free(new);
    }
    return new;
}

/**
 * @brief Calcula o maior valor dentre dois inteiros.
 * @param a O primeiro valor.
 * @param b O segundo valor.
 */
int bigger(int a, int b){
    return (a > b) ? a: b;
}


/**
 * @brief Retorna a altura do Nó ou -1 caso ele seja null
 * @param node O ponteiro para o Node
 */
int GetHeight(Node* node){
    if(node == NULL)
        return -1;
    else
        return node->height;
}

/**
 * @brief Calcula o Fator de balanceamento da Arvore
 * @param node O ponteiro para o Node 
 */
int balancingFactor(Node* node){
    if(node)
        return (GetHeight(node->left) - GetHeight(node->right));
    else
        return 0;
}

/**
 * @brief Rotaciona a arvore à esquerda
 * @param node O ponteiro para o Node
 */
Node* rotationLeft(Node* node){
    Node *y, *f;
    y = node->right;
    f = y->left;

    y->left = node;
    node->right = f;

    node->height = bigger(GetHeight(node->left), GetHeight(node->right)) + 1;
    y->height = bigger(GetHeight(y->left), GetHeight(y->right)) + 1;

    return y;
}

/**
 * @brief Rotaciona a arvore à direita
 * @param node O ponteiro para o Node
 */
Node* rotationRight(Node* node){
    Node *y, *f;

    y = node->left;
    f = y->right;

    y->right = node;
    node->left = f;

    node->height = bigger(GetHeight(node->left), GetHeight(node->right)) + 1;
    y->height = bigger(GetHeight(y->left), GetHeight(y->right)) + 1;

    return y;
}


Node* rotationRightLeft(Node* node){
    node->right = rotationRight(node->right);
    return rotationLeft(node);
}


Node* rotationLeftRight(Node* node){
    node->left = rotationLeft(node->left);
    return rotationRight(node);
}

/**
 * @brief Balanceia a Arvore
 * @param node O ponteiro para o root da Arvore
 */
Node* balance(Node* root){
    int fb = balancingFactor(root);
    
    //Caso ja esteja balanceado ele nem faz nada!
    if(fb == 0)
        return root;

    //Rotação à esquerda
    if(fb < -1 && balancingFactor(root->right) <= 0)
        root = rotationLeft(root);

    //Rotação à direita
    else if(fb > 1 && balancingFactor(root->left) >= 0)
        root = rotationRight(root);
    
    //Rotação dupla à esquerda
    else if(fb > 1 && balancingFactor(root->left) < 0)
        root = rotationLeftRight(root);

    //Rotação dupla à direita
    else if(fb < -1 && balancingFactor(root->right) > 0)
        root = rotationRightLeft(root);

    return root;
}

/**
 * @brief Insere o node
 * @param node O ponteiro para o Node
 * @param x O data do Node
 */
Node* insertNode(Node* root, int x){
    if(root == NULL)
        return newNode(x);
    
    else{
        if(x < root->value)
            root->left = insertNode(root->left, x);
        else if(x > root->value)
            root->right = insertNode(root->right, x);
        else
            printf("\nInserção não realizada, elemento ja existe!\n");
    }

    //Ele recalcula a altura do nó raiz
    root->height = bigger(GetHeight(root->left), GetHeight(root->right)) + 1;

    //Ele verifica se precisa balancear ou não
    root = balance(root);
    
    return root;
}

//Forma de remoção bem mais complexa, no caso ele busca em específico o valor e logo após faz o balanceamento
/**
 * @brief Forma de remoção bem mais complexa, no caso ele busca em específico o valor e logo após faz o balanceamento
 * @param node O ponteiro para o Node
 * @param key O que iremos remover
 */
Node* removeNode(Node* root, int key){
    if(root == NULL){
        printf("Value not found!\n");
        return NULL;
    }
    //Procurando o Nó para remover
    else{
        if(root->value == key){
            //Se o No não tiver filhos
            if(root->left == NULL && root->right == NULL){
                free(root);
                printf("Elemente folha removido: %d !\n", key);
                return NULL;
            }
            //Se o No tiver os dois filhos
            else{
                if(root->left != NULL && root->right != NULL){
                    Node* aux = root->left;
                    while(aux->right != NULL)
                        aux = aux->right;
                    root->value = aux->value;
                    aux->value = key;
                    printf("Elemento trocado: %d !\n", key);
                    root->left = removeNode(root->left, key);
                    return root;
                }
                else{
                    Node* aux;
                    if(root->left != NULL)
                        aux = root->left;
                    else
                        aux = root->right;
                    free(root);
                    printf("Elemento com 1 filho removido: %d !\n", key);
                    return aux;
                }
            }
        }
        else{
            if(key < root->value)
                root->left = removeNode(root->left, key);
            else
                root->right = removeNode(root->right, key);
        }

        root->height = bigger(GetHeight(root->left), GetHeight(root->right)) + 1;

        root = balance(root);

        return root;
    }
}

/**
 * @brief Procura um valor específico na subárvore AVL.
 * @param root O ponteiro para a raiz da subárvore.
 * @param key O valor a ser procurado.
 * @return Um ponteiro para o Node que contém o valor, ou NULL se o valor não for encontrado.
 */
Node* searchNode(Node* root, int key) {
    if (root == NULL || root->value == key) {
        return root; // Retorna o nó se encontrado ou NULL se a subárvore for vazia
    }

    if (key < root->value) {
        return searchNode(root->left, key); // Busca na subárvore esquerda
    } else {
        return searchNode(root->right, key); // Busca na subárvore direita
    }
}

/**
 * @brief Imprime a arvore Rotacionada 90 graus
 * @param node O ponteiro para o Node root
 * @param level Level inicial para printar
 */
void printTreeAVL(Node* root, int level){
    int i;
    if(root){
        printTreeAVL(root->right, level + 1);
        printf("\n\n");

        for(i = 0; i < level; i++)
            printf("\t");

        printf("%d", root->value);
        printTreeAVL(root->left, level + 1);
    }
}

/**
 * @brief Cria o ponteiro para Arvore
 */
Tree* createAVLTree() {
    Tree* newTree = malloc(sizeof(Tree));
    if (newTree) {
        newTree->root = NULL;
        newTree->numberNodes = 0;
        newTree->depth = -1; // -1 para árvore vazia
    } else {
        printf("\nErro ao alocar memória para a Árvore!\n");
    }
    return newTree;
}

/**
 * @brief Insere um valor na Arvore e cria um Node com esse valor.
 * @param tree O ponteiro para a estrutura Tree.
 * @param x O valor a ser colocado.
 */
void insertAVL(Tree* tree, int x) {
    if (tree == NULL) {
        printf("Erro: Árvore não inicializada.\n");
        return;
    }
    Node* oldRoot = tree->root;
    tree->root = insertNode(tree->root, x);
    
    // Se a raiz mudou ou um novo nó foi realmente inserido (não duplicado)
    // Uma forma mais robusta seria ter insertNode retornar um status (inserido/duplicado)
    if (tree->root != oldRoot || (tree->root && tree->root->value != x && oldRoot == NULL)) {
        tree->numberNodes++; // Incrementa o número de nós apenas se a inserção foi bem-sucedida e não é um duplicado
    }
    tree->numberNodes++;
    updateTreeMetrics(tree);
}

/**
 * @brief Remove um valor da árvore AVL.
 * @param tree O ponteiro para a estrutura Tree.
 * @param key O valor a ser removido.
 */
void removeAVL(Tree* tree, int key) {
    if (tree == NULL) {
        printf("Erro: Árvore não inicializada.\n");
        return;
    }
    Node* oldRoot = tree->root;
    tree->root = removeNode(tree->root, key);
    tree->numberNodes--;
    updateTreeMetrics(tree); // Atualiza métricas após a remoção
}

/**
 * @brief Imprime a árvore AVL.
 * @param tree O ponteiro para a estrutura Tree.
 */
void printAVL(Tree* tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("\nEmpty Tree!\n");
        return;
    }
    printTreeAVL(tree->root, 1);
}

/**
 * @brief Atualiza as métricas da árvore (número de nós e profundidade).
 * @param tree O ponteiro para a estrutura Tree.
 * @details Esta função recalcula o número de nós e a profundidade da árvore.
 */
void updateTreeMetrics(Tree* tree) {
    if (tree == NULL) return;
    tree->depth = GetHeight(tree->root);
}

/**
 * @brief Procura um valor na árvore AVL e exibe uma mensagem.
 * @param tree O ponteiro para a estrutura Tree.
 * @param key O valor a ser procurado.
 */
void searchAVL(Tree* tree, int key) {
    if (tree == NULL) {
        printf("\tErro: Árvore não inicializada.\n");
        return;
    }
    if (tree->root == NULL) {
        printf("\tEmpty Tree.The value %d can't be found.\n", key);
        return;
    }

    Node* foundNode = searchNode(tree->root, key);
    if (foundNode != NULL) {
        printf("\tValue %d found in Tree!\n", key);
    } else {
        printf("\tValue %d not found in Tree.\n", key);
    }
}

int main(){
    int option, value;
    Tree* AVL_Tree = createAVLTree();
    option = -1;
    do{
        printf("\n\n\t0 - Exit\n\t1 - Insert\n\t2 - Remove\n\t3 - Print Tree\n\t4 - Search Value\n\n");
        scanf("%d", &option);

        switch(option){
            case 0:
                printf("Shutting Down!");
                break;
            case 1:
                printf("\tEnter the value to be inserted: ");
                scanf("%d", &value);
                insertAVL(AVL_Tree,value);
                break;
            case 2:
                printf("\tEnter the value to be removed:");
                scanf("%d", &value);
                removeAVL(AVL_Tree,value);
                break;
            case 3:
                printAVL(AVL_Tree);
                break;
            case 4:
                printf("\tEnter the value to be searched:");
                scanf("%d", &value);
                searchAVL(AVL_Tree,value);
                break;
            default:
                printf("\tInvalid Option!\n");
        }
    }while(option != 0);

    return 0;
}