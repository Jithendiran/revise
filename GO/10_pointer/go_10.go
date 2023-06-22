/**
* @author: E.K.Jithendiran
* @date: 20.6.2023
 */
package main

import (
	"fmt"
)

func main() {
	a := 27
	b := a
	b = 12
	fmt.Println(a, b) // 27 12

	var aint int = 27
	var aptr *int = &aint
	fmt.Println(&aint, aptr) // 0xc0000180c0
	fmt.Println(aint, *aptr) // 27
	*aptr = 12
	fmt.Println(aint, *aptr) // 12

	intArray := [3]int{1, 2, 3}
	aptr = &intArray[0]
	// aptr + 1 // Err: aptr + 1 (mismatched types *int and untyped int)

	var ms *myStruct
	fmt.Println(ms)                  //<nil>
	ms = new(myStruct)               // this will init struct type store 0 and return address
	fmt.Println(ms, ms.id, (*ms).id) //&{0} 0 0
	(*ms).id = 27
	fmt.Println(ms, ms.id, (*ms).id) //&{27} 27 27
	ms.id = 12                       // compiler will translate to (*ms).id
	fmt.Println(ms, ms.id, (*ms).id) //&{12} 12 12
}

type myStruct struct {
	id int
}
