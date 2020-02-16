package main

import "fmt"
import "math/rand"
import "sync"
import "sync/atomic"
import "time"

func main() {
	var state = make(map[int]int)
	var mutex = &sync.Mutex{}

	var readOps uint64 = 0
	var writeOps uint64 = 0

	for r := 0; r < 20; r++ {
		go func() {
			total := 0
			key := rand.Intn(3)
			mutex.Lock()
			total += state[key]
			mutex.Unlock()
			atomic.AddUint64(&readOps, 1)
			fmt.Println("total:", total)
			time.Sleep(time.Millisecond)
		}()
	}

	for w := 0; w < 10; w++ {
		go func() {
			key := rand.Intn(3)
			val := rand.Intn(100)
			mutex.Lock()
			state[key] = val
			mutex.Unlock()
			atomic.AddUint64(&writeOps, 1)
			time.Sleep(time.Millisecond)
		}()
	}

	time.Sleep(time.Second)

	readOPsFinal := atomic.LoadUint64(&readOps)
	fmt.Println("readOps:", readOPsFinal)
	writeOpsFinal := atomic.LoadUint64(&writeOps)
	fmt.Println("writeOps:", writeOpsFinal)

	mutex.Lock()
	fmt.Println("state:", state)
	mutex.Unlock()
}
