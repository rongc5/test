package main

import "fmt"

func main() {
	qe := make(chan string, 2)
	qe <- "one"
	qe <- "two"
	close(qe)

	for em := range qe {
		fmt.Println(em)
	}
}
