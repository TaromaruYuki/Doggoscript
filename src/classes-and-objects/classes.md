# Classes

Classes are a way to group data and functions together. They are a blueprint for creating objects. Classes are defined using the `class` keyword.

```
class MyClass {}
```

## Defining functions

Functions can be defined inside of classes. They are called methods. They are defined [the same way](../basic-syntax/functions.md) anywhere else in code.

```
class MyClass {
    fn mymethod() {
        print("Foo")
    }
}
```

## Defining variables

To define a variable, follow the [variables](../basic-syntax/variables.md) section.
You then want to use the special $this variable with a `->` between the names.

```
class MyClass {
    fn mymethod($a) {
        val $this->$a is $a;
    }
}
```

## Special methods

Classes can have special methods that are called when certain things happen. These methods are identified by using two underscores (`__`) before the method name.

### `__constructor`

The `__constructor` method is called when an object is created. It is used to set up the object.

You typically define variables in the `__constructor` method. 

Example:

```
class MyClass {
    fn __constructor($a) {
        val $this->$a is $a;
        val $this->$b is 0;
    }
}
```

### `__string`

The `__string` method is called when the object is converted to a string. It is used to return a string representation of the object.

Example:

```
class MyClass {
    fn __string() {
        "MyClass"
    }
}
```

## Example class

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