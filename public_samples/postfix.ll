; ModuleID = 'samples/postfix.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@a = global i32 0

define i32 @postfix(i32 %b) {
entry:
  %b1 = alloca i32
  store i32 %b, i32* %b1
  br label %next

next:                                             ; preds = %entry
  %i = alloca i32
  %b2 = load i32* %b1
  %0 = sub i32 %b2, 1
  store i32 %0, i32* %b1
  store i32 %0, i32* %i
  %a = load i32* @a
  %1 = add i32 %a, 1
  store i32 %1, i32* @a
  %i3 = load i32* %i
  %2 = add i32 %i3, %1
  store i32 %2, i32* %i
  %b4 = load i32* %b1
  %a5 = load i32* @a
  %3 = add i32 %a5, %b4
  %4 = mul i32 2, %3
  %i6 = load i32* %i
  %5 = add i32 %i6, %4
  ret i32 %5
}
