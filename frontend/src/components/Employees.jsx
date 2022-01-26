import axios from "axios";
import React, { useState, useEffect } from "react";
import config from "../config";

function Employees() {
	const [employees, setEmployees] = useState([]);

	useEffect(() => {
		const query = () => {
			axios
				.get(`${config.backendLocation}/employees`)
				.then((res) => {
					console.log(res.data);
					setEmployees(res.data);
				})
				.catch((err) => {
					console.error(err);
					alert("server offline");
				});
		};
		query();
		setInterval(query, config.refreshTime);
	}, []);

	return (
		<div>
			<table>
				<thead>
					<tr>
						<th>Employee</th>
						<th>Is Present</th>
						<th>Updated At</th>
					</tr>
				</thead>
				<tbody>
					{employees.map((emp, index) => (
						<tr>
							<td>{emp.username}</td>
							<td className={emp.isInside ? "present" : "absent"}>
								{emp.isInside ? "yes" : "no"}
							</td>
							<td>{new Date(Date.parse(emp.updatedAt)).toString()}</td>
						</tr>
					))}
				</tbody>
			</table>
		</div>
	);
}

export default Employees;
