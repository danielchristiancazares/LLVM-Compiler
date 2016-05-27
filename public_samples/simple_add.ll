; ModuleID = 'samples/simple_add.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @simpleadd(float %f) {
entry:
  %f1 = alloca float
  store float %f, float* %f1
  br label %next

next:                                             ; preds = %entry
  %x = alloca float
  %f2 = load float* %f1
  %0 = fadd float %f2, 1.000000e+00
  store float %0, float* %x
  %x3 = load float* %x
  ret float %x3
}
