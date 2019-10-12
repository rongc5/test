package main

import (
	"math"
	"fmt"
)

func getAvage(arr []int, size int) float32 {
	var avg float32
	var sum int
	for i:= 0; i < size; i++ {
		sum += arr[i]
	}

	avg = float32(sum/size)

	return avg
}

func main() {
	sr := func(x float64) float64 {
		return math.Sqrt(x)
	}

	fmt.Println(sr(9))

	var balance = []int{1000, 2, 3, 17, 50}
	fmt.Printf("len:%d\n", len(balance))
	avg := getAvage(balance, len(balance))

	fmt.Printf("%f", avg)
}
