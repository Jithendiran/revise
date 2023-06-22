/**
* @author: E.K.Jithendiran
* @date: 21.6.2023
 */
package main

import (
	"fmt"
	"sync"
)

// channels
// it will eliminate race condition
// channel must inside go routine
// without listening channel can't write value and vice versa

var wg = &sync.WaitGroup{}

func main() {
	// channel()
	// channelLoop()
	getReceive()
}

func channel() {
	ch := make(chan int) // create channel of typ eint
	wg.Add(1)
	go func() {
		i := <-ch // i get value from channel
		fmt.Println(i)
		wg.Done()
	}()

	wg.Add(1)
	go func() {
		i := 27
		ch <- i // put to channel
		i = 12  // this will not pass
		wg.Done()
	}()
	wg.Wait()
}

func channelLoop() {
	ch := make(chan int) // create channel of type int
	for i := 0; i < 10; i++ {
		wg.Add(2)
		go func() {
			j := <-ch // i get value from channel
			fmt.Println(j)
			wg.Done()
		}()

		go func() {
			ch <- i // put to channel
			wg.Done()
		}()
	}
	wg.Wait()
}

func getReceive() {
	ch := make(chan int, 2) // create channel of type int, 2 is the buffer value

	wg.Add(2)

	// reader
	go func(ch <-chan int) {
		res, ok := <-ch
		fmt.Println("Got 1 : ", res, ok)

		res, ok = <-ch
		fmt.Println("Got 2 : ", res, ok)

		// res, ok = <-ch
		// fmt.Println("Got 3 : ", res, ok)

		// res, ok = <-ch
		// fmt.Println("Got 4 : ", res, ok) // Got 4 :  0 false (close signal)

		wg.Done()
	}(ch)

	// writer
	go func(ch chan<- int) {
		fmt.Println("Write 1")
		ch <- 27

		fmt.Println("Write 2")
		ch <- 12
		ch <- 1 // This one has no receiver
		wg.Done()
		close(ch)
	}(ch)
	wg.Wait()
}
