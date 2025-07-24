import EmscriptenModule from "../../AVL-Tree/AVL_Tree.mjs"

EmscriptenModule().then((Module) => {
  console.log("Módulo WASM carregado no Resultados")

  // 1. Inicializa a AVL
  Module._initLibrary()

  // 2. Pega os livros do sessionStorage
  const livros = JSON.parse(sessionStorage.getItem("livrosSalvos")) || []

  // 3. Prepara o wrapper do addBook do WASM
  const addBook = Module.cwrap("addBook", "string", [
    "string",
    "string",
    "string",
    "number",
    "number",
  ])

  // 4. Insere os livros na AVL
  livros.forEach((livro) => {
    addBook(livro.title, livro.author, livro.isbn, livro.year, livro.stock)
  })

  // 5. Wrapper para buscar livros no WASM
  const searchAndGetJSON = Module.cwrap("searchAndGetJSON", "string", [
    "string",
  ])

  // 6. Ligar elementos do HTML
  const titleSearch = document.getElementById("titleSearch")
  const searchButton = document.getElementById("searchButton")
  const bookGrid = document.querySelector(".book-grid")

  function renderBooks(query) {
    const resultJSON = searchAndGetJSON(query)
    const resultados = JSON.parse(resultJSON)

    bookGrid.innerHTML = ""

    resultados.forEach((livro) => {
      const card = document.createElement("a")
      card.className = "book-card"
      card.href = `livro.html?isbn=${encodeURIComponent(livro.isbn)}&title=${encodeURIComponent(livro.title)}&author=${encodeURIComponent(livro.author)}`
      card.innerHTML = `
        <div class="book-cover-placeholder">
          <img class="imageBook" src="../Assets/Imagem1.jpg" />
        </div>
        <div class="book-info">
          <p class="book-title">${livro.title}</p>
          <p class="book-author">${livro.author}</p>
        </div>
      `
      bookGrid.appendChild(card)
    })

    if (resultados.length === 0) {
      bookGrid.innerHTML =
        "<p style='padding: 1rem;'>Nenhum livro encontrado.</p>"
    }
  }

  // 7. Executar a busca somente quando clicar no botão
  searchButton.addEventListener("click", () => {
    const termo = titleSearch.value.trim()
    renderBooks(termo)
  })

  // 8. Mostra todos os livros ao carregar a página
  renderBooks("")
})
