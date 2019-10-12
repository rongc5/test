package main

import "fmt"

func main() {
	msg := make(chan string)
	signals := make(chan bool)

	select {
	case ms1 := <- msg:
		fmt.Println("received message", ms1)
	default:
		fmt.Println("no message received")
	}

	ms1 := "hi"
	select {
	case msg <- ms1:
		fmt.Println("sent message", ms1)
	default:
		fmt.Println("no message sent")
	}

	select {
	case ms1 := <- msg:
		fmt.Println("reveived message", ms1)
	case sig := <- signals:
		fmt.Println("received singal", sig)
	default:
		fmt.Println("no activity")
	}
}



