; ModuleID = 'if_test2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @iftest2() {
iftest2:
  %f = alloca float
  store float 0.000000e+00, float* %f
  %a = alloca float
  store float 0.000000e+00, float* %a
  %a1 = load float* %a
  %0 = fcmp ogt float %a1, 1.000000e+00
  br i1 %0, label %thenBB, label %elseBB

footerBB:                                         ; preds = %thenBB, %elseBB
  %f2 = load float* %f
  ret float %f2

elseBB:                                           ; preds = %iftest2
  store float 1.000000e+00, float* %f
  br label %footerBB

thenBB:                                           ; preds = %iftest2
  store float 2.000000e+00, float* %f
  br label %footerBB
}
