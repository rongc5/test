package main

import "fmt"

func main() {
	p := fmt.Printf
	var x interface{}

	switch i := x.(type) {
	case nil:
		p("%T", i)
	case int:
		p("int")
	case float64:
		p("float64")
	case func(int int) float64:
		p("func(int)")
	case bool, string:
		p("bool or string")
	default:
		p("don't konw the type")
	}

}
