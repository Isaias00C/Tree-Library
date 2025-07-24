 // Garante que o script só rode depois que a página carregar
 document.addEventListener('DOMContentLoaded', () => {

// 1. Pega os parâmetros da URL atual
const params = new URLSearchParams(window.location.search);

// 2. Extrai os valores de title e author
const title = params.get('title');
const author = params.get('author');

// 3. Encontra os elementos no HTML pelos IDs que você acabou de criar
const titleElement = document.getElementById('bookTitle');
const authorElement = document.getElementById('bookAuthor');

// 4. Preenche os elementos com os dados recebidos da URL
if (titleElement) {
    // Usa 'textContent' para inserir o texto de forma segura
    titleElement.textContent = title || 'Título não encontrado';
}
if (authorElement) {
    authorElement.textContent = author || 'Autor não encontrado';
}
});