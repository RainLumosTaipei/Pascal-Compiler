# Pascal Compiler

A simple Pascal language compiler and linker for x86-64 based on llvm.

## Install and Prepare

1. llvm

    ensure you have correctly compiled llvm source, all we need is llvm, 
    not include clang or llc. To the best, you would better get the Debug version.

2. visual studio
    
    you have to configure your project settings to include the llvm headers.

## Usage and Test

same as gcc, here are the options:

```shell
Usage:
        -O0     no optimize
        -O      use optimize
        -g      generate debug info
        -W      use warning hint
        -c      <filename> compile to .o file
        -S      <filename> compile to .ll file
        -o      <filename> rename output filename
```

some examples:

```shell
pcc -c two_sum.pas
```

```shell
pcc -o two_sum.exe -c add.pas main.pas
```

currently the syntax table is outside(cause it's a little big), 
so you need to copy the lrTable.bin in the root/misc directory. 
Probably have to use LALR instead of LR :(