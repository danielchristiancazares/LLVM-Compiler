; ModuleID = 'samples/for_loop.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = global <2 x float> zeroinitializer

define float @fortest(i32 %a) {
entry:
  %a1 = alloca i32
  store i32 %a, i32* %a1
  br label %next

next:                                             ; preds = %entry
  %i = alloca i32
  %sum = alloca float
  %v = load <2 x float>* @v
  %0 = extractelement <2 x float> %v, i32 0
  store float %0, float* %sum
  store i32 0, i32* %i
  br label %header

footer:                                           ; preds = %header
  %sum7 = load float* %sum
  ret float %sum7

stepBB:                                           ; preds = %bodyBB
  %i6 = load i32* %i
  %1 = add i32 %i6, 1
  store i32 %1, i32* %i
  br label %header

bodyBB:                                           ; preds = %header
  %v4 = load <2 x float>* @v
  %2 = extractelement <2 x float> %v4, i32 1
  %sum5 = load float* %sum
  %3 = fmul float %sum5, %2
  store float %3, float* %sum
  br label %stepBB

header:                                           ; preds = %stepBB, %next
  %i2 = load i32* %i
  %a3 = load i32* %a1
  %4 = icmp slt i32 %i2, %a3
  br i1 %4, label %bodyBB, label %footer
}
