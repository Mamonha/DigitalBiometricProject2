<?php

include 'connection.php';
date_default_timezone_set('America/Sao_Paulo');
if ($con) {
    echo "Successfully connected";

    print_r($_POST);

    /*if(isset($_POST['nome'])) {
        $nome = $_POST['nome'];
        echo " nome : ".$nome;

    */
}

/* if(isset($_POST['id_user'])) { 
        $id = $_POST['id_user'];
        
        echo "id:".$id;
     */

$consulta = "SELECT * FROM usuario";
$res =  $con->query($consulta);

echo "
            <title>Sistema biométrico</title>
        
            <style>
                * {
                    box-sizing: border-box;
                    padding: 0;
                    margin: 0;
                    font-family: comic sans ms;


                   color: #247BA0;  
                }    

                }
                body {
                    width: 100%;
                    margin: auto;
                }
                
                table {
                    overflow:hidden;
                    width:100%;
                    max-width: 900px;
                    border-collapse: collapse;
                    margin:auto;
    
                }
        
              
                tr:hover {
                    cursor:pointer; 
                    transition:all 0.15s ease;
                    background:pink;
                }

                th{
                    background:pink;
                }

                th,
                td {
                    padding: 10px;
                    border: 2px solid #555;
                    text-align: center;
                }
            </style>
        </head>
        <body>
            <table>
                <thead>
                    <tr>
                        <th>ID</th>
                        <th>Nome</th>
                        <th>Saldo</th>
                        <th>Ultimo acesso</th>

                    </tr>
                </thead>
                <tbody>";



echo "<script>
        
        
                function redirecionar(id){
                    window.location.href = 'user.php?id=' + id;

                }
        
        
            </script>
        ";


for ($row_no = $res->num_rows - 1; $row_no >= 0; $row_no--) {
    $res->data_seek($row_no);
    $row = $res->fetch_assoc();
    echo "<tr onclick=redirecionar(" . $row['id_user'] . ")>";
    echo "<td>" . $row['id_user'] . "</td>";
    echo "<td>" . $row['nome'] . "</    td>";
    echo "<td>" . $row['saldo'] . "</td>";
    echo "<td>" . date('d/m/Y \a\s h:i:s', strtotime($row['tempo'])) . "</td>";
    echo "</tr>";
}


echo "</tbody>

        </table>
    </body>";


   
