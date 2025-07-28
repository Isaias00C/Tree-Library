# 🌳 Tree-Library: Sistema de Biblioteca com Árvore AVL

Este é o projeto final da cadeira de Estruturas de Dados, que implementa um sistema de gerenciamento para uma biblioteca utilizando uma **Árvore de Busca Binária Balanceada (Árvore AVL)** como estrutura de dados principal para armazenar e organizar os livros.

## 📖 Sobre o Projeto

O **Tree-Library** foi criado para aplicar de forma prática os conceitos de estruturas de dados avançadas. O uso de uma Árvore AVL garante que as operações essenciais do sistema — como busca, inserção e remoção de livros — sejam executadas com alta performance, mantendo a complexidade em tempo logarítmico ($O(\log n)$), mesmo para um grande volume de dados.

O sistema simula o gerenciamento do acervo de uma biblioteca, onde cada livro é um nó na árvore, organizado por um código de identificação único.

## ✨ Funcionalidades

O programa é controlado via um menu interativo no console e oferece as seguintes funcionalidades:

- **Adicionar Livro:** Insere um novo livro no acervo. O sistema solicita o código, nome e autor do livro. A árvore se autobalanceia após a inserção para manter sua eficiência.
- **Remover Livro:** Exclui um livro do acervo a partir de seu código. A árvore também se reorganiza e se rebalanceia para preencher a lacuna.
- **Buscar Livro:** Procura por um livro específico utilizando seu código e exibe suas informações.
- **Imprimir Acervo (em ordem):** Lista todos os livros cadastrados em ordem crescente de código, utilizando a travessia _in-order_ da árvore.
- **Imprimir Árvore (estrutura):** Exibe a estrutura hierárquica da Árvore AVL, mostrando os nós pais e seus filhos, permitindo uma visualização clara da organização dos dados.

## 🛠️ Tecnologias e Conceitos Aplicados

- **Linguagem:** C++
- **Estrutura de Dados:** Árvore de Busca Binária Balanceada (AVL).
- **Principais Conceitos:**
  - Ponteiros e Alocação Dinâmica de Memória.
  - Recursividade para as operações de inserção, remoção e busca.
  - **Balanceamento de AVL:** Cálculo do fator de balanceamento e aplicação de rotações (Simples à Esquerda/Direita e Duplas) para manter a altura da árvore balanceada.
  - Travessias de Árvore: _In-Order_ (para listagem ordenada) e _Pre-Order_ (para exibição da estrutura).
  - Modularização com arquivos de cabeçalho (`.h`) e implementação (`.cpp`).

## 📂 Estrutura dos Arquivos

O código-fonte está organizado da seguinte forma:
