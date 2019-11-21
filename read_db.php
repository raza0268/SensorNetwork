<!DOCTYPE html>
<html>
	<head>
		<style>
			table {
				border-collapse: collapse;
				width: 100%;
				color: #1f5380;
				font-family: monospace;
				font-size: 20px;
				text-align: left;
			} 
			th {
				background-color: #1f5380;
				color: white;
			}
			tr:nth-child(even) {background-color: #f2f2f2}
		</style>
	</head>
	<?php
		//Creates new record as per request
		//Connect to database
        $hostname = "localhost";		//example = localhost or 192.168.0.0
		$username = "root";		//example = root
		$password = "";
		$dbname = "nodemcu_idr";
		// Create connection
		$conn = mysqli_connect($hostname, $username, $password, $dbname);
		// Check connection
		if (!$conn) {
			die("Connection failed !!!");
		} 
	?>
	<body>
		<table>
			<tr>
				<th align="center">No</th> 
				<th align="center">Temperature Value</th> 
				<th align="center">Date</th>
				<th align="center">Time</th>
			</tr>	
			<?php
				$table = mysqli_query($conn, "SELECT No, Ldr, Date, Time FROM nodemcu_idr_table ORDER BY No DESC"); 
				//Latest record will be show first
				//die($table);
				$i=1;
				//nodemcu_ldr_table = Youre_table_name
				while($row = mysqli_fetch_array($table))
				{
			?>
			<tr>
				<!-- <td><?php echo $row["No"]; ?></td> -->
				<td align="center"><?php echo $i; ?></td>
				<td align="center"><?php echo $row["Ldr"]; ?></td>
				<td align="center"><?php echo $row["Date"]; ?></td>
				<td align="center"><?php echo $row["Time"]; ?></td>
			</tr>

			<?php
			$i++;
				}
			?>
		</table>
	</body>
</html>
