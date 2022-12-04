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