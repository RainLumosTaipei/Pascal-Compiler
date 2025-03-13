; ModuleID = 'Pascal IR'
source_filename = "Pascal IR"

define i32 @ififElse() {
entry:
  %a = alloca i32, align 4
  store i32 0, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 0, ptr %b, align 4
  store i32 66, ptr %a, align 4
  store i32 10, ptr %b, align 4
  
  store i32 0, ptr %a, align 4
  br label %entry1

entry1:                                           ; preds = %then, %entry
  %a2 = load i32, ptr %a, align 4
  %ile = icmp sle i32 %a2, 4
  br i1 %ile, label %then, label %merge

then:                                             ; preds = %entry1
  %b3 = load i32, ptr %b, align 4
  %imul = mul i32 %b3, 2
  store i32 %imul, ptr %b, align 4
  
  %a4 = load i32, ptr %a, align 4
  %inc = add i32 %a4, 1
  store i32 %inc, ptr %a, align 4
  br label %entry1

merge:                                            ; preds = %entry1
  %b5 = load i32, ptr %b, align 4
  ret i32 %b5
}

define i32 @main() {
entry:
  %0 = call i32 @ififElse()
}
