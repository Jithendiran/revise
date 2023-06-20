/**
* @author: E.K.Jithendiran
* @date: 20.6.2023
 */
package main

import (
	"fmt"
	"sync"
	"time"
)

var wg = sync.WaitGroup{} // It's responsility is to sync multiple the go routines
var mrw = sync.RWMutex{}  // read, write mutext
var counter = 0

func main() {
	// goroutine
	// withoutSync()
	// withWait()
	// loop()

	// with mutext what we can do is infinite read can allow but only one write is allow
	// when writing all read are going to wait
	loopMutex()
}

func withoutSync() {
	msg := "Hi"
	go func() {
		// msg from closure memory
		fmt.Println(msg)
	}()
	msg = "bye" // go routine race condition
	// when goroutine is initiated main goroutine (main thread) will continue on other works
	// Op: Hi11

	go func(msg string) {
		fmt.Println(msg, "2")
	}(msg)
	// Op: bye 2
	msg = "Hi11"
	time.Sleep(time.Millisecond * 100) // pause main thread to exit
	// above line is bad practise
}

func withWait() {
	msg := "Hi"
	wg.Add(1) // 1 more goroutine is running (that 1 is main)
	go func() {
		// msg from closure memory
		fmt.Println(msg)
		wg.Done() // tell main goroutine sub routine is completed
	}()
	wg.Add(1) // 2 more goroutine is running (that 1 is main, 2 is previous func)
	go func(str string) {
		fmt.Println(str)
		wg.Done()
	}("hello")

	wg.Wait() // wait untill routine is completed
}

func loop() {
	for i := 0; i < 10; i++ {
		wg.Add(2)
		go printValue()
		go incValue()
	}
	wg.Wait()

	/*
		Op
			Counter :  1
			Counter :  3
			Counter :  4
			Counter :  5
			Counter :  6
			Counter :  7
			Counter :  3
			Counter :  8
			Counter :  10
			Counter :  9
	*/
}
func printValue() {
	fmt.Println("Counter : ", counter)
	wg.Done()
}
func incValue() {
	counter++
	wg.Done()
}

func loopMutex() {
	for i := 0; i < 10; i++ {
		wg.Add(2)
		mrw.RLock()
		go printValueMutes()
		mrw.Lock()
		go incValueMutex()
		// These force it to run like single thread
	}
	wg.Wait()

	/*
		Op
			Counter :  0
			Counter :  1
			Counter :  2
			Counter :  3
			Counter :  4
			Counter :  5
			Counter :  6
			Counter :  7
			Counter :  8
			Counter :  9
	*/
}
func printValueMutes() {
	fmt.Println("Counter : ", counter)
	mrw.RUnlock()
	wg.Done()
}
func incValueMutex() {
	counter++
	mrw.Unlock()
	wg.Done()
}
