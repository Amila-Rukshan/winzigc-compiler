### Dependacies

`winzigc` project have following dependencies.

- [bazel](https://bazel.build/)
- [clang](https://releases.llvm.org/download.html)

### Build

#### Debug build

In the root of the project run bazel build command to build the winzigc binary:
```
bazel build --cxxopt='-std=c++17' //winzigc/main:cmd --strip=never -c dbg --sandbox_debug --spawn_strategy=local
```

#### Optimized build

```
bazel build --cxxopt='-std=c++17' //winzigc/main:cmd -c opt
```

### Run

To compile a winzigc program, run the following command. 
```
sh scripts/compile-and-run.sh winzig_zz
```

- It will first run the above winzigc binary agaist the sample program in `./tests/winzig_zz` and emit llvm bitcode in text format to `./tests/winzig_zz.ll`.
- Then clang backend will generate the binary for your machine architecture to `./tests/winzig_zz_binary` file.
- Then it will start executing the above binary.


### Lauch debug in vscode

Debugger configuration is setup in `.vscode/launch.json`. Just click the "(lldb) Launch Debug" option in the vscode Run and Debug view.
This will compile the winzigc in with debug options and use the `./tests/winzig_zz` program in the arguments.

### Run tests

