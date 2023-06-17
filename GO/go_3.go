/**
* @author: E.K.Jithendiran
* @date: 17.6.2023
 */
package main

import (
	"fmt"
)

func main() {
	// Array
	// array must be fixed size
	intarry := [3]int{1, 2, 3} // method 1
	fmt.Println(intarry)       // [1 2 3]
	intarry2 := [...]int{4, 5, 6, 7}
	fmt.Println(intarry2) // [4 5 6 7]

	var strarry [5]string              // method 2
	fmt.Println(strarry, len(strarry)) // [] 2
	strarry[0] = "E.K."
	strarry[1] = "Jithendiran"
	// strarry[3] = "hello" // Err: index 2 out of bounds [0:2]
	fmt.Println(strarry, len(strarry), cap(strarry)) // [E.K. Jithendiran], 3, 3

	// 2d array
	var int2darray [3][3]int = [...][3]int{[3]int{1, 2, 3}, [3]int{4, 5, 6}, [3]int{7, 8, 9}}
	fmt.Println(int2darray) // [[1 2 3] [4 5 6] [7 8 9]]

	int2darray2 := [...][3]int{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}
	fmt.Println(int2darray2) // [[1 2 3] [4 5 6] [7 8 9]]

	var int2darray3 [2][2]int
	int2darray3[0] = [2]int{1, 2}
	int2darray3[1] = [2]int{3, 4}
	fmt.Println(int2darray3) // [[1 2] [3 4]]

	// arrays clone
	int2darray3copy := int2darray3
	int2darray3copy[0][0] = 27
	fmt.Println(int2darray3, int2darray3copy) // [[1 2] [3 4]] [[27 2] [3 4]]
	// it is a deep copy no pointer reference

	int2darray3copy2 := &int2darray3           // both has same data
	int2darray3copy2[0][1] = 27                // both values are polluted
	fmt.Println(int2darray3, int2darray3copy2) // [[1 27] [3 4]] &[[1 27] [3 4]]

	// slice
	// slice not have fixed element
	intslice1 := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	fmt.Println(intslice1) // [1 2 3 4 5 6 7 8 9 10

	// slice are naturally reference type
	intslice1Ref := intslice1
	intslice1Ref[0] = 11
	fmt.Println(intslice1, intslice1Ref) // [11 2 3 4 5 6 7 8 9 10] [11 2 3 4 5 6 7 8 9 10]
	intsli2 := intslice1[:]              // all element of slice
	intsli2[0] = 1
	intsli3 := intslice1[3:]  // slice from 3rd index to last
	intsli4 := intslice1[:6]  // slice from 0 th index to 5th index
	intsli5 := intslice1[2:4] // 2nd index, 3rd index
	fmt.Println(intsli2)      // [1 2 3 4 5 6 7 8 9 10]
	fmt.Println(intsli3)      // [4 5 6 7 8 9 10]
	fmt.Println(intsli4)      // [1 2 3 4 5 6]
	fmt.Println(intsli5)      // [3 4]

	// array slice
	fmt.Println(intarry2[:2]) // [4 5]

	//types print
	fmt.Printf("%d %T , %d %T\n", intarry2, intarry2, intslice1, intslice1) //[4 5 6 7] [4]int , [1 2 3 4 5 6 7 8 9 10] []int

	// make
	a := make([]int, 3, 5) // type, length, capacity (capacity is optional) if capacity is not given it takes length value as capacity
	fmt.Println(a)         // [0 0 0]
	fmt.Println(len(a))    // 3
	fmt.Println(cap(a))    // 5

	// a[3] = 1                // Err:runtime error: index out of range [3] with length 3
	a = append(a, 1)
	// append will create new array by copying specified array with element and assign it

	fmt.Println(a)      // [0 0 0 1]
	fmt.Println(len(a)) // 4
	fmt.Println(cap(a)) // 5

	a = append(a, 1, 1) // append from 2nd argument is only int
	fmt.Println(a)      // [0 0 0 1 1 1]
	fmt.Println(len(a)) // 6
	fmt.Println(cap(a)) // 10
	// capacity is increase by 2
	// it doubles by previous size

	// a = append(a, []int{2, 3, 4}) // Err: cannot use []int{…} (value of type []int) as int value in argument to append
	a = append(a, []int{2, 3, 4, 5, 6}...) // it works like js spread
	fmt.Println(a)                         // [0 0 0 1 1 1 2 3 4 5 6]
	fmt.Println(len(a))                    // 11
	fmt.Println(cap(a))                    // 20

	var b = []int{}
	fmt.Println(b)      // []
	fmt.Println(len(b)) // 0
	fmt.Println(cap(b)) // 0

	b = append(b, 1)
	fmt.Println(b)      // [1]
	fmt.Println(len(b)) // 1
	fmt.Println(cap(b)) // 1

	b = append(b, []int{2, 3, 4, 5, 6, 7, 8, 9}...)
	fmt.Println(b)      // [1 2 3 4 5 6 7 8 9]
	fmt.Println(len(b)) // 9
	fmt.Println(cap(b)) // 10
	c := append(b[:2], b[6:]...)
	fmt.Println(c) // [1 2 7 8 9]
	fmt.Println(b) //[1 2 7 8 9 6 7 8 9]
	// b is also changed removed 3rd element and filled missing with last element

	// Till date there is no function to copy the slice only need to loop
}
