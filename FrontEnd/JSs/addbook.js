document.querySelector(".BookInfos").addEventListener("submit", function (e) {
  e.preventDefault()

  const title = document.getElementById("titutoBook").value
  const author = document.getElementById("autorBook").value

  const newBook = {
    title: title,
    author: author,
    isbn: Date.now().toString(), // gera um "id"
    year: 2024,
    stock: 1,
  }

  // Pega a lista de livros já salvos
  const books = JSON.parse(sessionStorage.getItem("livrosSalvos")) || []

  // Adiciona o novo livro à lista
  books.push(newBook)

  // Salva de volta no sessionStorage
  sessionStorage.setItem("livrosSalvos", JSON.stringify(books))

  // Redireciona para a página de resultados
  window.location.href = "resultados.html"
})
