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
- [ ] Classes
- [X] Modules
- [ ] Error Handling
- [X] Maps / Dicts
- [ ] File IO
- [ ] List Indexing
- [ ] Assignment Operators

## Example

```
// This is a comment :)

fn join($elements, $seperator) {
    val $result is ""
    val $len is len($elements)

    for $i in 0..$len {
        val $obj is str(get($elements, $i))
        $result is $result + $obj

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
        append($result, $a)

        val $temp is $a
        $a is $b
        $b is $temp + $b
    }

    return $result
}

print("Calculating fibonacci sequence up to 100...")

val $fib is fibonacci(100)

print(join($fib, " "))
```

View `grammar.txt` to view the grammar
