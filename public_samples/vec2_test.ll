; ModuleID = 'vec2_test.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = global <2 x float> zeroinitializer

define float @v2test(float %x) {
v2test:
  %x1 = alloca float
  store float %x, float* %x1
  %v2 = alloca <2 x float>
  store <2 x float> zeroinitializer, <2 x float>* %v2
  %0 = load <2 x float>* %v2
  %1 = load <2 x float>* @v
  %2 = load float* %x1
  %3 = fmul <2 x float> %1, float %2
  store <2 x float> %3, <2 x float>* %v2
  %4 = load <2 x float>* %v2
  %vecfloat = extractelement <2 x float> %4, i32 0
  %5 = load <2 x float>* %v2
  %vecfloat2 = extractelement <2 x float> %5, i32 1
  %6 = fadd float %vecfloat, %vecfloat2
  ret float %6
}
