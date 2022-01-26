const mqtt = require("mqtt");
const Entry = require("./models/Entry");
const User = require("./models/User");
const config = require("./config");

const mqttConf = config.mqtt;

async function start() {
	const connectUrl = `mqtt://${mqttConf.server.host}:${mqttConf.server.port}`;

	const client = mqtt.connect(connectUrl, {
		clientId: mqttConf.client.id,
	});

	client.on("connect", () => {
		console.log("connected to mqtt server");

		client.subscribe([mqttConf.subscribeTopic], () => {
			console.log(`Subscribed to topic '${mqttConf.subscribeTopic}'`);
		});
	});

	client.on("message", async (topic, payload) => {
		console.log(`recieved payload '${payload}' on topic '${topic}'`);

		try {
			const user = await User.findOne({ rfid: payload });

			if (!user)
				return console.log(`rfid '${payload}' not attatched to any user`);

			user.isInside = !user.isInside;
			console.log(await user.save());

			const entry = new Entry({
				username: user.username,
				isInside: user.isInside,
			});
			console.log(await entry.save());

			// publishing
			client.publish(
				user.isInside
					? mqttConf.publishOnEntryTopic
					: mqttConf.publishOnExitTopic,
				user.username
			);
		} catch (err) {
			console.error(err);
		}
	});
}

module.exports = start;
