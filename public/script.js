
async function analyzeCode() {
    
  const code = document.getElementById("codeInput").value;
  const tokenOutput = document.getElementById("tokenOutput");
  const symbolTable = document.getElementById("symbolTable");
  tokenOutput.innerHTML = "";
  symbolTable.innerHTML = "";

  try {
    const res = await fetch("/analyze", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ code })
    });

    const { tokens, symbolTable: table } = await res.json();

    const printed = new Set();
    tokens.forEach(({ value, type }) => {
      if (printed.has(value)) return;
      printed.add(value);

      const div = document.createElement("div");
      div.className = `token ${type}`;
      div.innerHTML = `${value}<div class='label'>${type}</div>`;
      tokenOutput.appendChild(div);
    });

    table.forEach(({ name, type, address }) => {
      const row = `<tr><td>${name}</td><td>${type}</td><td>${address}</td></tr>`;
      symbolTable.innerHTML += row;
    });
  } catch (err) {
    tokenOutput.innerHTML = `<div style="color:red;">Error: ${err.message}</div>`;
  }
}
