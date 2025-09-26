let books = [];

async function loadBooks() {
    try {
        const response = await fetch('books.json');
        books = await response.json();
        renderTable();
    } catch(err) {
        console.error("Could not load books.json.", err);
        alert("Error loading library data.");
    }
}

function renderTable() {
    const searchValue = document.getElementById("searchInput").value.toLowerCase();
    const table = document.getElementById("booksTable");

    table.innerHTML = `
    <tr>
      <th>ID</th>
      <th>Title</th>
      <th>Author</th>
      <th>Status</th>
    </tr>`;

    books.forEach(book => {
        if(book.title.toLowerCase().includes(searchValue) || book.author.toLowerCase().includes(searchValue)) {
            const row = table.insertRow();
            row.insertCell(0).innerText = book.id;
            row.insertCell(1).innerText = book.title;
            row.insertCell(2).innerText = book.author;
            row.insertCell(3).innerText = book.status;
            row.cells[3].className = (book.status === "Available") ? "status-available" : "status-issued";
        }
    });
}

// Auto-refresh every 5 seconds
setInterval(loadBooks, 5000);
loadBooks();