; ModuleID = 'scope.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@y = global i32 0

define i32 @test2() {
test2:
  %0 = load i32* @y
  %1 = add i32 %0, 1
  store i32 %1, i32* @y
  %2 = load i32* @y
  ret i32 %2
}

define i32 @test() {
test:
  %0 = load i32* @y
  store i32 1, i32* @y
  %y = alloca i32
  store i32 10, i32* %y
  %1 = load i32* %y
  %2 = add i32 %1, 1
  store i32 %2, i32* %y
  %3 = load i32* %y
  %y1 = alloca i32
  store i32 %3, i32* %y1
  %i = alloca i32
  store i32 0, i32* %i
  br label %FORnext

FORnext:                                          ; preds = %test
  %4 = load i32* %i
  store i32 0, i32* %i
  br label %FORheaderBB

FORheaderBB:                                      ; preds = %FORstepBB, %FORnext
  %5 = load i32* %i
  %6 = icmp slt i32 %5, 10
  br i1 %6, label %FORbodyBB, label %FORfooterBB

FORbodyBB:                                        ; preds = %FORheaderBB
  %y2 = alloca i32
  store i32 0, i32* %y2
  br label %FORstepBB

FORstepBB:                                        ; preds = %FORbodyBB
  %7 = load i32* %i
  %8 = add i32 %7, 1
  store i32 %8, i32* %i
  %9 = load i32* %i
  br label %FORheaderBB

FORfooterBB:                                      ; preds = %FORheaderBB
  br label %WHILEheaderBB

WHILEheaderBB:                                    ; preds = %WHILEbodyBB, %FORfooterBB
  %10 = load i32* %i
  %11 = icmp eq i32 %10, 10
  br i1 %11, label %WHILEbodyBB, label %WHILEfooterBB

WHILEbodyBB:                                      ; preds = %WHILEheaderBB
  %12 = load i32* %y
  %13 = add i32 %12, 1
  store i32 %13, i32* %y
  %14 = load i32* %y
  %y3 = alloca i32
  store i32 0, i32* %y3
  %15 = load i32* %i
  store i32 1, i32* %i
  br label %WHILEheaderBB

WHILEfooterBB:                                    ; preds = %WHILEheaderBB
  %16 = load i32* %i
  %17 = icmp ne i32 %16, 0
  br i1 %17, label %IFthenBB, label %IFfooterBB

IFthenBB:                                         ; preds = %WHILEfooterBB
  %y4 = alloca i32
  store i32 0, i32* %y4
  br label %IFfooterBB

IFfooterBB:                                       ; preds = %IFthenBB, %WHILEfooterBB
  %FunctionCall = call i32 @test2()
  %globalY = alloca i32
  store i32 %FunctionCall, i32* %globalY
  %18 = load i32* %globalY
  %19 = mul i32 %18, 1000
  %20 = load i32* %y4
  %21 = add i32 %19, %20
  ret i32 %21
}
