/**
* @author: E.K.Jithendiran
* @date: 19.6.2023
 */
package main

import (
	"fmt"
)

func returnBool() bool {
	return true
}

func main() {

	//expression want to be boolean
	if true {
		fmt.Println("Test If")
	}
	// Op: Test If

	// if 1 {
	// 	fmt.Println("Test If")
	// }
	// Err : non-boolean condition in if statement

	// if must need curly brace even if it has one control line

	/*
		if true
		fmt.Println("Test If")
	*/
	// Err: syntax error: unexpected newline, expected { after if clause

	// initalizer if
	abcMap := map[string]int{
		"a": 1,
		"b": 2,
	}

	// 1st part of the if statement is initalizer
	//then it is seprated by ',' last part must be a boolean result
	if cValue, ok := abcMap["c"]; ok {
		fmt.Println("Has C : ", cValue)
	} else {
		fmt.Println("Has No C : ", cValue)
	}
	// Op: Has No C :  0

	// fmt.Println(cValue) // it's scope is only inside if

	// comparsion operator
	// >,<, ==, >=, <=, !=
	// also use || or, && and

	if "ab" > "ac" {
		fmt.Println("yes")
	} else {
		fmt.Println("No")
	}
	// Op: No

	if "ab" < "ac" {
		fmt.Println("yes")
	} else {
		fmt.Println("No")
	}
	// Op: yes

	if "ac" > "ac" {
		fmt.Println("yes")
	} else {
		fmt.Println("No")
	}
	// Op: No

	if "ac" == "ac" {
		fmt.Println("yes")
	} else {
		fmt.Println("No")
	}
	// Op: yes

	if "ac" != "ac" {
		fmt.Println("yes")
	} else {
		fmt.Println("No")
	}
	// Op: No

	if "AC" != "ac" {
		fmt.Println("yes")
	} else {
		fmt.Println("No")
	}
	// Op: yes

	if "AC" != "ac" && "AC" < "ac" {
		fmt.Println("yes")
	} else {
		fmt.Println("No")
	}
	// Op: yes
	// fmt.Println(!0) // Err: invalid operation: operator ! not defined on 0 (untyped int constant)
	fmt.Println(!false) // true

	if returnBool() {
		fmt.Println("Func return true")
	}
	// Op: Func return true

	if false {
		fmt.Println("1st")
	} else if true {
		fmt.Println("2nd")
	} else {
		fmt.Println("Last")
	}
	// Op: 2nd

	if 0.1 == 0.1000 {
		fmt.Println("yes")
	}
	// Op: yes
}
