; ModuleID = 'random_switch.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@entropy = global i32 1

define i32 @mod(i32 %x, i32 %m) {
mod:
  %x1 = alloca i32
  store i32 %x, i32* %x1
  %m2 = alloca i32
  store i32 %m, i32* %m2
  %0 = load i32* %x1
  %1 = load i32* %x1
  %2 = load i32* %m2
  %3 = sdiv i32 %1, %2
  %4 = load i32* %m2
  %5 = mul i32 %3, %4
  %6 = sub i32 %0, %5
  ret i32 %6
}

define i32 @rand(i32 %max) {
rand:
  %max1 = alloca i32
  store i32 %max, i32* %max1
  %0 = load i32* @entropy
  %1 = load i32* @entropy
  %2 = mul i32 11, %1
  %3 = add i32 %2, 37
  %FunctionCall = call i32 @mod(i32 %3, i32 100)
  store i32 %FunctionCall, i32* @entropy
  %4 = load i32* %max1
  %FunctionCall2 = call i32 @mod(i32 %FunctionCall, i32 %4)
  ret i32 %FunctionCall2
}

define i1 @test() {
test:
  %s1 = alloca i32
  store i32 1, i32* %s1
  %checkpoint = alloca i1
  store i1 true, i1* %checkpoint
  %i = alloca i32
  store i32 0, i32* %i
  br label %FORnext

FORnext:                                          ; preds = %test
  %0 = load i32* %i
  store i32 0, i32* %i
  br label %FORheaderBB

FORheaderBB:                                      ; preds = %FORstepBB, %FORnext
  %1 = load i32* %i
  %2 = icmp slt i32 %1, 25
  br i1 %2, label %FORbodyBB, label %FORfooterBB

FORbodyBB:                                        ; preds = %FORheaderBB
  %FunctionCall = call i32 @rand(i32 5)
  switch i32 %FunctionCall, label %default [
    i32 1, label %11
    i32 100, label %15
    i32 4, label %18
    i32 3, label %21
  ]

FORstepBB:                                        ; preds = %footerBB
  %3 = load i32* %i
  %4 = add i32 %3, 1
  store i32 %4, i32* %i
  %5 = load i32* %i
  br label %FORheaderBB

FORfooterBB:                                      ; preds = %FORheaderBB
  %6 = load i1* %checkpoint
  %7 = load i1* %checkpoint
  %8 = load i32* %s1
  %9 = icmp eq i32 %8, 8526
  %LogicalAnd = and i1 %7, %9
  store i1 %LogicalAnd, i1* %checkpoint
  %10 = load i32* %s1
  store i32 1, i32* %s1
  br label %FORnext1

footerBB:                                         ; preds = %default, %21, %15
  br label %FORstepBB

; <label>:11                                      ; preds = %FORbodyBB
  %12 = load i32* %s1
  %13 = add i32 %12, 1
  store i32 %13, i32* %s1
  %14 = load i32* %s1
  br label %15

; <label>:15                                      ; preds = %11, %FORbodyBB
  %16 = load i32* %s1
  %17 = load i32* %s1
  %mulAssign = mul i32 %17, 2
  store i32 %mulAssign, i32* %s1
  br label %footerBB

; <label>:18                                      ; preds = %FORbodyBB
  %19 = load i32* %s1
  %20 = add i32 %19, 3
  br label %21

; <label>:21                                      ; preds = %18, %FORbodyBB
  %22 = load i32* %s1
  %23 = load i32* %s1
  %addAssign = add i32 %23, 2
  store i32 %addAssign, i32* %s1
  br label %footerBB

default:                                          ; preds = %FORbodyBB
  br label %footerBB

FORnext1:                                         ; preds = %FORfooterBB
  %24 = load i32* %i
  store i32 0, i32* %i
  br label %FORheaderBB2

FORheaderBB2:                                     ; preds = %FORstepBB4, %FORnext1
  %25 = load i32* %i
  %26 = icmp slt i32 %25, 25
  br i1 %26, label %FORbodyBB3, label %FORfooterBB5

FORbodyBB3:                                       ; preds = %FORheaderBB2
  %FunctionCall8 = call i32 @rand(i32 7)
  switch i32 %FunctionCall8, label %default7 [
    i32 1, label %35
    i32 2, label %38
    i32 3, label %41
    i32 4, label %44
    i32 5, label %45
    i32 6, label %48
  ]

FORstepBB4:                                       ; preds = %footerBB6
  %27 = load i32* %i
  %28 = add i32 %27, 1
  store i32 %28, i32* %i
  %29 = load i32* %i
  br label %FORheaderBB2

FORfooterBB5:                                     ; preds = %FORheaderBB2
  %30 = load i1* %checkpoint
  %31 = load i1* %checkpoint
  %32 = load i32* %s1
  %33 = icmp eq i32 %32, 379810
  %LogicalAnd13 = and i1 %31, %33
  store i1 %LogicalAnd13, i1* %checkpoint
  %34 = load i1* %checkpoint
  ret i1 %34

footerBB6:                                        ; preds = %default7, %44, %41, %38
  br label %FORstepBB4

; <label>:35                                      ; preds = %FORbodyBB3
  %36 = load i32* %s1
  %37 = load i32* %s1
  %addAssign9 = add i32 %37, 2
  store i32 %addAssign9, i32* %s1
  br label %38

; <label>:38                                      ; preds = %35, %FORbodyBB3
  %39 = load i32* %s1
  %40 = load i32* %s1
  %mulAssign10 = mul i32 %40, 2
  store i32 %mulAssign10, i32* %s1
  br label %footerBB6

; <label>:41                                      ; preds = %FORbodyBB3
  %42 = load i32* %s1
  %43 = load i32* %s1
  %addAssign11 = add i32 %43, 1
  store i32 %addAssign11, i32* %s1
  br label %footerBB6

; <label>:44                                      ; preds = %FORbodyBB3
  br label %footerBB6

; <label>:45                                      ; preds = %FORbodyBB3
  %46 = load i32* %s1
  %47 = add i32 %46, 1
  store i32 %47, i32* %s1
  br label %48

; <label>:48                                      ; preds = %45, %FORbodyBB3
  %49 = load i32* %s1
  %50 = load i32* %s1
  %mulAssign12 = mul i32 %50, 3
  store i32 %mulAssign12, i32* %s1
  br label %default7

default7:                                         ; preds = %48, %FORbodyBB3
  br label %footerBB6
}
