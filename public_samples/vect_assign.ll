; ModuleID = 'vect_assign.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = global <2 x float> zeroinitializer

define float @vectassign() {
vectassign:
  %t = alloca <2 x float>
  store <2 x float> zeroinitializer, <2 x float>* %t
  %v = load <2 x float>* @v
  store <2 x float> %v, <2 x float>* %t
  %t1 = load <2 x float>* %t
  %0 = extractelement <2 x float> %t1, i32 0
  %t2 = load <2 x float>* %t
  %1 = extractelement <2 x float> %t2, i32 1
  %2 = fmul float %0, %1
  ret float %2
}
