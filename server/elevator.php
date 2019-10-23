<?php
$servername = "";
$username = "";
$password = "";
$dbname = "";

print_r($_GET['pressure']. PHP_EOL);
print_r($_GET['temp']. PHP_EOL);

if(isset($_GET['pressure']) && isset($_GET['temp'])){
	try {
		$conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
		// set the PDO error mode to exception
		$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

		// prepare sql and bind parameters
		$stmt = $conn->prepare("INSERT INTO elevator (pressure, temp)
		VALUES (:pressure, :temp)");
		$stmt->bindParam(':pressure', $pressure);
		$stmt->bindParam(':temp', $temp);

		// insert a row
		$pressure = $_GET['pressure'];
		$temp = $_GET['temp'];
		$stmt->execute();

		echo "New records created successfully";
	} catch(PDOException $e) {
		echo "Error: " . $e->getMessage();
	}
	$conn = null;	
} else {
	print("Please provide pressure AND temperature");
}