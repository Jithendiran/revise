/**
* @author: E.K.Jithendiran
* @date: 20.6.2023
 */
package main

import (
	"fmt"
	"strconv"
)

// function overloading not supported
// main function
func main() {
	greetFunc("Hi ", "you", 1) //Hi you 1
	a, b := "a", "b"
	fmt.Println("A : ", a, "B: ", b) // A :  a B:  b
	swap(&a, &b)
	fmt.Println("A : ", a, "B: ", b) // A :  b B:  a
	c, b, d := sum(1, 2, 3)
	//  [1 2 3]
	fmt.Println(c, b, d, *d)
	/*
		Op
		 6 Ok 0xc0000180f0 6
	*/
	c, err := sum1(1, 2, 3, 4)
	fmt.Println(c, err)
	// 10 <nil>
	c, err = sum1(-1, 2, -3, -4)
	fmt.Println(c, err)
	// -6 -ve Value error

	// annonyms function with immediate invoke function
	func() {
		fmt.Println("Immediate invoke")
	}()
	// Immediate invoke

	// in go functions are treated as types (like data type)
	f := func(i int) {
		fmt.Println("Function type : ", i)
	}
	f(1)
	// Function type :  1
	var f1 func(int)
	f1 = func(i int) {
		fmt.Println("Function type F1: ", i)
	}
	f1(1)
	// Function type F1:  1

	var f2 func(int) string
	// fmt.Println(f2(1)) // Err: invalid memory address or nil pointer dereference
	f2 = func(i int) string {
		return "Function type F2: " + strconv.Itoa(i)
	}
	fmt.Println(f2(1))
	// Function type F2: 1

	// This type of function must be outside
	// func divid( a , b int) (float32, error){
	// 	if b == 0 {
	// 		return 0.0, fmt.Errorf("2nd value != 0")
	// 	}
	// 	return float32(a/b), nil;
	// }

	g := greeter{
		"Jith",
		"Welcome",
	}
	g.greetFunc()
	// Jith Welcome
	fmt.Println(g)
	// {Jith Welcome}
	g.greetFuncPtr()
	// Jith Welcome
	fmt.Println(g)
	// {Jith }
}

// if function 1st letter is caps then it will be exported to outer packages

// msg no need variable type because next parameter is also same type compiler will automatically make it
func greetFunc(msg, name string, id int) {
	fmt.Println(msg + name + " " + strconv.Itoa(id))
}

func swap(name1, name2 *string) {
	tmp := *name1
	*name1 = *name2
	*name2 = tmp
}

// variable length argument must be the last argument
// only one variable length argument is allowed
func sum(value ...int) (int, string, *int) {
	fmt.Println(value)
	// value act like a slice
	result := 0
	for _, v := range value {
		result += v
	}
	// in golang return variable is promoted to shared memory
	// so return variable is not destroyed
	return result, "Ok", &result
}

// named return function
func sum1(value ...int) (result int, err error) {
	err = nil
	for _, v := range value {
		result = result + v
	}
	if result < 0 {
		err = fmt.Errorf("-ve Value error")
	}
	// in Named return we no need to provied name in last return
	return
}

// struct method
type greeter struct {
	name  string
	greet string
}

// It is method not function
// It will work with known context (g greeter)
// context can be any type int,  struct,...
func (g greeter) greetFunc() {
	// g is a copy not reference
	fmt.Println(g.name, g.greet)
	g.greet = ""
}

func (g *greeter) greetFuncPtr() {
	// g is a reference
	fmt.Println(g.name, g.greet)
	g.greet = ""
}

// func (g func(int)) function() {
// 	fmt.Println(g)
// }
// Err:  invalid receiver type func(int)
