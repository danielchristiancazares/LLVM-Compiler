; ModuleID = 'samples/multi_func.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = global <2 x float> zeroinitializer

define float @foo(float %a) {
entry:
  %a1 = alloca float
  store float %a, float* %a1
  br label %next

next:                                             ; preds = %entry
  %a2 = load float* %a1
  %v = load <2 x float>* @v
  %0 = extractelement <2 x float> %v, i32 0
  %1 = fadd float %0, %a2
  ret float %1
}

define float @bar(i32 %a) {
entry:
  %a1 = alloca i32
  store i32 %a, i32* %a1
  br label %next

next:                                             ; preds = %entry
  %t = alloca float
  %a2 = load i32* %a1
  %0 = icmp sgt i32 %a2, 1
  br i1 %0, label %thenBB, label %elseBB

footer:                                           ; preds = %thenBB, %elseBB
  %t3 = load float* %t
  ret float %t3

elseBB:                                           ; preds = %next
  store float 2.500000e-01, float* %t
  br label %footer

thenBB:                                           ; preds = %next
  %v = load <2 x float>* @v
  %1 = extractelement <2 x float> %v, i32 1
  store float %1, float* %t
  br label %footer
}
