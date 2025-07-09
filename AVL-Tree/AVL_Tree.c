#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int value;
    struct node *left, *right;
    int height;
}Node;

Node* newNode(int x);
int bigger(int a, int b);
int nodeHeight(Node* node);
Node* rotationLeft(Node* node);
Node* rotationRight(Node* node);
Node* rotationRightLeft(Node* node);
Node* rotationLeftRight(Node* node);
Node* balance(Node* root);
Node* insertNode(Node* root, int x);
Node* removeNode(Node* root, int key);
void printTreeAVL(Node* root, int level);

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

int bigger(int a, int b){
    return (a > b) ? a: b;
}

//Retorna a altura do Nó ou -1 caso ele seja null
int nodeHeight(Node* node){
    if(node == NULL)
        return -1;
    else
        return node->height;
}

int balancingFactor(Node* node){
    if(node)
        return (nodeHeight(node->left) - nodeHeight(node->right));
    else
        return 0;
}

//Função que rotaciona à esquerda
Node* rotationLeft(Node* node){
    Node *y, *f;
    y = node->right;
    f = y->left;

    y->left = node;
    node->right = f;

    node->height = bigger(nodeHeight(node->left), nodeHeight(node->right)) + 1;
    y->height = bigger(nodeHeight(y->left), nodeHeight(y->right)) + 1;

    return y;
}

//Função que rotaciona à direita
Node* rotationRight(Node* node){
    Node *y, *f;

    y = node->left;
    f = y->right;

    y->right = node;
    node->left = f;

    node->height = bigger(nodeHeight(node->left), nodeHeight(node->right)) + 1;
    y->height = bigger(nodeHeight(y->left), nodeHeight(y->right)) + 1;

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
    root->height = bigger(nodeHeight(root->left), nodeHeight(root->right)) + 1;

    //Ele verifica se precisa balancear ou não
    root = balance(root);
    
    return root;
}

//Forma de remoção bem mais complexa, no caso ele busca em específico o valor e logo após faz o balanceamento
Node* removeNode(Node* root, int key){
    if(root == NULL){
        printf("Valor não encontrado!\n");
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

        root->height = bigger(nodeHeight(root->left), nodeHeight(root->right)) + 1;

        root = balance(root);

        return root;
    }
}

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

int main(){
    int option, value;
    Node* root = NULL;
    option = -1;
    do{
        printf("\n\n\t0 - Sair\n\t1 - Inserir\n\t2 - Remover\n\t3 - Imprimir\n\n");
        scanf("%d", &option);

        switch(option){
            case 0:
                printf("Saindo!!!");
                break;
            case 1:
                printf("\tDigite o valor a ser inserido: ");
                scanf("%d", &value);
                root = insertNode(root, value);
                break;
            case 2:
                printf("\tDigite o valor a ser removido: ");
                scanf("%d", &value);
                root = removeNode(root, value);
                break;
            case 3:
                printTreeAVL(root, 1);
                break;
            default:
                printf("\tOpção invalida!!!!!\n");
        }
    }while(option != 0);

    return 0;
}