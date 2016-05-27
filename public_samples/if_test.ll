; ModuleID = 'samples/if_test.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@a = global float 0.000000e+00

define float @iftest() {
entry:
  br label %next

next:                                             ; preds = %entry
  %f = alloca float
  %a = load float* @a
  %0 = fcmp ogt float %a, 1.000000e+00
  br i1 %0, label %thenBB, label %elseBB

footer:                                           ; preds = %thenBB, %elseBB
  %f1 = load float* %f
  ret float %f1

elseBB:                                           ; preds = %next
  store float 1.000000e+00, float* %f
  br label %footer

thenBB:                                           ; preds = %next
  store float 2.000000e+00, float* %f
  br label %footer
}
