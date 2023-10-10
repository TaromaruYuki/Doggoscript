<img align="right" width="25%" src="res/Doggoscript-logo.png" />

# Doggoscript
<p>A programming language created in C++</p>

## Features
- [x] Variables
- [x] If statements
- [x] For loops
- [x] While loops
- [x] Functions
- [x] Lists
- [x] Strings
- [x] Arithmetic Operators
- [x] Comparison Operators
- [x] Logical Operators
- [x] Control Statements
- [x] Comments
- [X] Scopes (outside a function)
- [X] Classes
- [X] Modules
- [ ] Error Handling
- [X] Maps / Dicts
- [X] File IO

## Example

```
// This is a comment :)

incl "math"

fn join($elements, $seperator) {
    val $result is ""
    val $len is $elements->length()

    for $i in 0..$len {
        $result is $result + str($elements->at($i))

        if $i != $len - 1 {
            $result is $result + $seperator
        }
    }

    return $result
}

fn fibonacci($n) {
    val $a is 0
    val $b is 1
    val $result is []

    for $i in 0..$n {
        $result->append($a)

        val $temp is $a
        $a is $b
        $b is $temp + $b
    }

    return $result
}

val $res is "Calculating fibonacci sequence up to 100...\n\n"

print($res)

val $fib is fibonacci(100)

print(join(fibonacci(100), " "))
$res is $res + join($fib, " ")

print("10 + 20 is...")
print(add(10, 20))

val $file_str is load_file("hello.txt")
print("File 'hello.txt' content: " + $file_str)

save_file("save.txt", $res)
```

View `grammar.txt` to view the grammar
