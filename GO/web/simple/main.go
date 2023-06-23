package main

import (
	"fmt"
	"net/http"
	"os"
)

func router(res http.ResponseWriter, req *http.Request) {

	fmt.Println(req.URL.Path)

	switch req.URL.Path {
	case "/about":
		var method string
		switch req.Method {
		case http.MethodGet:
			method = http.MethodGet
		case http.MethodPost:
			method = http.MethodPost
		}
		fmt.Fprintf(res, "%s", "about : "+method)
	case "/page":
		data, err := os.ReadFile("index.html")
		if err != nil {
			panic(err)
		}
		res.Header().Set("Content-Type", "text/html")
		res.Write(data)
	case "/home":
		fallthrough
	case "/":
		res.WriteHeader(200)
		res.Write([]byte("Home"))
	}
}

func main() {

	// for method 1 & 2
	http.HandleFunc("/", router)
	http.HandleFunc("/root", func(res http.ResponseWriter, req *http.Request) {
		fmt.Fprint(res, "Root")
	})

	// Method 1
	// s := &http.Server{
	// 	Addr:    ":8080",
	// 	Handler: nil,
	// }
	// s.ListenAndServe()

	// Method 2
	// http.ListenAndServe(":8080", nil) // short hand of &http.Server{}

	// Method 3
	var customHandler http.ServeMux

	customHandler.HandleFunc("/", router)

	http.ListenAndServe(":8080", &customHandler) // short hand of &http.Server{}

}
