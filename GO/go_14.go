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

var wg = sync.WaitGroup{}

func main() {
	// channel()
	channelLoop()
}

func channel() {
	ch := make(chan int) // create channel of typ eint
	wg.Add(2)
	go func() {
		i := <-ch // i get value from channel
		fmt.Println(i)
		wg.Done()
	}()

	go func() {
		i := 27
		ch <- i // put to channel
		i = 12  // this will not pass
		wg.Done()
	}()
	wg.Wait()
}

func channelLoop() {
	ch := make(chan int) // create channel of typ eint
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
