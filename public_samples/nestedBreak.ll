; ModuleID = 'nestedBreak.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @test() {
test:
  %sum = alloca i32
  store i32 0, i32* %sum
  %k = alloca i32
  store i32 0, i32* %k
  br label %FORnext

FORnext:                                          ; preds = %test
  %0 = load i32* %k
  store i32 0, i32* %k
  br label %FORheaderBB

FORheaderBB:                                      ; preds = %FORstepBB, %FORnext
  %1 = load i32* %k
  %2 = icmp slt i32 %1, 10
  br i1 %2, label %FORbodyBB, label %FORfooterBB

FORbodyBB:                                        ; preds = %FORheaderBB
  %i = alloca i32
  store i32 0, i32* %i
  br label %FORnext1

FORstepBB:                                        ; preds = %FORfooterBB5
  %3 = load i32* %k
  %4 = add i32 %3, 1
  store i32 %4, i32* %k
  %5 = load i32* %k
  br label %FORheaderBB

FORfooterBB:                                      ; preds = %FORheaderBB
  %6 = load i32* %sum
  ret i32 %6

FORnext1:                                         ; preds = %FORbodyBB
  %7 = load i32* %i
  store i32 0, i32* %i
  br label %FORheaderBB2

FORheaderBB2:                                     ; preds = %FORstepBB4, %FORnext1
  %8 = load i32* %i
  %9 = icmp slt i32 %8, 5
  br i1 %9, label %FORbodyBB3, label %FORfooterBB5

FORbodyBB3:                                       ; preds = %FORheaderBB2
  %j = alloca i32
  store i32 0, i32* %j
  br label %WHILEheaderBB

FORstepBB4:                                       ; preds = %IFthenBB10
  %10 = load i32* %i
  %11 = add i32 %10, 1
  store i32 %11, i32* %i
  %12 = load i32* %i
  br label %FORheaderBB2

FORfooterBB5:                                     ; preds = %IFfooterBB9, %FORheaderBB2
  br label %FORstepBB

WHILEheaderBB:                                    ; preds = %IFthenBB8, %21, %FORbodyBB3
  %13 = load i32* %j
  %14 = add i32 %13, 1
  store i32 %14, i32* %j
  %15 = icmp slt i32 %13, 5
  br i1 %15, label %WHILEbodyBB, label %WHILEfooterBB

WHILEbodyBB:                                      ; preds = %WHILEheaderBB
  %16 = load i32* %j
  switch i32 %16, label %default [
    i32 1, label %21
    i32 2, label %24
  ]

WHILEfooterBB:                                    ; preds = %IFfooterBB, %WHILEheaderBB
  %17 = load i32* %i
  %18 = icmp eq i32 %17, 3
  br i1 %18, label %IFthenBB10, label %IFfooterBB11

footerBB:                                         ; preds = %IFthenBB, %default
  %19 = load i32* %j
  %20 = icmp eq i32 %19, 4
  br i1 %20, label %IFthenBB8, label %IFfooterBB9

; <label>:21                                      ; preds = %WHILEbodyBB
  %22 = load i32* %sum
  %23 = load i32* %sum
  %addAssign = add i32 %23, 1
  store i32 %addAssign, i32* %sum
  br label %WHILEheaderBB

; <label>:24                                      ; preds = %WHILEbodyBB
  %25 = load i32* %sum
  %26 = load i32* %sum
  %addAssign6 = add i32 %26, 2
  store i32 %addAssign6, i32* %sum
  %27 = load i32* %j
  %28 = icmp eq i32 %27, 2
  br i1 %28, label %IFthenBB, label %IFfooterBB

default:                                          ; preds = %WHILEbodyBB
  %29 = load i32* %sum
  %30 = load i32* %sum
  %addAssign7 = add i32 %30, 2
  store i32 %addAssign7, i32* %sum
  br label %footerBB

IFthenBB:                                         ; preds = %24
  br label %footerBB

IFfooterBB:                                       ; preds = %24
  br label %WHILEfooterBB

IFthenBB8:                                        ; preds = %footerBB
  br label %WHILEheaderBB

IFfooterBB9:                                      ; preds = %footerBB
  br label %FORfooterBB5

IFthenBB10:                                       ; preds = %WHILEfooterBB
  br label %FORstepBB4

IFfooterBB11:                                     ; preds = %WHILEfooterBB
}
