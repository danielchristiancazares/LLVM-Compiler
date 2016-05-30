; ModuleID = 'switch_scope.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@y = global i32 0

define i32 @test() {
test:
  %x = alloca i32
  store i32 1, i32* %x
  %z = alloca i32
  store i32 1, i32* %z
  %0 = load i32* @y
  switch i32 %0, label %default [
    i32 1, label %8
  ]

footerBB:                                         ; preds = %default, %8
  %1 = load i32* %x
  %2 = mul i32 %1, 10000
  %3 = load i32* %y
  %4 = mul i32 %3, 100
  %5 = add i32 %2, %4
  %6 = load i32* %z
  %7 = add i32 %5, %6
  ret i32 %7

; <label>:8                                       ; preds = %test
  %y = alloca i32
  store i32 5, i32* %y
  %x1 = alloca i32
  store i32 8, i32* %x1
  %9 = load i32* %z
  %10 = load i32* %y
  %11 = load i32* %x
  %12 = add i32 %10, %11
  store i32 %12, i32* %z
  br label %footerBB

default:                                          ; preds = %test
  br label %footerBB
}
