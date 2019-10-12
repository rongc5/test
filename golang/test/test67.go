package main

import "fmt"

func main() {
	var b int = 15
	var a int

	p := fmt.Printf
	numbers := []int{1,2,3,5}

	for a := 0; a < 10; a++ {
		p("a:%d\n", a)
	}

	for a < b {
		a++
		p("a:%d\n", a)
	}

	for i, x := range numbers {
		p("x = %d at %d\n", x, i)
	}
}
