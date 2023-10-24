# Functions

Functions are a important part of any programming language. They allow you to reuse code and make your code more readable. Functions are defined using the `fn` keyword.

```
fn function_name() {
    // Code goes here
}
```

Functions can also take arguments. Arguments are defined in the parentheses after the function name. Arguments are separated by commas.

Since arguments are variables, you still need to require the `$` symbol.

```
fn function_name($argument1, $argument2) {
    // Code goes here
}
```

Depending on how many lines you have, you may or may not need to use the `return` keyword. If you have a single line, you can omit the `return` keyword. If you have multiple lines, you need to use the `return` keyword.

```
fn add($a, $b) {
    $a + $b
}

// vs

fn add($a, $b) {
    $a is $a + $b
    return $a
}
```