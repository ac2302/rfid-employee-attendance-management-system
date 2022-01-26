const mongoose = require("mongoose");
const prompt = require("prompt");
const config = require("./config");

function main() {
	prompt.start();

	prompt.get(["username", "rfid"], async function (err, result) {
		if (err) {
			return console.error(err);
		}
		const user = new User(result);
		await user.save();
		console.log("user created");
		process.exit();
	});
}

// connecting to db
mongoose.connect(config.database.connectionString, (err) => {
	if (!err) {
		console.log("connected to db");
		main();
	} else console.error(err);
});

const User = mongoose.model(
	"User",
	new mongoose.Schema(
		{
			username: {
				type: String,
				required: true,
				unique: true,
			},
			isInside: {
				type: Boolean,
				default: false,
			},
			rfid: {
				type: String,
				required: true,
				unique: true,
			},
		},
		{ timestamps: true }
	)
);
