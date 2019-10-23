<?php
header("Access-Control-Allow-Origin: *");

$servername = "";
$username = "";
$password = "";
$dbname = "";

try {
    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $stmt = $conn->prepare("SELECT id, pressure, temp, timestamp FROM elevator ORDER BY id DESC");
    $stmt->execute();

    // set the resulting array to associative
    $stmt->setFetchMode(PDO::FETCH_ASSOC);
    $result = $stmt->fetchAll();
    $json = array();
    foreach ($result as $row => $messure) {
        $object = new stdClass();
        $object->date = date('Y-m-d H:i:s', strtotime($messure['timestamp']));
        $object->value = $messure['pressure'];
        array_push($json, $object);
    }
    echo json_encode($json);
}
catch(PDOException $e) {
    echo "Error: " . $e->getMessage();
}
$conn = null;
?>
