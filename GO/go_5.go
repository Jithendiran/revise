/**
* @author: E.K.Jithendiran
* @date: 19.6.2023
 */
package main

import (
	"fmt"
	"reflect"
)

type employee struct {
	id      int
	age     int
	name    string
	address [3]string
	// id string // Err: redeclare
}

const (
	land = iota + 1
	water
	air
)

// Animal is exposed to outer package
type Animal struct {
	Name       string
	AnimalType int
}

// embedding struct
// it is not inheritance
type Dog struct {
	Animal       // includes animal type
	RunningSpeed int
	JawStrength  float32
}

func main() {

	var employee1 employee
	employee1 = employee{
		id:   001,
		name: "Abc",
		address: [3]string{
			"District",
			"State",
			"Country",
		},
		// id: 003, Err: duplicate
	}

	fmt.Println(employee1)      // {1 0 Abc [District State Country]}
	fmt.Println(employee1.name) // Abc
	employee1.name = "cba"
	fmt.Println(employee1.name) // cba

	employee2 := employee{
		27,
		002,
		"Abcd",
		[3]string{
			"District",
			"State",
			"Country",
		},
	}

	fmt.Println(employee2) // {27 2 Abcd [District State Country]}

	// employee3 := employee{
	// 	002,
	// 	"Abcd",
	// } // Err :  too few values in struct literal of type employee

	// anonyms struct
	nameStruct := struct{ name string }{name: "def"}
	fmt.Println(nameStruct) // {def}

	// struct are not reference type
	employee3 := employee2
	employee3.name = "hij"

	fmt.Println(employee2.name) // Abcd
	fmt.Println(employee3.name) // hij

	// force to reference typr
	employee4 := &employee2
	employee4.name = "zyx"
	fmt.Println(employee2.name) // zyx
	fmt.Println(employee4.name) // zyx

	dogAnimal := Dog{}
	dogAnimal.Name = "Tom"
	dogAnimal.AnimalType = land
	dogAnimal.JawStrength = 2.231
	dogAnimal.RunningSpeed = 25

	fmt.Println(dogAnimal)        // {{Tom 1} 25 2.231}
	fmt.Println(dogAnimal.Animal) // {Tom 1}

	// jamesDog := Dog{
	// 	Name:         "James",
	// 	AnimalType:   land,
	// 	JawStrength:  2.5,
	// 	RunningSpeed: 20,
	// }
	// Err: unknown field Name in struct literal of type Dog,

	jamesDog := Dog{
		Animal:       Animal{Name: "James", AnimalType: land},
		JawStrength:  2.5,
		RunningSpeed: 20,
	}
	fmt.Println(jamesDog) //{{James 1} 20 2.5}

	// tags
	// tags is used to describe the fields
	type Employee struct {
		Name  string `required "max:10"`
		email string
	}

	fmt.Println(Employee{Name: "aa"})            //{aa }
	fmt.Println(Employee{email: "aa@gmail.com"}) //{aa@gmail.com}

	tag := reflect.TypeOf(Employee{})
	field, ok := tag.FieldByName("Name")
	fmt.Println(field.Tag, ok) // required "max:10" true

	// manually test the validation GO is not support inbuilt validation
}
