<?php

function myTest() {
    static $x = 1;
    $x++;
    echo $x, "<br>";
}

myTest();
myTest();
myTest();


