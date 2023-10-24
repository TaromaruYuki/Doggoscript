# Control Flow

You can change the control flow of the program using the if statement. The if statement does not have any parens surrounding it.

```
if 5 > 10 {
    print("5 is greater than 10") // This will not print
}
```

---

To add other cases, use a `else if`.

```
if 5 > 10 {
    print("5 is greater than 10") // This will not print
} else if 6 < 10 {
    print("6 is less than 10") // This will print
}
```

You can have as many `else if` statements as you need.

---

To add a case that will execute if none of the other cases are true, use a `else`.

```
if 5 > 10 {
    print("5 is greater than 10") // This will not print
} else if 6 > 10 {
    print("6 is greater than 10") // This will not print
} else {
    print("5 and 6 are not greater than 10") // This will print
}
```
