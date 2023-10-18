#include <gtest/gtest.h>
#include <doggoscript/doggoscript.hpp>
#include <iostream>

TEST(Interpreter, Equations) {
    std::map<std::string, double> tests = {
        {"1 + 2 * 3", 7},
        {"(1 + 2) * 3", 9},
        {"3 ** 5", 243},
        {"2 * (3 - 5) ** 3 / 2 + 1", -7},
        {"-1 * 2", -2}
    };

    for (auto &test : tests) {
        std::cout << "Testing equation \"" << test.first << "\"... ";
        DoggoscriptResult result = run(test.first, nullptr);
        ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
        ASSERT_TRUE(result.result.has_value());
        ASSERT_EQ(result.result.value()->str(), std::to_string(test.second));
        std::cout << "OK!" << std::endl;
    }
}

TEST(Interpreter, Strings) {
    std::map<std::string, std::string> tests = {
        {"\"Hello, world!\"", "\"Hello, world!\""},
        {"\"\\\"Hello, world!\\\"\"", "\"\"Hello, world!\"\""},
        {"\"Hello\\nWorld!\"", "\"Hello\nWorld!\""},
        {"\"Hello\\tWorld!\"", "\"Hello\tWorld!\""},
        {"\"Hello\\rWorld!\"", "\"Hello\rWorld!\""},
        {"\"\\H\\e\\l\\l\\o\\!\"", "\"Hello!\""},
        {"\"\"", "\"\""}
    };

    for(auto &test : tests) {
        std::cout << "Testing string " << test.first << "... ";
        DoggoscriptResult result = run(test.first, nullptr);
        ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
        ASSERT_TRUE(result.result.has_value());
        ASSERT_EQ(result.result.value()->print_friendly(), test.second);
        std::cout << "OK!" << std::endl;
    }
}

TEST(Interpreter, Boolean) {
    std::cout << "Testing boolean \"false\"... ";
    DoggoscriptResult result = run("false", nullptr);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "false");
    std::cout << "OK!" << std::endl;

    std::cout << "Testing boolean \"true\"... ";
    result = run("true", nullptr);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "true");
    std::cout << "OK!" << std::endl;
}

TEST(Interpreter, Lists) {
    std::map<std::string, std::string> tests = {
        {"[]", "[]"},
        {"[1, 2, 3]", "[1.000000, 2.000000, 3.000000]"},
        {"[\"A\", \"B\"]", "[\"A\", \"B\"]"},
        {"[true, false]", "[true, false]"},
        {"[\n    1,\n    2,\n    3\n]", "[1.000000, 2.000000, 3.000000]"}
    };

    for(auto& test : tests) {
        std::cout << "Testing list " << test.first << "... ";
        DoggoscriptResult result = run(test.first, nullptr);
        ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
        ASSERT_TRUE(result.result.has_value());
        ASSERT_EQ(result.result.value()->str(), test.second);
        std::cout << "OK!" << std::endl;
    }
}

TEST(Interpreter, Dictionary) {
    std::map<std::string, std::string> tests = {
        {"{1 -> \"a\", 2 -> \"b\"}", "{1.000000 -> \"a\", 2.000000 -> \"b\"}"},
        {"{\"err\" -> {\"code\" -> 404, \"msg\" -> \"Not found\"}, \"content\" -> \"\"}", "{\"err\" -> {\"code\" -> 404.000000, \"msg\" -> \"Not found\"}, \"content\" -> \"\"}"}
    };

    for(auto& test : tests) {
        std::cout << "Testing dict " << test.first << "... ";
        DoggoscriptResult result = run(test.first, nullptr);
        ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
        ASSERT_TRUE(result.result.has_value());
        ASSERT_EQ(result.result.value()->str(), test.second);
        std::cout << "OK!" << std::endl;
    }
}

TEST(Interpreter, Variables) {
    auto *symbol_table = new SymbolTable();
    populate_symbol_table(symbol_table);

    std::cout << "Testing variable def...";
    DoggoscriptResult result = run("val $a is 5", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), std::to_string((double)5));
    std::cout << "OK!" << std::endl;

    std::cout << "Testing variable access...";
    result = run("$a", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), std::to_string((double)5));
    std::cout << "OK!" << std::endl;

    std::cout << "Testing variable re-def...";
    result = run("$a is 10", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), std::to_string((double)10));
    std::cout << "OK!" << std::endl;
}

TEST(Interpreter, IfStatement) {
    std::map<std::string, double> tests {
        {"if 5 = 5 { 1 }", 1},
        {"if 1 = 3 { 1 } else { 10 }", 10},
        {"if 1 > 5 { 1 } else if 1 < -1 { 2 } else if 1 = 1 { 3 } else { 4 }", 3},
        {"if !true { 1 } else { 2 }", 2},
        {"if 5 <= 5 { 1 }", 1},
        {"if 8 >= 5 { 1 }", 1},
    };

    for(auto& test : tests) {
        std::cout << "Testing if statement \"" << test.first << "\"... ";
        DoggoscriptResult result = run(test.first, nullptr);
        ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
        ASSERT_TRUE(result.result.has_value());
        ASSERT_EQ(result.result.value()->str(), std::to_string(test.second));
        std::cout << "OK!" << std::endl;
    }
}

TEST(Interpreter, ForStatement) {
    auto *symbol_table = new SymbolTable();
    populate_symbol_table(symbol_table);

    std::cout << "Testing for statement... ";
    DoggoscriptResult result = run("val $res is 0; for $i in 0..50 { $res is $res + $i }", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    result = run("$res", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "1225.000000");
    std::cout << "OK!" << std::endl;

    std::cout << "Testing stepped for statement... ";
    result = run("val $res is 0; for $i in 0..50..2 { $res is $res + $i }", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    result = run("$res", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "600.000000");
    std::cout << "OK!" << std::endl;

    std::cout << "Testing negative stepped for statement... ";
    result = run("val $res is 0; for $i in 10..0..-1 { $res is $res + $i }", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    result = run("$res", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "55.000000");
    std::cout << "OK!" << std::endl;
}

TEST(Interpreter, WhileStatement) {
    auto *symbol_table = new SymbolTable();
    populate_symbol_table(symbol_table);

    std::cout << "Testing while statement... ";
    DoggoscriptResult result = run("val $res is 0; while $res != 100 { $res is $res + 1 }", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    result = run("$res", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "100.000000");
    std::cout << "OK!" << std::endl;

    std::cout << "Testing false while statement... ";
    result = run("val $res is 0; while $res = 100 { $res is $res + 1 }", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    result = run("$res", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "0.000000");
    std::cout << "OK!" << std::endl;
}

TEST(Interpreter, Functions) {
    auto *symbol_table = new SymbolTable();
    populate_symbol_table(symbol_table);

    std::cout << "Testing function def 'add'... ";
    DoggoscriptResult result = run("fn add($a, $b) { $a + $b }", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "<function add>");
    std::cout << "OK!" << std::endl;

    std::cout << "Testing function run 'add' with params '5', '10'... ";
    result = run("add(5, 10)", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "15.000000");
    std::cout << "OK!" << std::endl;

    std::cout << "Testing multiline function def 'eq'... ";
    result = run("fn eq($a, $b, $c) {\nval $c is $a * $b + $c\nval $a is $a * 2\nreturn $a + $b + $c\n}", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "<function eq>");
    std::cout << "OK!" << std::endl;

    std::cout << "Testing function run 'eq' with params '2', '6', '10'... ";
    result = run("eq(2, 6, 10)", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "32.000000");
    std::cout << "OK!" << std::endl;
}

TEST(Interpreter, Classes) {
    auto *symbol_table = new SymbolTable();
    populate_symbol_table(symbol_table);

    std::cout << "Testing class def 'Person'... ";
    DoggoscriptResult result = run("class Person {\n\tfn __constructor($name, $age) {\n\t\tval $this->$name is $name\n\t\tval $this->$age is $age\n\t}\n\t\n\tfn greeting() {\n\t\tprint(\"Hello! My name is \" + $this->$name + \"! I am \" + str($this->$age) + \" years old!\")\n\t}\n\t\n\tfn birthday() {\n\t\t$this->$age is $this->$age + 1;\n\t\tprint($this->$name + \" is now \" + str($this->$age) + \" years old!\")\n\t}\n}", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    ASSERT_EQ(result.result.value()->str(), "<class Person>");
    std::cout << "OK!" << std::endl;

    std::cout << "Testing class instance creation 'Person'... ";
    result = run("val $person is Person(\"John Doe\", 46)", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    std::cout << "OK!" << std::endl;

    std::cout << "Testing class method 'greeting' on class 'Person'" << std::endl;
    result = run("$person->greeting()", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    std::cout << "OK!" << std::endl;

    std::cout << "Testing class method 'birthday' on class 'Person'" << std::endl;
    result = run("$person->birthday()", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    std::cout << "OK!" << std::endl;

    std::cout << "Testing class method 'greeting' on class 'Person'" << std::endl;
    result = run("$person->greeting()", symbol_table);
    ASSERT_FALSE(result.error.has_value()) << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.result.has_value());
    std::cout << "OK!" << std::endl;
}