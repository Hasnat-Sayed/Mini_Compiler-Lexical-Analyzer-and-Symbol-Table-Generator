const express = require("express");
const { spawn } = require("child_process");
const app = express();
const PORT = 3000;

app.use(express.static("public"));
app.use(express.json());

app.post("/analyze", (req, res) => {
  const code = req.body.code;
  console.log("Received Code:", code);

  const analyzer = spawn("./analyzer.exe");

  let output = "";
  analyzer.stdout.on("data", data => {
    output += data.toString();
  });

  analyzer.stderr.on("data", data => {
    console.error("Analyzer Error:", data.toString());
  });

  analyzer.on("close", () => {
    try {
      console.log("Analyzer Output:", output);
      const result = JSON.parse(output);
      res.json(result);
    } catch (err) {
      console.error("JSON Parse Error:", err.message);
      res.status(500).send("Failed to parse output");
    }
  });

  analyzer.stdin.write(code);
  analyzer.stdin.end();
});

app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});
