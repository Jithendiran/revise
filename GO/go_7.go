/**
* @author: E.K.Jithendiran
* @date: 19.6.2023
 */
package main

import (
	"fmt"
)

func switchFunc(value int) int {
	var result int

	// there is no break
	switch value = value + 1 - 1; value {
	case 1, 5:
		result = 1
	case 2, 9: // case 1, 9 or case 2, 5, 1 are error bcz 1, 5 is redudant
		result = 2
	default:
		result = -1
	}

	return result
}

func main() {
	fmt.Println(switchFunc(1)) // Op: 1
	fmt.Println(switchFunc(2)) // Op: 2
	fmt.Println(switchFunc(3)) // Op: -1
	fmt.Println(switchFunc(5)) // Op: 1

	// switch tag less syntax
	i := 59
	switch {
	case i <= 10:
		fmt.Println("i <= 10")
	case i <= 20: // any number less than 10 is valid for both cases. which case caught 1st that will be execute
		fmt.Println("i <= 20")
	case i <= 50:
		fmt.Println("case 50")
		fallthrough // It will allow next case to execute
	case i <= 30 || i <= 40:
		fmt.Println("Case is 30 or 40")
	case i <= 60: // if only caught in less than 60 case no OP will be execuled
	case i <= 70:
		fmt.Println("Case 60 or 70")
	default:
		fmt.Println("Default")
	}
	// Ip i = 10 or less than 10;Op: i <= 10
	// Ip i = 35 or less than 10;Op: case 50; Case is 30 or 40

	// Ip i = 50 or less than 50;Op: Case 50; Case is 30 or 40
	// Case 30 or 40 is also executed because of fallthrough
	// if fallthrough is found next case is not validated

	// Ip = 65; Op: Case 60 or 70

	// Ip = 59; Op:

	// Types switch case
	var j interface{} = []int{1, 2}
	switch j.(type) { //j.(type) -> get type
	case int:
		fmt.Println("J is Int")
	case float32:
		fmt.Println("J is float32")
	case float64:
		fmt.Println("J is float64")
	case string:
		fmt.Println("J is String")
	case []int: //slice
		fmt.Println("Start")
		break // below lines are not executed
		fmt.Println("End")
	//case can struct, function, array, ...
	default:
		fmt.Println("Others types")
	}
	// Ip:1;    Op:J is Int
	// Ip:1.0;  Op: is J is float64
	// Ip:true; Op: Others types
	// Ip:"HI"; Op: J is String
	// Ip: 'i'; Op: Others types
	// Ip: float32(3); Op: J is float32
	// Ip: []int{1, 2}; Op: start

	// fmt.Println(j.(type)) // Err: invalid syntax tree: use of .(type) outside type switch
}
