; ModuleID = 'samples/if_nested.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = global <2 x float> zeroinitializer

define float @ifnested(float %arg) {
entry:
  %arg1 = alloca float
  store float %arg, float* %arg1
  br label %next

next:                                             ; preds = %entry
  %f = alloca float
  %arg2 = load float* %arg1
  %0 = fcmp ogt float %arg2, 1.000000e+00
  br i1 %0, label %thenBB, label %elseBB

footer:                                           ; preds = %footer3, %elseBB
  %f9 = load float* %f
  ret float %f9

elseBB:                                           ; preds = %next
  %arg8 = load float* %arg1
  store float %arg8, float* %f
  br label %footer

thenBB:                                           ; preds = %next
  %v = load <2 x float>* @v
  %1 = extractelement <2 x float> %v, i32 0
  %2 = fcmp olt float %1, 5.000000e-01
  br i1 %2, label %thenBB5, label %elseBB4

footer3:                                          ; preds = %thenBB5, %elseBB4
  br label %footer

elseBB4:                                          ; preds = %thenBB
  %v7 = load <2 x float>* @v
  %3 = extractelement <2 x float> %v7, i32 1
  store float %3, float* %f
  br label %footer3

thenBB5:                                          ; preds = %thenBB
  %v6 = load <2 x float>* @v
  %4 = extractelement <2 x float> %v6, i32 0
  store float %4, float* %f
  br label %footer3
}
