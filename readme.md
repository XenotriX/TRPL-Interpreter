# The Reactive Programming Language

> This is the interpreter for TRPL

> The specification for the language itself can be found [here](https://github.com/XenotriX1337/TRPL/blob/draft/specification.md)

The Reactive Programming Language or TRPL is a language developed as part of a school project.

What makes this language different from others, is that when assigning a term to a variable, the term is not evaluated before being assigned but instead the variable stores the term itself.

So, for example if you have this code:

``` js
var x = 5
var y = x + 2
x = 10
print y
```

most languages would output `7` whereas in TRPL this produces the output `12`.

Now, you might be wondering if this can be useful or not and to be honest I have no clue.
So, you shouldn't think of TRPL as the main programming language for your next project but as an experiment to try to think differently.

## Getting Started

To use the interpreter, you will need to build it yourself.

### Linux

1. Clone this repository
2. Install **GNU/Bison** and **flex**
3. Create a `build` directory
4. Inside your `build` directory run `cmake ..`
5. Once the cmake configuartion is done, compile with `make`

### Windows

I have not yet tried to compile this on windows but since there is no platform specific code, the cmake file should work.

> If you are a windows user and built this project, I would appreciat it a lot if you could help me writing this guide.

## Contributing

All contributions and suggestions are welcome.

For suggestions and bugs, please file an issue.

For direct contributions, please file a pull request.

> If you'd like to contribute to the language itself, I invite you to do so on the [Specification](https://github.com/xenotrix1337/TRPL) repository.

## Author

Tibo Clausen
tibo.clausen@gmail.com

## License

This project is licensed under the MIT License. For more information see the [LICENSE](https://github.com/XenotriX1337/TRPL-Interpreter/blob/master/LICENSE) file.

