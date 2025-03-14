; ModuleID = 'Pascal IR'
source_filename = "Pascal IR"

@N = global i32 0
@L = global i32 0
@M = global i32 0
@c2 = global [3 x i32] zeroinitializer
@c1 = global [3 x i32] zeroinitializer
@c0 = global [3 x i32] zeroinitializer
@b2 = global [3 x i32] zeroinitializer
@b1 = global [3 x i32] zeroinitializer
@b0 = global [3 x i32] zeroinitializer
@a2 = global [3 x i32] zeroinitializer
@a1 = global [3 x i32] zeroinitializer
@a0 = global [3 x i32] zeroinitializer
@x = global i32 0
@i = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @tran() {
entry:
  %i = alloca i32, align 4
  %M.pre = load i32, ptr @M, align 4
  br label %entry1

entry1:                                           ; preds = %then, %entry
  %i3 = phi i32 [ 0, %entry ], [ %inc, %then ]
  store i32 %i3, ptr %i, align 4
  %isub = add i32 %M.pre, -1
  %ile.not = icmp sgt i32 %i3, %isub
  br i1 %ile.not, label %merge, label %then

then:                                             ; preds = %entry1
  %0 = load i32, ptr getelementptr inbounds nuw (i8, ptr @a2, i64 4), align 4
  store i32 %0, ptr getelementptr inbounds nuw (i8, ptr @c1, i64 8), align 4
  %1 = load i32, ptr getelementptr inbounds nuw (i8, ptr @a1, i64 8), align 4
  store i32 %1, ptr getelementptr inbounds nuw (i8, ptr @c2, i64 4), align 4
  %a1 = load i32, ptr @a1, align 4
  store i32 %a1, ptr getelementptr inbounds nuw (i8, ptr @c0, i64 4), align 4
  %a2 = load i32, ptr @a2, align 4
  store i32 %a2, ptr getelementptr inbounds nuw (i8, ptr @c0, i64 8), align 4
  %2 = load i32, ptr getelementptr inbounds nuw (i8, ptr @a0, i64 4), align 4
  store i32 %2, ptr @c1, align 4
  %3 = load i32, ptr getelementptr inbounds nuw (i8, ptr @a0, i64 8), align 4
  store i32 %3, ptr @c2, align 4
  %4 = load i32, ptr getelementptr inbounds nuw (i8, ptr @a1, i64 4), align 4
  store i32 %4, ptr getelementptr inbounds nuw (i8, ptr @c1, i64 4), align 4
  %5 = load i32, ptr getelementptr inbounds nuw (i8, ptr @a2, i64 8), align 4
  store i32 %5, ptr getelementptr inbounds nuw (i8, ptr @c2, i64 8), align 4
  %a0 = load i32, ptr @a0, align 4
  store i32 %a0, ptr @c0, align 4
  %inc = add i32 %i3, 1
  br label %entry1

merge:                                            ; preds = %entry1
  ret i32 0
}

define i32 @main() {
entry:
  store i32 3, ptr @N, align 4
  store i32 3, ptr @M, align 4
  store i32 3, ptr @L, align 4
  store i32 0, ptr @i, align 4
  br label %entry1

entry1:                                           ; preds = %then, %entry
  %M = load i32, ptr @M, align 4
  %isub = sub i32 %M, 1
  %i = load i32, ptr @i, align 4
  %ile = icmp sle i32 %i, %isub
  br i1 %ile, label %then, label %merge

then:                                             ; preds = %entry1
  %i2 = load i32, ptr @i, align 4
  %0 = getelementptr [3 x i32], ptr @a0, i32 0, i32 %i2
  %i3 = load i32, ptr @i, align 4
  store i32 %i3, ptr %0, align 4
  %i4 = load i32, ptr @i, align 4
  %1 = getelementptr [3 x i32], ptr @a1, i32 0, i32 %i4
  %i5 = load i32, ptr @i, align 4
  store i32 %i5, ptr %1, align 4
  %i6 = load i32, ptr @i, align 4
  %2 = getelementptr [3 x i32], ptr @a2, i32 0, i32 %i6
  %i7 = load i32, ptr @i, align 4
  store i32 %i7, ptr %2, align 4
  %i8 = load i32, ptr @i, align 4
  %3 = getelementptr [3 x i32], ptr @b0, i32 0, i32 %i8
  %i9 = load i32, ptr @i, align 4
  store i32 %i9, ptr %3, align 4
  %i10 = load i32, ptr @i, align 4
  %4 = getelementptr [3 x i32], ptr @b1, i32 0, i32 %i10
  %i11 = load i32, ptr @i, align 4
  store i32 %i11, ptr %4, align 4
  %i12 = load i32, ptr @i, align 4
  %5 = getelementptr [3 x i32], ptr @b2, i32 0, i32 %i12
  %i13 = load i32, ptr @i, align 4
  store i32 %i13, ptr %5, align 4
  %i14 = load i32, ptr @i, align 4
  %inc = add i32 %i14, 1
  store i32 %inc, ptr @i, align 4
  br label %entry1

merge:                                            ; preds = %entry1
  %6 = call i32 @tran()
  store i32 0, ptr @i, align 4
  br label %entry15

entry15:                                          ; preds = %then16, %merge
  %N = load i32, ptr @N, align 4
  %isub18 = sub i32 %N, 1
  %i19 = load i32, ptr @i, align 4
  %ile20 = icmp sle i32 %i19, %isub18
  br i1 %ile20, label %then16, label %merge17

then16:                                           ; preds = %entry15
  %i21 = load i32, ptr @i, align 4
  %7 = getelementptr [3 x i32], ptr @c0, i32 0, i32 %i21
  %8 = load i32, ptr %7, align 4
  call void (ptr, ...) @printf(ptr @0, i32 %8)
  %i22 = load i32, ptr @i, align 4
  %inc23 = add i32 %i22, 1
  store i32 %inc23, ptr @i, align 4
  br label %entry15

merge17:                                          ; preds = %entry15
  store i32 0, ptr @i, align 4
  br label %entry24

entry24:                                          ; preds = %then25, %merge17
  %N27 = load i32, ptr @N, align 4
  %isub28 = sub i32 %N27, 1
  %i29 = load i32, ptr @i, align 4
  %ile30 = icmp sle i32 %i29, %isub28
  br i1 %ile30, label %then25, label %merge26

then25:                                           ; preds = %entry24
  %i31 = load i32, ptr @i, align 4
  %9 = getelementptr [3 x i32], ptr @c1, i32 0, i32 %i31
  %10 = load i32, ptr %9, align 4
  call void (ptr, ...) @printf(ptr @1, i32 %10)
  %i32 = load i32, ptr @i, align 4
  %inc33 = add i32 %i32, 1
  store i32 %inc33, ptr @i, align 4
  br label %entry24

merge26:                                          ; preds = %entry24
  store i32 0, ptr @i, align 4
  br label %entry34

entry34:                                          ; preds = %then35, %merge26
  %N37 = load i32, ptr @N, align 4
  %isub38 = sub i32 %N37, 1
  %i39 = load i32, ptr @i, align 4
  %ile40 = icmp sle i32 %i39, %isub38
  br i1 %ile40, label %then35, label %merge36

then35:                                           ; preds = %entry34
  %i41 = load i32, ptr @i, align 4
  %11 = getelementptr [3 x i32], ptr @c2, i32 0, i32 %i41
  %12 = load i32, ptr %11, align 4
  call void (ptr, ...) @printf(ptr @2, i32 %12)
  %i42 = load i32, ptr @i, align 4
  %inc43 = add i32 %i42, 1
  store i32 %inc43, ptr @i, align 4
  br label %entry34

merge36:                                          ; preds = %entry34
  ret i32 0
}

declare void @scanf(ptr, ...)

declare void @printf(ptr, ...)
