/**
* @author: E.K.Jithendiran
* @date: 20.6.2023
 */
package main

import (
	"fmt"
)

func main() {
	var w writer = consoleWriter{}
	w.write([]byte("Hello")) // like java upcasting

	myInt := intcounter(0)
	var inc incrementer = &myInt
	inc.increment()
	inc.increment()
	fmt.Println(myInt)
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
