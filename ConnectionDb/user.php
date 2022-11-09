<?php

include 'conexion.php';

$id = $_GET["id"];

$consulta = "SELECT * FROM usuario WHERE id_user=$id";
$res =  $con->query($consulta);

for ($row_no = $res->num_rows - 1; $row_no >= 0; $row_no--) {
    $res->data_seek($row_no);
    $row = $res->fetch_assoc();

    echo $row['id_user'];
    echo $row['nome'];
    echo $row['saldo'];
    echo date('d/m/Y \a\s h:i:s', strtotime($row['tempo']));
}
