/**
* @author: E.K.Jithendiran
* @date: 19.6.2023
 */
package main

import (
	"fmt"
	"log"
)

func main() {

	deferFunc()
	panicFunc(1)
	// Op
	// start
	// Before exit
	// 2023/06/19 16:56:02 Error :  1 is not allowed
	panicFunc(0)
	// Op
	// start
	// Before exit
	// 2023/06/19 16:56:02 Error :  runtime error: integer divide by zero
	panicFunc(4)
	// Op
	// start
	// 0
	// End
	// Before exit
}

func deferFunc() {
	// defer
	j := 1
	fmt.Println("Start")
	defer fmt.Println("middle")
	defer fmt.Println("middle", j) // j is taken when the line encounter
	j = 2
	fmt.Println("End", 2)
	/*
		Op
			Start
			End 2
			middle 1
			middle
	*/
	// defer execute after all the function statement and before function return
	// defer function are execute in last in first out order
}

func panicFunc(b int) {
	// GO has no Exception
	println("start")
	defer func() {
		if err := recover(); err != nil { // if no error err is nil else it has error
			log.Println("Error : ", err)
		}
	}()
	defer fmt.Println("Before exit") // It execute when function is exit even it is in panic
	if b == 1 {
		panic("1 is not allowed")
	}
	a := 1
	fmt.Println(a / b)
	fmt.Println("End")
}
