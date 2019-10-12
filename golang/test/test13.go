package main

import "fmt"

func fact(n int) int {
	if n == 0 {
		return 1
	}

	return n * fact(n - 1)
}

func zeroval(ival int)  {
	ival = 0
}

func zeroptr(iptr * int)  {
	*iptr = 0
}

func main() {
	fmt.Println(fact(7))

	i := 1
	fmt.Println("initial:", i)
	zeroval(i)
	fmt.Println("zeroval:", i)
	zeroptr(&i)
	fmt.Println("zeroptr:", i)

	fmt.Println("pointer:", &i)
}