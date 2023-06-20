/**
* @author: E.K.Jithendiran
* @date: 16.6.2023
 */
package main

// main function in Package main is the entry point for program

// import "fmt"
import (
	"fmt"
	"strconv"
)

// Data Types
// bool
// string
// int int8 int16 int32 int64
// uint uint8 uint16 uint32 uint64 uintptr
// byte // alias for uint8
// rune // alias for int32, Represents a Unicode point
// float32 float64
// complex64 complex128

// Package level variables
var i int = 6

// any file in the package main can access i
// j := 2; // not possible

var k = 4.

var (
	name = "E.K.Jithendiran"
	date = "16/6/2023"
	age  = 22
)

// Package END
var I int = 27

// any packages other than main also access I
// lowercase first letter variables are package level
// uppercase first letter variables are exposed to outer packages
// no private scope

/**
* Naming conventions
* Pascal or camelCase
*   Capitalize acronyms (HTTP, URL)
 */

func main() {
	// block scope variables
	var str string = "hello"
	var pi = 3.14
	var a int8                                 //declare must specify type
	a = 1                                      // define
	b := 2                                     // declare and define -> var b = 2;
	fmt.Println("Hello ")                      // Hello
	fmt.Printf("%d %d %s %f\n", a, b, str, pi) // 1 2 hello 3.140000
	fmt.Println(i, k)                          // 6 4
	// Variables can be used by 3 types

	var i int      // varable i is a int type
	fmt.Println(i) // 0
	i = 2
	fmt.Println(i) // 2

	var j int = 3
	fmt.Println(j) // 3

	// j := 5 // ERR: no new variables on left side of :=
	// cannot redeclare varable in same scope

	k := 3
	k = 4
	fmt.Println(k) // 4

	fmt.Printf("Value : %v Type : %T \n", str, str) // Value : hello Type : string

	// variable conversions
	var in = 42
	var fl float32 = float32(in)    // explict type conversion, implicit is not possible
	fmt.Printf("%f : %T\n", fl, fl) // 42.000000 : float32
	// var st string = string(fl) //  ERR:cannot convert fl (variable of type float32) to type string
	fmt.Printf("%s\n", string(in)) // *
	// unicode of * is 42

	// To Convert actual string format
	fmt.Printf("%s %T\n", strconv.Itoa(in), strconv.Itoa(in)) //42 string

	var HTTP = "http" // It also exposed to outer packages
	println(HTTP)     // http

	// fmt.Println(int16(1) + int32(2));// Err: (mismatched types int16 and int32)
	println("HI Hello"[3])         // 72
	println(string("HI Hello"[3])) // H
	str = "Hello rewrite"
	println(str) // Hello rewrite
	// str[2] = "i" // Err:  cannot assign to str[2] (value of type byte)
	// str[2] = 65 // Err:  cannot assign to str[2] (value of type byte)
	println("String " + "Concat") // String Concat

	// string to collection of bytes
	byt := []byte(str)
	fmt.Println(byt) // [72 101 108 108 111 32 114 101 119 114 105 116 101]

	// "" -> string -> utf-8 -> uint8
	// '' -> rune -> utf-32 -> uint32
	// fmt.Println('i'+"i") // 'i' + "i" (mismatched types untyped rune and untyped string)

	type counter int
	var ic counter = 0
	fmt.Printf("%d, %T\n", ic, ic) // 0, main.counter
}

// Run
// go run go1_1.go

// Build
// go build go_1.go -> generate executable file
