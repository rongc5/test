package main

import "fmt"

func sum(nums ...int)  {
	fmt.Print(nums, " ")
	total := 0
	for _, num := range nums{
		total += num
	}

	fmt.Println(total)
}

func intSeq() func() int  {
	i := 0
	return func() int {
		i += 1
		return i
	}
}

func main() {
	sum(1, 2)
	sum(1, 2, 3)

	nums := []int{1,2,3,4}
	sum(nums...)

	nextInt := intSqe()

	fmt.Println(nextInt())
	fmt.Println(nextInt())
	fmt.Println(nextInt())

	newInts := intSeq()
	fmt.Println(newInts())

}