/**
* @author: E.K.Jithendiran
* @date: 19.6.2023
 */
package main

import (
	"fmt"
)

func main() {

	for i, j := 0, 3; i < j; i, j = i+1, j-1 {
		fmt.Println(i, j)
	}
	/*
	 Op:
	 	0 3
	 	1 2
	*/
	// fmt.Println(i, j) // Err: i,j undefined

	k := 0

	for ; k < 10; k++ {
		if k == 3 {
			break
		}
		fmt.Println(k)
	}
	/*
		 Op:
		 	0
		 	1
			2
	*/
	k = 0
	for { // or for ; ; ;{}
		if k >= 8 {
			break
		}
		fmt.Println(k)
		k += 2
	}
	/*
		 Op:
			0
			2
			4
			6
	*/

	k = 0
	for k <= 8 { // like while
		fmt.Println(k)
		k += 2
	}
	/*
		Op:
			0
			2
			4
			6
			8
	*/

	for i := 0; i < 10; i++ {
		if i%2 == 0 {
			continue
		}
		fmt.Println(i)
	}
	/*
		Op:
			1
			3
			5
			7
			9
	*/
	fmt.Println("Label")
Loop:
	for i := 0; i < 3; i++ {
		for j := 0; j < 5; j++ {
			fmt.Println(i, j, "Start")
			if i == 1 && j == 3 {
				continue // inner loop continue
			} else if i == 0 && j == 3 {
				continue Loop // continue to outer loop
			} else if i == 2 && j == 3 {
				break Loop // break outer loop also
			}
			fmt.Println(i, j, "END")
		}
	}
	/*
		Op
			0 0 Start
			0 0 END
			0 1 Start
			0 1 END
			0 2 Start
			0 2 END
			0 3 Start
			// continue Loop
			1 0 Start
			1 0 END
			1 1 Start
			1 1 END
			1 2 Start
			1 2 END
			1 3 Start
			// continue
			1 4 Start
			1 4 END
			2 0 Start
			2 0 END
			2 1 Start
			2 1 END
			2 2 Start
			2 2 END
			2 3 Start
			// break Loop
	*/

	slice := []int{1, 2, 3}

	for k, v := range slice { // same for array, map, strings, channels, slices
		fmt.Println(k, v)
	}
	/*
		Op
			0 1
			1 2
			2 3
	*/

	abcMap := map[string]int{
		"a": 1,
		"b": 2,
	}
	for k, v := range abcMap {
		fmt.Println(k, v)
	}
	/*
		Op
			b 2
			a 1
	*/

	for k := range abcMap {
		fmt.Println(k) // a b
	}

	for k, v := range "Jithendiran" {
		fmt.Println(k, v, string(v)) // a b
	}
	/*
		Op
			0 74 J
			1 105 i
			2 116 t
			3 104 h
			4 101 e
			5 110 n
			6 100 d
			7 105 i
			8 114 r
			9 97 a
			10 110 n
	*/
}
