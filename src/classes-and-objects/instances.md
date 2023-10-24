# Instances

Instances are objects that are created from a class. They are created by calling the class name as if it were a function.

Let's take our person class as a example.

```
class Person {
    fn __constructor($name, $age) {
        val $this->$name is $name;
        val $this->$age is $age;
    }
    
    fn __string() {
        "Person: " + $this->$name + " (" + str($this->$age) + ")";
    }
    
    fn hello() {
        print("Hello! My name is " + $this->$name + " and I am " + str($this->$age) + " years old.");
    }
    
    fn birthday() {
        val $this->$age is $this->$age + 1;
        print("Happy birthday, " + $this->$name + "!");
    }
}
```

To create an instance of this class, we would do the following:

```
val $person is Person("John Doe", 20);
```

This creates a new instance of the `Person` class and stores it in the variable `$person`.

We can then call the methods of the class on the instance:

```
$person->hello()    // Prints "Hello! My name is John Doe and I am 20 years old."
$person->birthday() // Prints "Happy birthday, John Doe!"
$person->hello()    // Prints "Hello! My name is John Doe and I am 21 years old."
```

When printing the class, it wil print the string returned by the `__string` method:

```
print($person) // Prints "Person: John Doe (21)"
```

If there is no __string method, it will have a default string representation:

```
print($person) // Prints <Person at address 0x0000000000000000>
```

You can also access variables

```
print($person->$name) // Prints "John Doe"
```