import "./App.css";
import Employees from "./components/Employees";
import config from "./config";

function App() {
	return (
		<div>
			<button
				onClick={() => {
					window.open(`${config.backendLocation}/csv`, "_blank");
				}}
			>
				Download Logs
			</button>
			<h1>Employee Attendance Management System</h1>
			<Employees />
		</div>
	);
}

export default App;
