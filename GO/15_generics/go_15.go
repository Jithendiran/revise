/**
* @author: E.K.Jithendiran
* @date: 23.6.2023
 */
package main

import "fmt"

type number interface {
	~int | ~int8 | ~int16 | ~int32 | ~int64 | ~float32 | ~float64
}
type numberArray interface {
	~[]int | ~[]int8 | ~[]int16 | ~[]int32 | ~[]int64 | ~[]float32 | ~[]float64
}
type text interface {
	~string | ~byte | ~rune
}
type textArray interface {
	~[]string | ~[]byte | ~[]rune
}

type value interface {
	number | text
}

type arrayValues interface {
	numberArray | textArray
}

type intNum int

// without ~ this alias type will not work
// [T type1, T2 type2, ...]
func add[T number](a T, b T) T {
	return a + b
}

func reduce[A value](arr []A, logic func(A, A) A) A {
	var res A
	for _, v := range arr {
		res = logic(res, v)
	}
	return res
}

type structType[S value | arrayValues] struct {
	id   int
	data S
}

func main() {
	fmt.Println(add(1, 2))              // 3
	fmt.Println(add(float32(1.1), 2.1)) // 3.1999998
	fmt.Println(add(float64(2.1), 3.1)) // 5.2
	fmt.Println(add(3, intNum(2)))      // 5.2

	fmt.Println(reduce([]int{1, 2, 3}, func(acc int, v int) int {
		return acc + v
	}))

	st := structType[[]int]{
		id:   1,
		data: []int{12}, //1,
	}
	fmt.Println(st)
}
