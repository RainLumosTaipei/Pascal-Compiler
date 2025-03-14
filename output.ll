; ModuleID = 'Pascal IR'
source_filename = "Pascal IR"

@INF = constant i32 32767
@e = global [16 x [16 x i32]] zeroinitializer
@book = global [16 x i32] zeroinitializer
@dis = global [16 x i32] zeroinitializer
@n = global i32 0
@m = global i32 0
@j = global i32 0
@i = global i32 0
@v = global i32 0
@u = global i32 0

define void @Dijkstra() {
entry:
  %j = alloca i32, align 4
  store i32 0, ptr %j, align 4
  %k = alloca i32, align 4
  store i32 0, ptr %k, align 4
  %min_index = alloca i32, align 4
  store i32 0, ptr %min_index, align 4
  %min_num = alloca i32, align 4
  store i32 0, ptr %min_num, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  store i32 1, ptr %i, align 4
  br label %entry1

entry1:                                           ; preds = %then, %entry
  %n = load i32, ptr @n, align 4
  %i2 = load i32, ptr %i, align 4
  %ile = icmp sle i32 %i2, %n
  br i1 %ile, label %then, label %merge

then:                                             ; preds = %entry1
  %i3 = load i32, ptr %i, align 4
  %0 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %i3
  %i4 = load i32, ptr %i, align 4
  %1 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 1, i32 %i4
  %2 = load i32, ptr %1, align 4
  store i32 %2, ptr %0, align 4
  %i5 = load i32, ptr %i, align 4
  %3 = getelementptr [16 x i32], ptr @book, i32 0, i32 %i5
  store i32 0, ptr %3, align 4
  %i6 = load i32, ptr %i, align 4
  %inc = add i32 %i6, 1
  store i32 %inc, ptr %i, align 4
  br label %entry1

merge:                                            ; preds = %entry1
  store i32 1, ptr getelementptr ([16 x i32], ptr @book, i32 0, i32 1), align 4
  store i32 1, ptr %i, align 4
  br label %entry7

entry7:                                           ; preds = %merge32, %merge
  %n10 = load i32, ptr @n, align 4
  %isub = sub i32 %n10, 1
  %i11 = load i32, ptr %i, align 4
  %ile12 = icmp sle i32 %i11, %isub
  br i1 %ile12, label %then8, label %merge9

then8:                                            ; preds = %entry7
  %INF = load i32, ptr @INF, align 4
  store i32 %INF, ptr %min_num, align 4
  store i32 0, ptr %min_index, align 4
  store i32 1, ptr %k, align 4
  br label %entry13

merge9:                                           ; preds = %entry7
  ret void

entry13:                                          ; preds = %merge21, %then8
  %n16 = load i32, ptr @n, align 4
  %k17 = load i32, ptr %k, align 4
  %ile18 = icmp sle i32 %k17, %n16
  br i1 %ile18, label %then14, label %merge15

then14:                                           ; preds = %entry13
  br label %entry19

merge15:                                          ; preds = %entry13
  %min_index29 = load i32, ptr %min_index, align 4
  %4 = getelementptr [16 x i32], ptr @book, i32 0, i32 %min_index29
  store i32 1, ptr %4, align 4
  store i32 1, ptr %j, align 4
  br label %entry30

entry19:                                          ; preds = %then14
  %min_num22 = load i32, ptr %min_num, align 4
  %k23 = load i32, ptr %k, align 4
  %5 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %k23
  %6 = load i32, ptr %5, align 4
  %igt = icmp sgt i32 %min_num22, %6
  %k24 = load i32, ptr %k, align 4
  %7 = getelementptr [16 x i32], ptr @book, i32 0, i32 %k24
  %8 = load i32, ptr %7, align 4
  %ieq = icmp eq i32 %8, 0
  %and = and i1 %igt, %ieq
  br i1 %and, label %then20, label %else

then20:                                           ; preds = %entry19
  %k25 = load i32, ptr %k, align 4
  %9 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %k25
  %10 = load i32, ptr %9, align 4
  store i32 %10, ptr %min_num, align 4
  %k26 = load i32, ptr %k, align 4
  store i32 %k26, ptr %min_index, align 4
  br label %merge21

else:                                             ; preds = %entry19
  br label %merge21

merge21:                                          ; preds = %else, %then20
  %k27 = load i32, ptr %k, align 4
  %inc28 = add i32 %k27, 1
  store i32 %inc28, ptr %k, align 4
  br label %entry13

entry30:                                          ; preds = %merge39, %merge15
  %n33 = load i32, ptr @n, align 4
  %j34 = load i32, ptr %j, align 4
  %ile35 = icmp sle i32 %j34, %n33
  br i1 %ile35, label %then31, label %merge32

then31:                                           ; preds = %entry30
  br label %entry36

merge32:                                          ; preds = %entry30
  %i59 = load i32, ptr %i, align 4
  %inc60 = add i32 %i59, 1
  store i32 %inc60, ptr %i, align 4
  br label %entry7

entry36:                                          ; preds = %then31
  %min_index40 = load i32, ptr %min_index, align 4
  %j41 = load i32, ptr %j, align 4
  %11 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %min_index40, i32 %j41
  %12 = load i32, ptr %11, align 4
  %INF42 = load i32, ptr @INF, align 4
  %ilt = icmp slt i32 %12, %INF42
  br i1 %ilt, label %then37, label %else38

then37:                                           ; preds = %entry36
  br label %entry43

else38:                                           ; preds = %entry36
  br label %merge39

merge39:                                          ; preds = %else38, %merge46
  %j57 = load i32, ptr %j, align 4
  %inc58 = add i32 %j57, 1
  store i32 %inc58, ptr %j, align 4
  br label %entry30

entry43:                                          ; preds = %then37
  %j47 = load i32, ptr %j, align 4
  %13 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %j47
  %14 = load i32, ptr %13, align 4
  %min_index48 = load i32, ptr %min_index, align 4
  %15 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %min_index48
  %16 = load i32, ptr %15, align 4
  %min_index49 = load i32, ptr %min_index, align 4
  %j50 = load i32, ptr %j, align 4
  %17 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %min_index49, i32 %j50
  %18 = load i32, ptr %17, align 4
  %iadd = add i32 %16, %18
  %igt51 = icmp sgt i32 %14, %iadd
  br i1 %igt51, label %then44, label %else45

then44:                                           ; preds = %entry43
  %j52 = load i32, ptr %j, align 4
  %19 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %j52
  %min_index53 = load i32, ptr %min_index, align 4
  %20 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %min_index53
  %21 = load i32, ptr %20, align 4
  %min_index54 = load i32, ptr %min_index, align 4
  %j55 = load i32, ptr %j, align 4
  %22 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %min_index54, i32 %j55
  %23 = load i32, ptr %22, align 4
  %iadd56 = add i32 %21, %23
  store i32 %iadd56, ptr %19, align 4
  br label %merge46

else45:                                           ; preds = %entry43
  br label %merge46

merge46:                                          ; preds = %else45, %then44
  br label %merge39
}

define i32 @main() {
entry:
  store i32 1, ptr @i, align 4
  br label %entry1

entry1:                                           ; preds = %merge4, %entry
  %n = load i32, ptr @n, align 4
  %i = load i32, ptr @i, align 4
  %ile = icmp sle i32 %i, %n
  br i1 %ile, label %then, label %merge

then:                                             ; preds = %entry1
  store i32 1, ptr @j, align 4
  br label %entry2

merge:                                            ; preds = %entry1
  store i32 1, ptr @i, align 4
  br label %entry19

entry2:                                           ; preds = %merge9, %then
  %n5 = load i32, ptr @n, align 4
  %j = load i32, ptr @j, align 4
  %ile6 = icmp sle i32 %j, %n5
  br i1 %ile6, label %then3, label %merge4

then3:                                            ; preds = %entry2
  br label %entry7

merge4:                                           ; preds = %entry2
  %i17 = load i32, ptr @i, align 4
  %inc18 = add i32 %i17, 1
  store i32 %inc18, ptr @i, align 4
  br label %entry1

entry7:                                           ; preds = %then3
  %i10 = load i32, ptr @i, align 4
  %j11 = load i32, ptr @j, align 4
  %ieq = icmp eq i32 %i10, %j11
  br i1 %ieq, label %then8, label %else

then8:                                            ; preds = %entry7
  %i12 = load i32, ptr @i, align 4
  %j13 = load i32, ptr @j, align 4
  %0 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %i12, i32 %j13
  store i32 0, ptr %0, align 4
  br label %merge9

else:                                             ; preds = %entry7
  %i14 = load i32, ptr @i, align 4
  %j15 = load i32, ptr @j, align 4
  %1 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %i14, i32 %j15
  %INF = load i32, ptr @INF, align 4
  store i32 %INF, ptr %1, align 4
  br label %merge9

merge9:                                           ; preds = %else, %then8
  %j16 = load i32, ptr @j, align 4
  %inc = add i32 %j16, 1
  store i32 %inc, ptr @j, align 4
  br label %entry2

entry19:                                          ; preds = %then20, %merge
  %m = load i32, ptr @m, align 4
  %i22 = load i32, ptr @i, align 4
  %ile23 = icmp sle i32 %i22, %m
  br i1 %ile23, label %then20, label %merge21

then20:                                           ; preds = %entry19
  %u = load i32, ptr @u, align 4
  %v = load i32, ptr @v, align 4
  %2 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %u, i32 %v
  %i24 = load i32, ptr @i, align 4
  %inc25 = add i32 %i24, 1
  store i32 %inc25, ptr @i, align 4
  br label %entry19

merge21:                                          ; preds = %entry19
  call void @Dijkstra()
  store i32 1, ptr @i, align 4
  br label %entry26

entry26:                                          ; preds = %then27, %merge21
  %n29 = load i32, ptr @n, align 4
  %i30 = load i32, ptr @i, align 4
  %ile31 = icmp sle i32 %i30, %n29
  br i1 %ile31, label %then27, label %merge28

then27:                                           ; preds = %entry26
  %i32 = load i32, ptr @i, align 4
  %3 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %i32
  %4 = load i32, ptr %3, align 4
  %i33 = load i32, ptr @i, align 4
  %inc34 = add i32 %i33, 1
  store i32 %inc34, ptr @i, align 4
  br label %entry26

merge28:                                          ; preds = %entry26
}
