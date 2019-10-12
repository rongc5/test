package main

import "fmt"

func main() {
	var a int = 100
	var b int = 200

	fmt.Printf("a %d b %d\n", a, b)

	swap(&a, &b)

	fmt.Printf("a %d b %d\n", a, b)
}

func swap(x *int, y * int)  {
	var tmp int
	tmp = *x
	*x = *y
	*y = tmp
}