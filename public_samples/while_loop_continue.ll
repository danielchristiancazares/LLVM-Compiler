; ModuleID = 'while_loop_continue.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@b = global i32 0

define i32 @whiletest() {
whiletest:
  %i = alloca i32
  store i32 0, i32* %i
  %sum = alloca i32
  store i32 0, i32* %sum
  store i32 0, i32* %i
  store i32 0, i32* %sum
  br label %headerBB

headerBB:                                         ; preds = %IFthenBB, %whiletest
  %i1 = load i32* %i
  %b = load i32* @b
  %0 = icmp slt i32 %i1, %b
  br i1 %0, label %bodyBB, label %footerBB

bodyBB:                                           ; preds = %headerBB
  %sum2 = load i32* %sum
  %i3 = load i32* %i
  %1 = add i32 %sum2, %i3
  store i32 %1, i32* %sum
  %i4 = load i32* %i
  %2 = icmp sgt i32 %i4, 50
  br i1 %2, label %IFthenBB, label %IFfooterBB

footerBB:                                         ; preds = %IFfooterBB6, %headerBB
  %sum9 = load i32* %sum
  ret i32 %sum9

IFfooterBB:                                       ; preds = %bodyBB
  %sum5 = load i32* %sum
  %3 = icmp sgt i32 %sum5, 1024
  br i1 %3, label %IFthenBB7, label %IFfooterBB6

IFthenBB:                                         ; preds = %bodyBB
  store i32 1, i32* %i
  br label %headerBB

IFfooterBB6:                                      ; preds = %IFfooterBB
  %i8 = load i32* %i
  %4 = add i32 %i8, 1
  store i32 %4, i32* %i
  br label %footerBB

IFthenBB7:                                        ; preds = %IFfooterBB
}
