const express = require("express");
const app = express();
const hostname = "127.0.0.1";
const port = 3061;
const bodyParser = require("body-parser");
const urlencodedParser = bodyParser.urlencoded({ extended: false });
app.use(express.static("./JoguinhoHTML/"));
app.use(express.json());

var externalPosition = 0;
var externalGameStatus = 1;

// post data to server
app.post("/sendposition", function (req, res) {
  res.header("Access-Control-Allow-Origin", "*");
  console.log(req);
  console.log(req.body);
  externalPosition = req.body.position;
  texto = req.body;
  console.log(texto);
  console.log("Enviei um dado");
  res.send(texto);
});

// get data from server
app.get("/sendposition", function (req, res) {
  res.header("Access-Control-Allow-Origin", "*");
  dados = {
    position: externalPosition
  },
  jsonD = JSON.stringify(dados);
  res.send(jsonD);
});

// get game status from server
app.get("/gamestatus", function (req, res) {
  res.header("Access-Control-Allow-Origin", "*"),
  dados = {
    status: externalGameStatus
  },
  jsonD = JSON.stringify(dados);
  res.send(jsonD);
});

// post game status to server
app.post("/gamestatus", function (req, res) {
  res.header("Access-Control-Allow-Origin", "*");
  console.log(req);
  console.log(req.body);
  externalGameStatus = req.body.status;
  texto = req.body;
  console.log(texto);
  console.log("Enviei um dado");
  res.send(texto);
});

app.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});