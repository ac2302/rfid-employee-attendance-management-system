const router = require("express").Router();
const json2csv = require("json2csv");
const User = require("./models/User");
const Entry = require("./models/Entry");

router.get("/employees", async (req, res) => {
	try {
		return res.json(await User.find());
	} catch (err) {
		res.status(500).json(err);
	}
});

router.get("/present", async (req, res) => {
	try {
		return res.json(await User.find({ isInside: true }));
	} catch (err) {
		res.status(500).json(err);
	}
});

router.get("/logs", async (req, res) => {
	try {
		return res.json(await Entry.find());
	} catch (err) {
		res.status(500).json(err);
	}
});

router.get("/csv", async (req, res) => {
	try {
		const entries = (await Entry.find()).reverse();

		const fields = ["employee", "entry/exit", "time"];
		const data = entries.map((entry) => {
			return {
				employee: entry.username,
				"entry/exit": entry.isInside ? "entry" : "exit",
				time: new Date(Date.parse(entry.createdAt)).toString(),
			};
		});

		const csv = json2csv.parse(data, fields);

		res.attachment(`logs ${new Date().toString()}.csv`);
		res.type("txt");
		res.send(csv);
	} catch (err) {
		res.status(500).json(err);
	}
});

module.exports = router;
