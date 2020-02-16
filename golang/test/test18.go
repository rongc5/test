package main

import "fmt"

func main() {

	msg := make(chan string)

	go func() {
		msg <- "ping"
	}()

	ms := <- msg

	fmt.Println(ms)
}
