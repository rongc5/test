<?php
$x = 5;
$y = 6;

function myTest() {
    global $x, $y;
    $y = $x + $y;
}

myTest();
echo $y;
?>
