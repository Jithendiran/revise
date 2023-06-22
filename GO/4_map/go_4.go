/**
* @author: E.K.Jithendiran
* @date: 19.6.2023
 */
package main

import (
	"fmt"
)

func main() {
	// map
	// order of a map is not guarenteed
	abcMap := map[string]int{
		"a": 1,
		"b": 2,
	}

	fmt.Println(abcMap)        // map[a:1 b:2]
	fmt.Printf("%T\n", abcMap) // map[string]int

	var fltMap map[int]float32 = map[int]float32{
		1: 1.1,
		2: 2.2,
	}
	fmt.Println(fltMap) // map[1:1.1 2:2.2]
	// map key can be int, float, bool, string and array
	// slice is invalid of map key

	makeMap := make(map[float32]string)
	makeMap = map[float32]string{
		1.0: "a",
		2.1: "b",
	}
	makeMap[2.2] = "c"
	delete(makeMap, 1.0)

	fmt.Println(makeMap)      // map[2.1:b 2.2:c]
	fmt.Println(makeMap[2.1]) // b
	fmt.Println(makeMap[7])   //
	fmt.Println(makeMap[1.0]) //

	value, ok := makeMap[7]
	fmt.Println(value, ok) // false

	value, ok = makeMap[2.1]
	fmt.Println(value, ok) //b true
	// if OK is True then it has key
	// if OK is False then it has no key
	fmt.Println(len(makeMap)) // 2

	// Map is a pass by reference
	refMap := makeMap
	delete(refMap, 2.2)
	fmt.Println(makeMap) //map[2.1:b]
	fmt.Println(refMap)  //map[2.1:b]
}
