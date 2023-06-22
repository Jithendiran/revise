/**
* @author: E.K.Jithendiran
* @date: 21.6.2023
 */
package main

type closer1 interface {
	close1() error
}

type writer1 interface {
	write1([]byte) (int, error)
}

// composing interface
type writecloser1 interface {
	writer1
	closer1
}

type myWriteCloser1 struct{}

// If atleast one implemented method of writecloser1 is receiving pointer all the methods are accept pointer
func (mwc *myWriteCloser1) write1(data []byte) (int, error) {
	return 0, nil
}

func (mwc myWriteCloser1) close1() error {
	return nil
}

type myWriteCloser2 struct{}

// If all implemented method of writecloser1 is receiving pass value or pointer
func (mwc myWriteCloser2) write1(data []byte) (int, error) {
	return 0, nil
}

func (mwc myWriteCloser2) close1() error {
	return nil
}

func main() {
	// var wc writecloser1 = myWriteCloser1{}
	// Err:  cannot use myWriteCloser1{} (value of type myWriteCloser1) as writecloser1 value in variable declaration: myWriteCloser1 does not implement writecloser1 (method write1 has pointer receiver)
	var wc writecloser1 = &myWriteCloser1{}
	// accept only pointer
	wc.write1([]byte("Hi"))
	wc.close1()

	// accept pointer as well as value
	var wc2 writecloser1 = myWriteCloser2{}
	wc2.write1([]byte("Hi"))
	wc2.close1()

	var wc3 writecloser1 = &myWriteCloser2{}
	wc3.write1([]byte("Hi"))
	wc3.close1()
}
