; ModuleID = 'nested_case_switch.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = global <2 x float> zeroinitializer
@y = global i32 0

define i32 @test() {
test:
  %0 = load i32* @y
  switch i32 %0, label %default [
    i32 1, label %1
    i32 3, label %2
  ]

footerBB:                                         ; No predecessors!
  unreachable

; <label>:1                                       ; preds = %test
  ret i32 2

; <label>:2                                       ; preds = %test
  ret i32 3

default:                                          ; preds = %test
  %negativeNum = sub i32 0, 1
  ret i32 %negativeNum
}
