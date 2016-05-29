; ModuleID = 'multi_func.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = global <2 x float> zeroinitializer

define float @foo(float %a) {
foo:
  %a1 = alloca float
  store float %a, float* %a1
  %v = load <2 x float>* @v
  %0 = extractelement <2 x float> %v, i32 0
  %a2 = load float* %a1
  ret void
}

define float @bar(i32 %a) {
bar:
  %a1 = alloca i32
  store i32 %a, i32* %a1
  %t = alloca float
  store float 0.000000e+00, float* %t
  %a2 = load i32* %a1
  %0 = icmp sgt i32 %a2, 1
  br i1 %0, label %IFthenBB, label %IFelseBB

IFthenBB:                                         ; preds = %bar
  %t3 = load float* %t
  %v = load <2 x float>* @v
  %1 = extractelement <2 x float> %v, i32 1
  store float %1, float* %t
  br label %IFfooterBB

IFelseBB:                                         ; preds = %bar
  %t4 = load float* %t
  store float 2.500000e-01, float* %t
  br label %IFfooterBB

IFfooterBB:                                       ; preds = %IFelseBB, %IFthenBB
  %t5 = load float* %t
  ret float %t5
}
