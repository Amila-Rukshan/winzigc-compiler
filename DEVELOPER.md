### Dependacies

`winzigc` project have following dependencies.

- [bazel](https://bazel.build/)
- [clang](https://releases.llvm.org/download.html)

### Build

#### Debug build

In the root of the project, run the Bazel build command to build the Winzigc binary:
```
bazel build --cxxopt='-std=c++17' //winzigc/main:cmd --strip=never -c dbg --sandbox_debug --spawn_strategy=local
```

#### Optimized build

```
bazel build --cxxopt='-std=c++17' //winzigc/main:cmd -c opt
```

### Compile and run a winzigc program

To compile and run a Winzigc program (in the `example-programs` directory), run the following command.
```
sh scripts/compile-and-run.sh <program_name>
```
```
sh scripts/compile-and-run.sh winzig_zz
```

- It will first run the above Winzigc binary for the sample program in `./example-programs/winzig_zz` and emit LLVM bitcode in text format to `./example-programs/winzig_zz.ll`.
- Then Clang backend will generate the binary for your machine architecture into `./example-programs/winzig_zz_binary` file.
- Then it will start executing the above binary.


### Lauch debugger in vscode

Debugger configuration is set up in `.vscode/launch.json`. Just click the "(lldb) Launch Debug" option in the vscode Run and Debug view.
This will compile the winzigc in with debug options and use the `./example-programs/winzig_zz` program in the arguments.

### Run tests

#### Lexer tests

```
bazel test --cxxopt=-std=c++17 --test_output=all //test/frontend/lexer:lexer_test
```

#### Parser tests

```
bazel test --cxxopt=-std=c++17 --test_output=all //test/frontend/parser:parser_test
```

### Integration tests

Launch "Debug Integration Tests" from the debug options list in vscode debug view.
