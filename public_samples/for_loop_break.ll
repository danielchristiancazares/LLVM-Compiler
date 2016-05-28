; ModuleID = 'for_loop_break.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = global float 0.000000e+00
@a = global i32 0

define float @fortest() {
fortest:
  %i = alloca i32
  store i32 0, i32* %i
  %sum = alloca float
  store float 0.000000e+00, float* %sum
  %v = load float* @v
  store float %v, float* %sum
  br label %next

next:                                             ; preds = %fortest
  store i32 0, i32* %i
  br label %headerBB

headerBB:                                         ; preds = %stepBB, %next
  %i1 = load i32* %i
  %a = load i32* @a
  %0 = icmp slt i32 %i1, %a
  br i1 %0, label %bodyBB, label %footerBB

bodyBB:                                           ; preds = %headerBB
  %i2 = load i32* %i
  %1 = icmp sgt i32 %i2, 5
  br i1 %1, label %thenBB, label %footerBB3

stepBB:                                           ; No predecessors!
  %i5 = load i32* %i
  %2 = add i32 %i5, 1
  store i32 %2, i32* %i
  %3 = load i32* %i
  br label %headerBB

footerBB:                                         ; preds = %thenBB, %headerBB
  br label %footerBB3

footerBB3:                                        ; preds = %footerBB, %bodyBB
  %sum6 = load float* %sum
  ret float %sum6

thenBB:                                           ; preds = %bodyBB
  %sum4 = load float* %sum
  %4 = fmul float %sum4, 3.000000e+00
  store float %4, float* %sum
  br label %footerBB
}
