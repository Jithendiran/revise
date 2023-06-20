/**
* @author: E.K.Jithendiran
* @date: 17.6.2023
 */
package main

import (
	"fmt"
	// "math"
)

func main() {
	// constants
	// Constants are immutable

	// untyped constant
	const pI = 3.14 // If 1st letter is upper case it will be exposed to outer packages

	// typed constant
	const nAME string = "E.K.Jithendiran"

	// const mySin = math.Sin(2) // Err: const must be store it's value in compile time not in run time
	fmt.Println(pI, nAME) // 3.14 E.K.Jithendiran

	// const i int8 = 2
	// const j int16 = 3456
	// i + j // Err:  invalid operation: i + j (mismatched types int8 and int16)
	// The above error is only happen for typed constant

	const k = 2
	const l int64 = 5
	fmt.Println(k + l)        // 7
	fmt.Println(k + int32(8)) // 10
	// The above line does not give error bcz while compilation all k will be replaced to 2 and type is defined while operation

	// enumerated constant
	const (
		a = iota
		b = iota
		c
		d // iota auto increment
		e
	)

	const f = iota                   // iota reset
	const g = iota                   // iota reset
	fmt.Println(a, b, c, d, e, f, 0) //0 1 2 3 4 0

	const (
		a1 = iota + 1
		b1
		c1
	)
	fmt.Println(a1, b1, c1) // 1 2 3

	const (
		_ = iota // _ is write only
		a2
		b2
	)
	// fmt.Println(_) // Err: cannot use _ as value or type
	fmt.Println(a2, b2) // 1 2
}
