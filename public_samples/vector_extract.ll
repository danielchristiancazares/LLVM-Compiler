; ModuleID = 'samples/vector_extract.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v2 = global <2 x float> zeroinitializer

define float @foo(float %f) {
entry:
  %f1 = alloca float
  store float %f, float* %f1
  br label %next

next:                                             ; preds = %entry
  %t = alloca float
  %v2 = load <2 x float>* @v2
  %0 = extractelement <2 x float> %v2, i32 1
  %f2 = load float* %f1
  %1 = fadd float %f2, %0
  store float %1, float* %t
  %t3 = load float* %t
  ret float %t3
}
