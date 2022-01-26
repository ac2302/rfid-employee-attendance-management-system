const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");
const mqttClient = require("./mqttClient");
const config = require("./config");

const app = express();
app.use(cors());

// routes
app.use(require("./routes"));

// connecting to db
mongoose.connect(config.database.connectionString, (err) => {
	if (!err) console.log("connected to db");
	else console.error(err);
});

mqttClient();

const port = config.server.port;
app.listen(port, () => {
	console.log(`server live on port ${port}`);
});
