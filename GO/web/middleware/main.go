/**
* @author: E.K.Jithendiran
* @date: 24.6.2023
 */
package main

import (
	"fmt"
	"net/http"
)

// basic handler start
func middleware1(handlerFunc http.HandlerFunc) http.HandlerFunc {

	return func(res http.ResponseWriter, req *http.Request) {
		fmt.Println("Middle ware 1 start")
		handlerFunc.ServeHTTP(res, req)
		fmt.Println("Middle ware 1 end")
	}
}

func middleware2(handlerFunc http.HandlerFunc) http.HandlerFunc {

	return func(res http.ResponseWriter, req *http.Request) {
		fmt.Println("Middle ware 2 start")
		handlerFunc.ServeHTTP(res, req)
		fmt.Println("Middle ware 2 end")
	}
}

// basic handler end

// advanced handler start
// ref:https://dev.to/karankumarshreds/middlewares-in-go-41j
type Middleware func(http.HandlerFunc) http.HandlerFunc
type Chain []Middleware

func CreateChain(middlewares ...Middleware) Chain {
	var slice Chain
	return append(slice, middlewares...)
}

func (c Chain) Then(originalHandler http.HandlerFunc) http.HandlerFunc {
	for i := range c {
		// Same as to m1(m2(m3(originalHandler)))
		// len(c) -1 -i this is for reverse
		originalHandler = c[len(c)-1-i](originalHandler)
	}
	return originalHandler
}

// advanced handler end

func main() {
	var funRoot = func(res http.ResponseWriter, req *http.Request) {
		fmt.Println("home")
		fmt.Fprint(res, "Home")
	}
	http.HandleFunc("/base", middleware1(middleware2(funRoot))) // It will basic middle ware
	// It can cause middleware chaining problem
	// eg: m1(m2(m3(handler)))

	http.HandleFunc("/advanced", CreateChain(middleware1, middleware2).Then(funRoot)) // It will basic middle ware
	http.ListenAndServe(":8080", nil)
}
