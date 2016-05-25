; ModuleID = 'arith_post_dec_int.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@x = constant i32 0
@y = constant i32 0

define i32 @arith() {
entry:
  %x = load i32* @x
  %0 = sub i32 %x, 1
  store i32 %0, i32* @x
  %1 = load i32* @x
  %x1 = load i32* @x
  %2 = sub i32 %x1, 1
  store i32 %2, i32* @x
  %3 = load i32* @x
  ret i32 %3
}
