#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>

// Funcao para iniciar nossas variaveis no javacscript
void initLibrary();

typedef struct Book {
    char title[256];
    char author[128];
    char isbn[20]; // Usar como chave para a árvore (deve ser único)
    int year;
    int stock;
} Book;

void displayBook(Book book);

typedef struct Node{
    Book book;
    struct Node *left, *right;
    int height;
}Node;

Node* newNode(Book book);
int bigger(int a,int b);
int GetHeight(Node* node);
int balancingFactor(Node* node); 
Node* rotationLeft(Node* node);
Node* rotationRight(Node* node);
Node* rotationRightLeft(Node* node);
Node* rotationLeftRight(Node* node);
Node* balance(Node* root);
Node* insertNode(Node* root, Book book);
Node* removeNode(Node* root, const char* isbn);
Node* searchNode(Node* root, const char* isbn);
void printTreeAVL(Node* root, int level); 

typedef struct {
    Book *books; // Um array dinâmico de Books encontrados
    int count;      // Quantidade de Books encontrados
    int capacity;   // Capacidade atual do array
} SearchResults;

void startSearchResults(SearchResults *list);
void addBookToResultsList(SearchResults *list, Book book); 
void freeSearchResults(SearchResults *list); 
void searchBooksByTitleSubstring(Node *node, const char *searchTerm, SearchResults *results); 

typedef struct Tree {
    int numberNodes;// Numero de nos da arvove
    Node     *root;// No raiz da arvore
    int      depth;// profundidade da arvoce
} Tree;

Tree* createAVLTree();
void insertAVL(Tree* tree, Book book);
void removeAVL(Tree* tree, const char* isbn);
void searchAVL(Tree* tree, const char* isbn);
void printAVL(Tree* tree);
void updateTreeMetrics(Tree* tree);

// Funcoes wrapers pra nao mandar estruturas complexas pro javascript
int addBook(const char* title, const char* author, const char* isbn, int year, int stock);
int removeBookByISBN(const char* isbn);
const char* searchAndGetJSON(const char* searchTerm);

Tree* global_libraryTree = NULL;
SearchResults global_searchResults;

/*--------------daqui pra baixo eh codigo, daqui pra cima eh declaracao constante--------------------------*/
EMSCRIPTEN_KEEPALIVE
void initLibrary(){
    if(global_libraryTree == NULL){
        global_libraryTree = createAVLTree();
        startSearchResults(&global_searchResults);
        printf("biblioteca inicializada!\n");
    }
}

/**
 * @brief Cria um Node.
 * @param x O data a ser inserido.
 */
Node* newNode(Book book){
    Node* new = malloc(sizeof(Node));

    if(new){
        new->book = book;
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
 * @param root O ponteiro para o Node root
 * @param Book O struct Livro a ser inserido.
 */
Node* insertNode(Node* root, Book book){
    if(root == NULL)
        return newNode(book);

    // Usamos strcmp para comparar ISBNs
    int cmp = strcmp(book.isbn, root->book.isbn);

    if(cmp < 0) // Book.isbn é "menor" que root->Book.isbn
        root->left = insertNode(root->left, book);
    else if(cmp > 0) // Book.isbn é "maior" que root->Book.isbn
        root->right = insertNode(root->right, book);
    else {
        printf("\nInsertion not performed, Book with ISBN '%s' already exists!\n", book.isbn);
        // Podemos atualizar o estoque aqui ou apenas impedir a inserção
        // root->Book.quantidadeDisponivel += Book.quantidadeDisponivel; // Exemplo de atualização de estoque
    }


    //Ele recalcula a altura do nó raiz
    root->height = bigger(GetHeight(root->left), GetHeight(root->right)) + 1;

    //Ele verifica se precisa balancear ou não
    root = balance(root);

    return root;
}

//Forma de remoção bem mais complexa, no caso ele busca em específico o valor e logo após faz o balanceamento
/**
 * @brief Remove um nó da árvore AVL.
 * @param root O ponteiro para a raiz da subárvore.
 * @param isbn O ISBN (chave) do livro a ser removido.
 */
Node* removeNode(Node* root, const char* isbn){
    if(root == NULL){
        printf("Book with ISBN '%s' not found!\n", isbn);
        return NULL;
    }
    //Procurando o Nó para remover
    else{
        int cmp = strcmp(isbn, root->book.isbn);

        if(cmp == 0){ // Book encontrado para remoção
            //Se o No não tiver filhos
            if(root->left == NULL && root->right == NULL){
                printf("Book '%s' (ISBN: %s) removed!\n", root->book.title, root->book.isbn);
                free(root);
                return NULL;
            }
            //Se o No tiver os dois filhos
            else{
                if(root->left != NULL && root->right != NULL){
                    Node* aux = root->left;
                    while(aux->right != NULL)
                        aux = aux->right;
                    // Copia os dados do sucessor (nó mais à direita da subárvore esquerda)
                    root->book = aux->book;
                    // Remove o nó que teve seus dados copiados
                    root->left = removeNode(root->left, aux->book.isbn);
                    return root;
                }
                else{ // Nó com um filho
                    Node* aux;
                    if(root->left != NULL)
                        aux = root->left;
                    else
                        aux = root->right;
                    printf("Book '%s' (ISBN: %s) removed!\n", root->book.title, root->book.isbn);
                    free(root);
                    return aux;
                }
            }
        }
        else{ // Continua buscando o nó para remover
            if(cmp < 0)
                root->left = removeNode(root->left, isbn);
            else
                root->right = removeNode(root->right, isbn);
        }

        // Recalcula altura e balanceia após a remoção
        root->height = bigger(GetHeight(root->left), GetHeight(root->right)) + 1;
        root = balance(root);
        return root;
    }
}

/**
 * @brief Procura um livro específico na subárvore AVL pelo ISBN.
 * @param root O ponteiro para a raiz da subárvore.
 * @param isbn O ISBN a ser procurado.
 * @return Um ponteiro para o Node que contém o livro, ou NULL se o livro não for encontrado.
 */
Node* searchNode(Node* root, const char* isbn){
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(isbn, root->book.isbn);

    if (cmp == 0) { // ISBN encontrado
        return root;
    } else if (cmp < 0) { // Busca na subárvore esquerda
        return searchNode(root->left, isbn);
    } else { // Busca na subárvore direita
        return searchNode(root->right, isbn);
    }
}


/**
 * @brief Imprime a arvore Rotacionada 90 graus
 * @param root O ponteiro para o Node root
 * @param level Level inicial para printar
 */
void printTreeAVL(Node* root, int level){
    int i;
    if(root){
        printTreeAVL(root->right, level + 1);
        printf("\n\n");

        for(i = 0; i < level; i++)
            printf("\t");

        printf("%s", root->book.isbn); // Imprime o ISBN e altura
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
 * @brief Insere um livro na Arvore.
 * @param tree O ponteiro para a estrutura Tree.
 * @param Book O struct Livro a ser inserido.
 */
void insertAVL(Tree* tree, Book book) {
    if (tree == NULL) {
        printf("Error:Tree not started yet.\n");
        return;
    }
    // Verifica se o ISBN já existe antes de tentar inserir para não contar duplicado
    if (searchNode(tree->root, book.isbn) != NULL) {
        printf("Book with ISBN '%s' already exists!\n", book.isbn);
        return;
    }

    tree->root = insertNode(tree->root, book);
    tree->numberNodes++; // Incrementa o número de nós apenas se a inserção foi bem-sucedida
    updateTreeMetrics(tree);
}

/**
 * @brief Remove um livro da árvore AVL.
 * @param tree O ponteiro para a estrutura Tree.
 * @param isbn O ISBN do livro a ser removido.
 */
void removeAVL(Tree* tree, const char* isbn) {
    if (tree == NULL) {
        printf("Erro: Árvore não inicializada.\n");
        return;
    }
    // Verifica se o livro existe antes de tentar remover
    if (searchNode(tree->root, isbn) == NULL) {
        printf("Livro com ISBN '%s' não encontrado para remoção.\n", isbn);
        return;
    }

    tree->root = removeNode(tree->root, isbn);
    tree->numberNodes--; // Decrementa o número de nós
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
 * @brief Procura um livro na árvore AVL pelo ISBN e exibe uma mensagem.
 * @param tree O ponteiro para a estrutura Tree.
 * @param isbn O ISBN a ser procurado.
 */
void searchAVL(Tree* tree, const char* isbn) {
    if (tree == NULL) {
        printf("\tErro: Árvore não inicializada.\n");
        return;
    }
    if (tree->root == NULL) {
        printf("\tÁrvore vazia. O livro com ISBN '%s' não pode ser encontrado.\n", isbn);
        return;
    }

    Node* foundNode = searchNode(tree->root, isbn);
    if (foundNode != NULL) {
        printf("\n\tLivro encontrado pelo ISBN '%s':\n", isbn);
        displayBook(foundNode->book);
    } else {
        printf("\tLivro com ISBN '%s' não encontrado na árvore.\n", isbn);
    }
}

void displayBook(Book Book) {
    printf("Title: %s\n", Book.title);
    printf("Author: %s\n", Book.author);
    printf("ISBN: %s\n", Book.isbn);
    printf("Year: %d\n", Book.year);
    printf("Stock: %d\n", Book.stock);
    printf("----------------------------------------\n");
}

void startSearchResults(SearchResults *lista) {
    lista->count = 0;
    lista->capacity = 5; // Comece com uma capacidade pequena
    lista->books = (Book *)malloc(lista->capacity * sizeof(Book));
    if (lista->books == NULL) {
        printf("Error after try allocating memory!");
    }
}

void addBookToResultsList(SearchResults *list, Book book) {
    if (list->count == list->capacity) {
        list->capacity *= 2; // Dobra a capacidade
        Book *newList = (Book *)realloc(list->books, list->capacity * sizeof(Book));
        if (newList == NULL) {
            perror("Error reallocating memory for search results list");
            exit(EXIT_FAILURE);
        }
        list->books = newList;
    }
    list->books[list->count++] = book; // Adiciona o livro (copia a struct)
}

// Função para liberar a memória alocada pela lista de resultados
void freeSearchResults(SearchResults *lista) {
    free(lista->books);
    lista->books = NULL;
    lista->count = 0;
    lista->capacity = 0;
}

/**
 * @brief Busca livros na árvore por uma sequência de caracteres no título
 * e armazena os resultados em uma SearchResultsList.
 * @param node O nó atual no percurso da árvore.
 * @param searchTerm A sequência de caracteres a ser procurada no título.
 * @param results Ponteiro para a estrutura SearchResultsList onde os livros encontrados serão armazenados.
 */
void searchBooksByTitleSubstring(Node *node, const char *searchTerm, SearchResults *results) {
    if (node == NULL) {
        return;
    }

    // Percorre a subárvore esquerda
    searchBooksByTitleSubstring(node->left, searchTerm, results);

    // Verifica o nó atual
    // strstr() retorna um ponteiro para a primeira ocorrência da substring,
    // ou NULL se a substring não for encontrada.
    // Para busca case-insensitive, você pode converter ambos para minúsculas antes de usar strstr.
    if (strstr(node->book.title, searchTerm) != NULL) {
        addBookToResultsList(results, node->book);
    }

    // Percorre a subárvore direita
    searchBooksByTitleSubstring(node->right, searchTerm, results);
}

EMSCRIPTEN_KEEPALIVE
int addBook(const char* title, const char* author, const char* isbn, int year, int stock){
    if(global_libraryTree == NULL) return -1; // ERROR: nao inicializado

    if(searchNode(global_libraryTree->root, isbn) != NULL){
        return 0; // isbn ja existe
    }

    Book newBook;

    strncpy(newBook.title, title, sizeof(newBook.title) - 1);
    newBook.title[sizeof(newBook.title) - 1] = '\0';

    strncpy(newBook.author, author, sizeof(newBook.author) - 1);
    newBook.author[sizeof(newBook.author) - 1] = '\0';

    strncpy(newBook.isbn, isbn, sizeof(newBook.isbn) - 1);
    newBook.isbn[sizeof(newBook.isbn) - 1] = '\0';
    
    newBook.year = year;
    newBook.stock = stock;

    insertAVL(global_libraryTree, newBook);
    return 1; // Sucesso
}

EMSCRIPTEN_KEEPALIVE
int removeBookByISBN(const char* isbn) {
    if (global_libraryTree == NULL || searchNode(global_libraryTree->root, isbn) == NULL) {
        return 0; // Livro não encontrado
    }
    removeAVL(global_libraryTree, isbn);
    return 1; // Sucesso
}

EMSCRIPTEN_KEEPALIVE
const char* searchAndGetJSON(const char* searchTerm) {
    if (global_libraryTree == NULL || global_libraryTree->root == NULL) {
        return "[]"; // Retorna um array JSON vazio se a árvore estiver vazia
    }

    // Limpa os resultados da busca anterior
    freeSearchResults(&global_searchResults);
    startSearchResults(&global_searchResults);

    searchBooksByTitleSubstring(global_libraryTree->root, searchTerm, &global_searchResults);

    if (global_searchResults.count == 0) {
        return "[]";
    }

    // Aloca um buffer grande o suficiente para a string JSON
    // CUIDADO: Esta é uma abordagem simplificada. Para produção, uma biblioteca JSON ou um
    // construtor de string dinâmico seria mais seguro para evitar estouro de buffer.
    static char jsonBuffer[4096]; // Buffer estático para simplicidade
    char tempBuffer[512];
    
    strcpy(jsonBuffer, "[");

    for (int i = 0; i < global_searchResults.count; i++) {
        // Formata cada livro como um objeto JSON
        // Nota: Isso não trata caracteres especiais como aspas dentro dos campos.
        sprintf(tempBuffer, "{\"title\":\"%s\",\"author\":\"%s\",\"isbn\":\"%s\",\"year\":%d,\"stock\":%d}",
                global_searchResults.books[i].title,
                global_searchResults.books[i].author,
                global_searchResults.books[i].isbn,
                global_searchResults.books[i].year,
                global_searchResults.books[i].stock);
        
        strcat(jsonBuffer, tempBuffer);
        if (i < global_searchResults.count - 1) {
            strcat(jsonBuffer, ",");
        }
    }
    strcat(jsonBuffer, "]");

    return jsonBuffer;
}
// Codigo comentado pq nao precisa da main no frontend

/*
    Funcoes para colocar o 

    Adicionar livro
    Remover Livro
    Procurar livro por ID
    procurar livro por titulo

    nao ha necessidade de mostrar a arvora, nao vamo usar essa funcao, pro front so precisa dessa 4 funcoes ai
*/

/*
int main(){
    int option;
    char input_isbn[20];
    char input_title_search[256]; // Variável para a string de busca por título
    Book newBook; // Variável para armazenar os dados do novo livro a ser inserido
    
    Tree* AVL_Tree = createAVLTree();
    
    do{
        printf("\n\n--- AVL BOOK LIBRARY MENU ---\n"); // Menu em inglês
        printf("0 - Exit\n");
        printf("1 - Insert Book\n");
        printf("2 - Remove Book (by ISBN)\n");
        printf("3 - Print Tree (debug - by ISBN)\n");
        printf("4 - Search Book (by ISBN)\n");
        printf("5 - Search Books (by Title - substring)\n"); // Nova opção para busca por título
        printf("-------------------------------\n");
        printf("Choose an option: ");
        scanf("%d", &option);
        // Limpar o buffer de entrada após scanf para evitar problemas com fgets
        while (getchar() != '\n'); // Consome o '\n' restante no buffer
        
        switch(option){
            case 0:
            printf("Shutting Down the Library!\n"); 
            // TODO: Implementar função para liberar toda a memória alocada para os nós da árvore
            // Exemplo: void freeTreeNodes(Node* node); e chamar freeTreeNodes(AVL_Tree->root);
            free(AVL_Tree); // Libera a estrutura da Tree principal
            break;
            case 1:
            printf("\n--- Insert New Book ---\n"); 
            printf("Title: ");
            fgets(newBook.title, sizeof(newBook.title), stdin);
            newBook.title[strcspn(newBook.title, "\n")] = 0; // Remove o '\n' do final da string
            
            printf("Author: ");
            fgets(newBook.author, sizeof(newBook.author), stdin);
            newBook.author[strcspn(newBook.author, "\n")] = 0; // Remove o '\n'
            
            printf("ISBN: ");
            fgets(newBook.isbn, sizeof(newBook.isbn), stdin);
            newBook.isbn[strcspn(newBook.isbn, "\n")] = 0; // Remove o '\n'
            
            printf("Year of Publication: ");  
            scanf("%d", &newBook.year); // Lê o ano
            while (getchar() != '\n'); // Limpa o buffer
            
            printf("Quantity in Stock: ");  
            scanf("%d", &newBook.stock); // Lê o estoque
            while (getchar() != '\n'); // Limpa o buffer
            
            insertAVL(AVL_Tree, newBook); // Insere o novo livro na árvore
            break;
            case 2:
            printf("\n--- Remove Book ---\n"); 
            printf("Enter the ISBN of the book to be removed: ");
            fgets(input_isbn, sizeof(input_isbn), stdin);
            input_isbn[strcspn(input_isbn, "\n")] = 0; // Remove o '\n'
            removeAVL(AVL_Tree, input_isbn); // Remove o livro pelo ISBN
            break;
            case 3:
            printf("\n--- AVL Tree (by ISBN) ---\n"); 
            printAVL(AVL_Tree); // Imprime a árvore (mostra ISBNs e alturas)
            printf("\nNumber of nodes: %d, Depth: %d\n", AVL_Tree->numberNodes, AVL_Tree->depth); 
            break;
            case 4:
            printf("\n--- Search Book by ISBN ---\n"); 
            printf("Enter the ISBN of the book to be searched: ");
            fgets(input_isbn, sizeof(input_isbn), stdin);
            input_isbn[strcspn(input_isbn, "\n")] = 0; // Remove o '\n'
            searchAVL(AVL_Tree, input_isbn); // Busca o livro pelo ISBN e o exibe
            break;
            case 5: // Busca por Título (Substring)
            printf("\n--- Search Books by Title (Substring) ---\n"); 
            printf("Enter the title sequence to search: ");
            fgets(input_title_search, sizeof(input_title_search), stdin);
            input_title_search[strcspn(input_title_search, "\n")] = 0; // Remove o '\n'
            
            SearchResults foundBooks; // Cria uma estrutura para armazenar os resultados
            startSearchResults(&foundBooks); // Inicializa a lista de resultados
            
            // Chama a função de busca que percorre a árvore para encontrar livros por título
            searchBooksByTitleSubstring(AVL_Tree->root, input_title_search, &foundBooks);
            
            if (foundBooks.count > 0) {
                printf("\n%d Book(s) found with '%s' in the title:\n", foundBooks.count, input_title_search); 
                for (int i = 0; i < foundBooks.count; i++) {
                    printf("\n--- Book %d ---\n", i + 1); 
                    displayBook(foundBooks.books[i]); // Exibe cada livro encontrado
                }
            } else {
                printf("\nNo books found with '%s' in the title.\n", input_title_search); 
            }
            
            freeSearchResults(&foundBooks); // Libera a memória alocada para os resultados da busca
            break;
            default:
            printf("\tInvalid Option! Please try again.\n"); 
        }
    } while(option != 0);
    
    return 0;
}
*/