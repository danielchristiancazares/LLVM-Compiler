; ModuleID = 'arith_gt_float.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@x = constant float 0.000000e+00
@y = constant float 0.000000e+00

define i1 @arith() {
entry:
  %x = load float* @x
  %y = load float* @y
  %0 = fcmp ogt float %x, %y
  ret i1 %0
}
