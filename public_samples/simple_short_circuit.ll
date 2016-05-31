; ModuleID = 'simple_short_circuit.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i1 @test() {
test:
  %x = alloca i32
  store i32 0, i32* %x
  %y = alloca i32
  store i32 1, i32* %y
  %checkpoint = alloca i1
  store i1 true, i1* %checkpoint
  %0 = load i32* %x
  %1 = icmp eq i32 %0, 0
  %2 = load i32* %x
  %3 = add i32 %2, 1
  store i32 %3, i32* %x
  %4 = load i32* %x
  %5 = icmp eq i32 %4, 1
  %LogicalAnd = or i1 %1, %5
  br i1 %LogicalAnd, label %IFthenBB, label %IFelseBB

IFthenBB:                                         ; preds = %test
  %6 = load i1* %checkpoint
  %7 = load i1* %checkpoint
  %LogicalAnd1 = and i1 %7, true
  store i1 %LogicalAnd1, i1* %checkpoint
  br label %IFfooterBB

IFelseBB:                                         ; preds = %test
  %8 = load i1* %checkpoint
  store i1 false, i1* %checkpoint
  br label %IFfooterBB

IFfooterBB:                                       ; preds = %IFelseBB, %IFthenBB
  %9 = load i1* %checkpoint
  %10 = load i1* %checkpoint
  %11 = load i32* %x
  %12 = icmp eq i32 %11, 0
  %LogicalAnd2 = and i1 %10, %12
  store i1 %LogicalAnd2, i1* %checkpoint
  %13 = load i32* %y
  %14 = icmp eq i32 %13, 0
  %15 = load i32* %y
  %16 = add i32 %15, 1
  store i32 %16, i32* %y
  %17 = load i32* %y
  %18 = icmp eq i32 %17, 1
  %LogicalAnd3 = and i1 %14, %18
  br i1 %LogicalAnd3, label %IFthenBB4, label %IFfooterBB5

IFthenBB4:                                        ; preds = %IFfooterBB
  %19 = load i1* %checkpoint
  store i1 false, i1* %checkpoint
  br label %IFfooterBB5

IFfooterBB5:                                      ; preds = %IFthenBB4, %IFfooterBB
  %20 = load i1* %checkpoint
  %21 = load i1* %checkpoint
  %22 = load i32* %y
  %23 = icmp eq i32 %22, 1
  %LogicalAnd6 = and i1 %21, %23
  store i1 %LogicalAnd6, i1* %checkpoint
  %24 = load i32* %x
  store i32 0, i32* %x
  %25 = load i32* %y
  store i32 0, i32* %y
  %26 = load i32* %x
  %27 = add i32 %26, 1
  store i32 %27, i32* %x
  %28 = load i32* %x
  %29 = icmp eq i32 %28, 0
  %30 = load i32* %y
  %31 = add i32 %30, 1
  store i32 %31, i32* %y
  %32 = load i32* %y
  %33 = icmp eq i32 %32, 0
  %LogicalAnd7 = and i1 %29, %33
  br i1 %LogicalAnd7, label %IFthenBB8, label %IFfooterBB9

IFthenBB8:                                        ; preds = %IFfooterBB5
  %34 = load i1* %checkpoint
  store i1 false, i1* %checkpoint
  br label %IFfooterBB9

IFfooterBB9:                                      ; preds = %IFthenBB8, %IFfooterBB5
  %35 = load i1* %checkpoint
  %36 = load i1* %checkpoint
  %37 = load i32* %x
  %38 = icmp eq i32 %37, 1
  %LogicalAnd10 = and i1 %36, %38
  %39 = load i32* %y
  %40 = icmp eq i32 %39, 0
  %LogicalAnd11 = and i1 %LogicalAnd10, %40
  store i1 %LogicalAnd11, i1* %checkpoint
  %41 = load i32* %x
  store i32 0, i32* %x
  %42 = load i32* %y
  store i32 0, i32* %y
  %43 = load i32* %x
  %44 = add i32 %43, 1
  store i32 %44, i32* %x
  %45 = load i32* %x
  %46 = icmp eq i32 %45, 1
  %47 = load i32* %y
  %48 = add i32 %47, 1
  store i32 %48, i32* %y
  %49 = load i32* %y
  %50 = icmp eq i32 %49, 0
  %LogicalAnd12 = or i1 %46, %50
  br i1 %LogicalAnd12, label %IFthenBB13, label %IFfooterBB14

IFthenBB13:                                       ; preds = %IFfooterBB9
  %51 = load i1* %checkpoint
  %52 = load i1* %checkpoint
  %LogicalAnd15 = and i1 %52, true
  store i1 %LogicalAnd15, i1* %checkpoint
  br label %IFfooterBB14

IFfooterBB14:                                     ; preds = %IFthenBB13, %IFfooterBB9
  %53 = load i1* %checkpoint
  %54 = load i1* %checkpoint
  %55 = load i32* %x
  %56 = icmp eq i32 %55, 1
  %LogicalAnd16 = and i1 %54, %56
  %57 = load i32* %y
  %58 = icmp eq i32 %57, 0
  %LogicalAnd17 = and i1 %LogicalAnd16, %58
  store i1 %LogicalAnd17, i1* %checkpoint
  %59 = load i1* %checkpoint
  ret i1 %59
}
