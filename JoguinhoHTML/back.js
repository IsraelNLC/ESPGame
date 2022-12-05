const express = require('express');
const app = express();
const hostname = '127.0.0.1';
const port = 3061;
const bodyParser = require('body-parser');
const urlencodedParser = bodyParser.urlencoded({ extended: false })
app.use(express.static("./JoguinhoHTML/"));
app.use(express.json());

app.listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});

var externalData = 0;
var externalGameStatus = 0;

// post data to server
app.post("/senddata", function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    console.log(req);
    console.log(req.body);
    externalData = req.body.data;
    texto = req.body;
    console.log(texto);
    console.log("Enviei um dado");
    res.send(texto);
  });

// get data from server
app.get("/senddata", function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    dados = {
      data: externalData
    };
    jsonD = JSON.stringify(dados);
    res.send(jsonD);
  });


// get game status from server
app.get("/sendgamestatus", function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    dados = {
      gameStatus: externalGameStatus
    };
    jsonD = JSON.stringify(dados);
    res.send(jsonD);
  });

// post game status to server
app.post("/sendgamestatus", function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    console.log(req);
    console.log(req.body);
    gameStatus = req.body.gameStatus;
    texto = req.body;
    console.log(texto);
    console.log("Enviei um dado");
    res.send(texto);
  }
);
