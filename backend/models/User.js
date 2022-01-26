const mongoose = require("mongoose");

module.exports = mongoose.model(
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
