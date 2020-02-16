package main

import "fmt"

func split(sum int) (x, y int)  {
	x = sum * 4/9
	y = sum - x
	return
}

//var c, python, java bool

var i, j int = 1, 2

func main()  {
	fmt.Println(split(17))

	//var c, python, java = true, false, "no!"
	c, python, java := true, false, "no!"
	k := 3
	fmt.Println(i, j)
	var i int
	fmt.Println(i, c, python, java, k)
}