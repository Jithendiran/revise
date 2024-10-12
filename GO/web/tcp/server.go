/**
* @author: E.K.Jithendiran
* @date: 27.6.2023
 */
package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"net"
	"time"
)

const (
	serverType = iota + 1
	broadCastType
	userTypeType
)

type (
	ResponseServer struct {
		ResponseType int8   `json:"responseType"` // 1 -> server msg, 2-> broad cast, 3-> user msg
		FromUser     string `json:"fromUser"`
		Data         string `json:"data"`
	}
	client struct {
		conn net.Conn
	}
)

var (
	clients []client
)

func (conn client) connWrite(responseType int8, data, user string) {
	res := ResponseServer{
		FromUser:     user,
		Data:         data,
		ResponseType: responseType,
	}
	value, err := json.Marshal(res)

	if err != nil {
		fmt.Println("Error in json conversion : ", err)
	}
	conn.conn.Write(value)
	// json.NewEncoder(conn.conn).Encode(res)
}

func conRead(conn net.Conn) {
	for {
		message, _ := bufio.NewReader(conn).ReadString('\n')
		fmt.Print("Message from clent: " + message)
	}
}

func handleConnection(clientConn client) {
	clients = append(clients, clientConn)
	fmt.Println("Connection added : ", clientConn.conn.RemoteAddr())
	clientConn.connWrite(serverType, "Connected to server ", clientConn.conn.RemoteAddr().String())
	fmt.Println("msg sent")
	// clientConn.conn.Write([]byte("HI"))
}

func main() {

	ln, err := net.Listen("tcp", "localhost:8080")
	defer fmt.Println("Connection closed")
	defer ln.Close()

	if err != nil {
		// handle error
		fmt.Println("%v Error in startup", time.Now())
		panic(err)
	}
	fmt.Println("Tcp Scoket listening : ", ln.Addr())

	for {
		conn, err := ln.Accept()
		// above line wait untill new connection accept
		if err != nil {
			fmt.Println("%v Error in connection", time.Now())
			continue
		}
		go handleConnection(client{conn: conn})
		go conRead(conn)
	}
}
