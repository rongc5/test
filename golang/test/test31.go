package main

import "fmt"
import "time"
import "sync/atomic"

func main() {
	var ops uint64 = 0

	for i := 0; i < 20; i++ {
		go func() {
			for {
				atomic.AddUint64(&ops, 1)
				time.Sleep(time.Millisecond)
				break
			}
		}()
	}

	time.Sleep(time.Second)

	opsFinal := atomic.LoadUint64(&ops)
	fmt.Println("ops:", opsFinal)
}

