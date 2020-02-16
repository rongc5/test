package main

import "time"
import "fmt"

func main() {
	tk := time.NewTicker(time.Millisecond * 500)

	go func() {
		for t := range tk.C {
			fmt.Println("tick at", t)
		}
	}()

	time.Sleep(time.Millisecond * 1600)
	tk.Stop()
	fmt.Println("tick stopped")
}
