# Importing a File

To import a file, use the `incl` keyword. You can import any file with the `.ds` extension.

When specifying the file, use a string without the `.ds` extension.

For example, we can have a math file with common math functions.

math.ds
```
fn add($a, $b) {
    $a + $b
}
```

main.ds
```
incl "math"

print(add(1, 2)) // Prints 3
```

