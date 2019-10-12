package main

import (
	"fmt"
	"math"
	"time"
)

const (
	Big = 1 << 100
	Small = Big >> 99
)

func needInt(x int) int {
	return x*10 + 1
}

func needFloat(x float64) float64 {
	return x * 0.1
}

func sqrt(x float64) string {
	if x < 0 {
		return sqrt(-x) + "i"
	}

	return fmt.Sprint(math.Sqrt(x))
}

func pow(x, n, lim float64) float64 {
	if v:= math.Pow(x, n); v < lim {
		return v
	} else {
		fmt.Printf("%g >= %g \n", v, lim)
	}

	return lim
}

func main() {
	//fmt.Println(needFloat(Small))
	//fmt.Println(needInt(Small))
	//fmt.Println(needFloat(Big))
	//
	//sum := 0
	//for i := 0; i < 10; i++ {
	//	sum += i
	//}

	//for ; sum < 1000; {
	//	sum += sum
	//}

	//for sum < 1000 {
	//	sum += sum
	//}
	//fmt.Println(sum)
	//
	//fmt.Println(sqrt(2), sqrt(4))
	//
	//fmt.Println(pow(3, 2, 10), pow(3, 3,20))

	//fmt.Print("Go runs on ")
	//switch os := runtime.GOOS;os {
	//case "darwin":
	//	fmt.Println("OS X.")
	//case "linux":
	//	fmt.Println("Linux")
	//default:
	//	fmt.Printf("%s.", os)
	//}
	//
	//fmt.Println("what's Saturday?")
	//today := time.Now().Weekday()
	//switch time.Saturday {
	//case today + 0:
	//	fmt.Println("Today")
	//case today + 1:
	//	fmt.Println("tommorrow")
	//case today + 2:
	//	fmt.Println("in two days")
	//default:
	//	fmt.Println("Too far away")
	//}

	t := time.Now()
	switch  {
	case t.Hour() < 12:
		fmt.Println("Good morning")
	case t.Hour() < 17:
		fmt.Println("Good afternoon")
	default:
		fmt.Println("Good evening")

	}

	defer fmt.Println("world")

	for i:=0; i< 10; i++ {
		defer fmt.Println(i)
	}

	fmt.Println("hello")

}
