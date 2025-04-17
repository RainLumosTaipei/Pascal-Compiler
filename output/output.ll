; ModuleID = 'Pascal Compiler'
source_filename = "output/output.exe"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc"

@c = global i32 0
@b = global i32 0
@a = global i32 0

define i32 @main_main() {
entry:
  store i32 1, ptr @a, align 4
  store i32 2, ptr @b, align 4
  %a = load i32, ptr @a, align 4
  %b = load i32, ptr @b, align 4
  %0 = call i32 @add(i32 %a, i32 %b)
  store i32 %0, ptr @c, align 4
  ret i32 0
}

declare void @scanf(ptr, ...)

declare void @printf(ptr, ...)

define i32 @add_main() {
entry:
  ret i32 0
}

define i32 @add(i32 %b, i32 %a) {
entry:
  %iadd = add i32 %a, %b
  ret i32 %iadd
}
