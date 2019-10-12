package main

import (
	"fmt"
	"math/cmplx"
	)

var (
	ToBe bool = false
	MaxInt uint64 = 1<<64 -1
	z complex128 = cmplx.Sqrt(-5+12i)
)

const Pi  = 3.14

func main() {
	const f = "%T(%v)\n"
	fmt.Printf(f, ToBe, ToBe)
	fmt.Printf(f, MaxInt, MaxInt)
	fmt.Printf(f,z,z)

	var i int
	var l float64
	var b bool
	var s string
	fmt.Printf("%v %v %v %q\n", i, l, b, s)

	v := 42
	fmt.Printf("%T", v)

	fmt.Println("hppy", Pi)

	const Truth  = true
	fmt.Println("Go rules?", Truth)
}
