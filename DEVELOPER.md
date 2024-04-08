### Dependencies

`WinZigC` project has following dependencies:

- [bazel](https://bazel.build/)
- [clang](https://releases.llvm.org/download.html)

### Build

#### Debug build

In the root of the project, run the Bazel build command to build the WinZigC binary:

```
bazel build --cxxopt='-std=c++17' //winzigc/main:cmd --strip=never -c dbg --sandbox_debug --spawn_strategy=local
```

#### Optimized build

```
bazel build --cxxopt='-std=c++17' //winzigc/main:cmd -c opt
```

### Compile and run a WinZigC program

To compile and run a WinZigC program (in the `example-programs` directory), run the following command.

```
sh scripts/compile-and-run.sh <program_name>
```

```
sh scripts/compile-and-run.sh winzig_zz
```

- It will first run the above WinZigC binary for the sample program in `./example-programs/winzig_zz` and emit LLVM bitcode in text format to `./example-programs/winzig_zz.ll`.
- Then Clang backend will generate the binary for your machine architecture into `./example-programs/winzig_zz_binary` file.
- Then it will start executing the above binary.

### How to debug a WinZigC program

Since `WinZigC` emits debug infomation and it is also a `C-family` language, WinZigC programms can be debugged using a LLBD debugger. To let the VSCode editor to place breakepoints, you have to rename your WinZigC program with `.c` extension for now. For example, rename the `winzig_zz` file as `winzig_zz.c`.

1. Comiple with the debug flag (`-dbg`). (Make sure to comment out the last line in `compile-and-run.sh` to prevent removal of the generated WinZigC binary)

   ```
   sh scripts/compile-and-run.sh winzig_zz.c -dbg
   ```

2. Launch "WinZigC Program Debug" from the debug options list in the VSCode debug view. (Make sure to update the binary path accordingly)

### Lauch debugger in vscode

Debugger configuration is set up in `.vscode/launch.json`. Just click the "(lldb) Launch Debug" option in the vscode Run and Debug view.
This will compile the WinZigC in with debug options and use the `./example-programs/winzig_zz` program in the arguments.

### Run tests

#### Lexer tests

```
bazel test --cxxopt=-std=c++17 --test_output=all //test/frontend/lexer:lexer_test
```

#### Parser tests

```
bazel test --cxxopt=-std=c++17 --test_output=all //test/frontend/parser:parser_test
```

#### Semantic tests

```
bazel test --cxxopt=-std=c++17 --test_output=all //test/visitor/semantic:semantic_test
```

### Integration tests

Launch "Debug Integration Tests" from the debug options list in vscode debug view.
