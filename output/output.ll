; ModuleID = 'Pascal IR'
source_filename = "Pascal IR"

@b = global i32 0
@a = global i32 0
@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @PascalMain() {
entry:
  call void (ptr, ...) @scanf(ptr @0, ptr @a)
  %a = load i32, ptr @a, align 4
  call void (ptr, ...) @printf(ptr @1, i32 %a)
  ret i32 0
}

declare void @scanf(ptr, ...)

declare void @printf(ptr, ...)
