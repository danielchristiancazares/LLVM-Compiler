; ModuleID = 'samples/assign.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@x = global i32 0

define i32 @main(i32 %f) {
entry:
  %f1 = alloca i32
  store i32 %f, i32* %f1
  br label %next

next:                                             ; preds = %entry
  %y = alloca i32
  %z = alloca i32
  %f2 = load i32* %f1
  %x = load i32* @x
  %0 = add i32 %x, %f2
  store i32 %0, i32* %z
  store i32 %0, i32* %y
  %z3 = load i32* %z
  %y4 = load i32* %y
  %1 = add i32 %y4, %z3
  ret i32 %1
}
