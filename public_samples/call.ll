; ModuleID = 'call.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @f(float %x, float %y, float %z) {
f:
  %x1 = alloca float
  store float %x, float* %x1
  %y2 = alloca float
  store float %x, float* %y2
  %z3 = alloca float
  store float %x, float* %z3
  %x4 = load float* %x1
  %y5 = load float* %y2
  %0 = fadd float %x4, %y5
  %z6 = load float* %z3
  %1 = fadd float %0, %z6
  ret float %1
}

define float @main() {
main:
  %x = alloca float
  store float 0.000000e+00, float* %x
  %x1 = load float* %x
  store float 6.000000e+00, float* %x
  ret void
}
