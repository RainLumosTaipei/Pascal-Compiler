; ModuleID = 'Pascal IR'
source_filename = "Pascal IR"

@c = global i32 0
@b = global i32 0
@a = global i32 0
@vecA = global [10 x [10 x i32]] zeroinitializer

define i32 @myFunc(i32 %q, i32 %p, float %s, float %r) {
entry:
  %q1 = alloca i32, align 4
  store i32 0, ptr %q1, align 4
  %p2 = alloca i32, align 4
  store i32 0, ptr %p2, align 4
  %s3 = alloca float, align 4
  store float 0.000000e+00, ptr %s3, align 4
  %r4 = alloca float, align 4
  store float 0.000000e+00, ptr %r4, align 4
  %lb = alloca i32, align 4
  store i32 0, ptr %lb, align 4
  %la = alloca i32, align 4
  store i32 0, ptr %la, align 4
  store i32 1, ptr %p2, align 4
  %p25 = load i32, ptr %p2, align 4
  %la6 = load i32, ptr %la, align 4
  %iadd = add i32 %p25, %la6
  store i32 %iadd, ptr %q1, align 4
  %p27 = load i32, ptr %p2, align 4
  %q18 = load i32, ptr %q1, align 4
  %iadd9 = add i32 %p27, %q18
  %0 = load i32, ptr getelementptr ([10 x [10 x i32]], ptr @vecA, i32 0, i32 6, i32 8), align 4
  %imul = mul i32 %iadd9, %0
  store i32 %imul, ptr @b, align 4
  %b = load i32, ptr @b, align 4
  ret i32 %b
}

define i32 @main() {
entry:
  store i32 3, ptr @a, align 4
}

define void @myProc() {
entry:
  %0 = call i32 @myFunc(i32 10, i32 100, float 1.000000e+00, float 2.000000e+00)
  store i32 %0, ptr @c, align 4
  ret void
}
