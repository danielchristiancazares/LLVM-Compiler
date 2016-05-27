; ModuleID = 'samples/while_loop.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @whiletest(i32 %b) {
entry:
  %b1 = alloca i32
  store i32 %b, i32* %b1
  br label %next

next:                                             ; preds = %entry
  %i = alloca i32
  %sum = alloca i32
  store i32 0, i32* %i
  store i32 0, i32* %sum
  br label %header

footer:                                           ; preds = %header
  %sum7 = load i32* %sum
  ret i32 %sum7

bodyBB:                                           ; preds = %header
  %i4 = load i32* %i
  %sum5 = load i32* %sum
  %0 = add i32 %sum5, %i4
  store i32 %0, i32* %sum
  %i6 = load i32* %i
  %1 = add i32 %i6, 1
  store i32 %1, i32* %i
  br label %header

header:                                           ; preds = %bodyBB, %next
  %i2 = load i32* %i
  %b3 = load i32* %b1
  %2 = icmp slt i32 %i2, %b3
  br i1 %2, label %bodyBB, label %footer
}
