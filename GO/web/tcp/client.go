/**
* @author: E.K.Jithendiran
* @date: 27.6.2023
 */
package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"sync"
)

type ResponseClient struct {
	ResponseType int8   `json:"responseType"` // 1 -> server msg, 2-> broad cast, 3-> user msg
	FromUser     string `json:"fromUser"`
	Data         string `json:"data"`
}

var wg = &sync.WaitGroup{}

func listner(conn *net.Conn) {
	var buf []byte
	(*conn).Write([]byte("listening..."))
	for {
		fmt.Println("Listening...")
		n, err := bufio.NewReader(*conn).ReadByte()
		fmt.Println("Err : ", err)
		if err == io.EOF {
			fmt.Println("Err - EOF")
			break
		}
		buf = append(buf, n)
	}
	fmt.Println("Message : ", string(buf))
	wg.Done()
}

func main() {
	conn, err := net.Dial("tcp", "localhost:8080")
	defer conn.Close()
	defer fmt.Println("Connection ended")
	if err != nil {
		// handle error
		fmt.Println("Error in connection")
	}
	conn.Write([]byte("Hi"))
	wg.Add(1)
	go listner(&conn)
	wg.Wait()
}
