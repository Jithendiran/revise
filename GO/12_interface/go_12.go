/**
* @author: E.K.Jithendiran
* @date: 20.6.2023
 */
package main

import (
	"fmt"
	"reflect"
)

type square struct {
	length float32
}

func (s square) area() float32 {
	return s.length * s.length
}

func (s square) circumference() float32 {
	return 4 * s.length
}

type circle struct {
	radius float32
}

func (c circle) area() float32 {
	return 3.14 * c.radius * c.radius
}

func (c circle) circumference() float32 {
	return 3.14 * 2 * c.radius
}

type shape interface {
	// interfaces groups types based on the methods
	// a int // Err: only methods supported
	area() float32
	circumference() float32
}

func getShape(s shape) {
	fmt.Printf("%T - Area : %f\n", s, s.area())
	fmt.Printf("%T - circumference : %f\n", s, s.circumference())
}

// empty interface
type emptyInterface interface{}

func typeInterface(v emptyInterface) {
	fmt.Printf("%v, %T\n", v, v)
}

func main() {

	var sh shape = circle{3}
	fmt.Println(sh.area())          // 28.26
	fmt.Println(sh.circumference()) // 18.84

	getShape(sh)
	/*
		Op
			main.circle - Area : 28.260000
			main.circle - circumference : 18.840000
	*/

	sh = square{2}
	fmt.Println(sh.area())          // 4
	fmt.Println(sh.circumference()) //8
	getShape(sh)
	/*
		Op
			main.square - Area : 4.000000
			main.square - circumference : 8.000000
	*/
	itype := 0
	typeInterface(itype) // 0, int
	typeInterface(sh)    // {2}, main.square

	var w writer = consoleWriter{}
	w.write([]byte("Hello")) // like java upcasting

	myInt := intcounter(0)
	var inc incrementer = &myInt
	inc.increment()
	inc.increment()
	fmt.Println(myInt)

	// type assertion
	func(s shape) {
		// fmt.Println(s.length) // not accessable
		//  length is not known to shape because it does not have
		ts := s.(square) // convert shape to underlying circle type
		fmt.Println(ts.length)
		fmt.Printf("%T - Area : %f\n", s, s.area())
		fmt.Printf("%T - circumference : %f\n", s, s.circumference())
	}(sh)

	// Type switch
	var ty interface{} = sh
	ty1 := reflect.TypeOf(sh)
	fmt.Println("Empty Type : ", ty) // Empty Type :  {2}
	fmt.Printf("%v %T\n", ty1, ty1)  // main.square *reflect.rtype
	switch sh.(type) {
	case circle:
		fmt.Println("Circle")
	case square:
		fmt.Println("Square")
	default:
		fmt.Println("Other type")
	}
	// Op: Square
}

// interface is a type like struct
// naming convention if a interface has only one method interface name is end with er
type writer interface {
	// interface are don't describe data it describes behaviour
	write([]byte) (int, error)
	// any thing implement this interface that will take ip and write
}

type consoleWriter struct{}

// no explict implement of interface in GO. go is implicit implement interface
func (cw consoleWriter) write(data []byte) (int, error) {
	n, err := fmt.Println(string(data))
	return n, err
}

type incrementer interface {
	increment() int
}

type intcounter int

func (ic *intcounter) increment() int {
	(*ic)++
	return int(*ic)
}
