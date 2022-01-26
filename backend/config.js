const config = {
	server: {
		port: 5500,
	},
	database: {
		connectionString: "mongodb://127.0.0.1:27017/attendance",
	},
	mqtt: {
		server: {
			host: "aryansdomain.com",
			port: 1883,
		},
		client: {
			id: "server",
		},
		subscribeTopic: "attendance/swipe",
		publishOnEntryTopic: "attendance/entry",
		publishOnExitTopic: "attendance/exit",
	},
};

module.exports = config;
