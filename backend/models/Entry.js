const mongoose = require("mongoose");

module.exports = mongoose.model(
	"Entry",
	new mongoose.Schema(
		{
			username: {
				type: String,
				required: true,
			},
			isInside: {
				type: Boolean,
				required: true,
			},
		},
		{ timestamps: true }
	)
);
