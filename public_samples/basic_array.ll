; ModuleID = 'basic_array.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @test() {
test:
  %x = alloca [5 x i32]
  store [5 x i32] zeroinitializer, [5 x i32]* %x
  %i = alloca i32
  store i32 0, i32* %i
  br label %FORnext

FORnext:                                          ; preds = %test
  %0 = load i32* %i
  store i32 0, i32* %i
  br label %FORheaderBB

FORheaderBB:                                      ; preds = %FORstepBB, %FORnext
  %1 = load i32* %i
  %2 = icmp slt i32 %1, 5
  br i1 %2, label %FORbodyBB, label %FORfooterBB

FORbodyBB:                                        ; preds = %FORheaderBB
  %3 = load i32* %i
  %4 = load [5 x i32]* %x
  %ArrayAccess = getelementptr [5 x i32]* %x, i32 0, i32 %3
  %ArrayAccessed = load i32* %ArrayAccess
  %5 = load i32* %i
  %6 = sub i32 4, %5
  store i32 %6, i32* %ArrayAccess
  br label %FORstepBB

FORstepBB:                                        ; preds = %FORbodyBB
  %7 = load i32* %i
  %8 = add i32 %7, 1
  store i32 %8, i32* %i
  %9 = load i32* %i
  br label %FORheaderBB

FORfooterBB:                                      ; preds = %FORheaderBB
  br label %FORnext1

FORnext1:                                         ; preds = %FORfooterBB
  %10 = load i32* %i
  store i32 0, i32* %i
  br label %FORheaderBB2

FORheaderBB2:                                     ; preds = %FORstepBB4, %FORnext1
  %11 = load i32* %i
  %12 = icmp slt i32 %11, 5
  br i1 %12, label %FORbodyBB3, label %FORfooterBB5

FORbodyBB3:                                       ; preds = %FORheaderBB2
  %13 = load i32* %i
  %14 = load [5 x i32]* %x
  %ArrayAccess6 = getelementptr [5 x i32]* %x, i32 0, i32 %13
  %ArrayAccessed7 = load i32* %ArrayAccess6
  %15 = load [5 x i32]* %x
  %ArrayAccess8 = getelementptr [5 x i32]* %x, i32 0, i32 %ArrayAccessed7
  %ArrayAccessed9 = load i32* %ArrayAccess8
  %16 = load [5 x i32]* %x
  %ArrayAccess10 = getelementptr [5 x i32]* %x, i32 0, i32 %ArrayAccessed9
  %ArrayAccessed11 = load i32* %ArrayAccess10
  %17 = load i32* %i
  %18 = load [5 x i32]* %x
  %ArrayAccess12 = getelementptr [5 x i32]* %x, i32 0, i32 %17
  %ArrayAccessed13 = load i32* %ArrayAccess12
  %19 = load [5 x i32]* %x
  %ArrayAccess14 = getelementptr [5 x i32]* %x, i32 0, i32 %ArrayAccessed13
  %ArrayAccessed15 = load i32* %ArrayAccess14
  store i32 %ArrayAccessed15, i32* %ArrayAccess10
  br label %FORstepBB4

FORstepBB4:                                       ; preds = %FORbodyBB3
  %20 = load i32* %i
  %21 = load i32* %i
  %addAssign = add i32 %21, 2
  store i32 %addAssign, i32* %i
  br label %FORheaderBB2

FORfooterBB5:                                     ; preds = %FORheaderBB2
  br label %FORnext16

FORnext16:                                        ; preds = %FORfooterBB5
  %22 = load i32* %i
  store i32 0, i32* %i
  br label %FORheaderBB17

FORheaderBB17:                                    ; preds = %FORstepBB19, %FORnext16
  %23 = load i32* %i
  %24 = icmp slt i32 %23, 4
  br i1 %24, label %FORbodyBB18, label %FORfooterBB20

FORbodyBB18:                                      ; preds = %FORheaderBB17
  %25 = load i32* %i
  %26 = add i32 %25, 1
  store i32 %26, i32* %i
  %27 = load i32* %i
  %28 = load [5 x i32]* %x
  %ArrayAccess21 = getelementptr [5 x i32]* %x, i32 0, i32 %27
  %ArrayAccessed22 = load i32* %ArrayAccess21
  %29 = load i32* %i
  %30 = add i32 %29, 1
  store i32 %30, i32* %i
  %31 = load i32* %i
  %32 = load [5 x i32]* %x
  %ArrayAccess23 = getelementptr [5 x i32]* %x, i32 0, i32 %31
  %ArrayAccessed24 = load i32* %ArrayAccess23
  %addAssign25 = add i32 %ArrayAccessed24, 1
  store i32 %addAssign25, i32* %ArrayAccess21
  %33 = load i32* %i
  %34 = add i32 %33, 1
  store i32 %34, i32* %i
  %35 = load i32* %i
  br label %FORstepBB19

FORstepBB19:                                      ; preds = %FORbodyBB18
  %36 = load i32* %i
  %37 = load i32* %i
  store i32 %37, i32* %i
  br label %FORheaderBB17

FORfooterBB20:                                    ; preds = %FORheaderBB17
  %38 = load [5 x i32]* %x
  %ArrayAccess26 = getelementptr [5 x i32]* %x, i32 0, i32 0
  %ArrayAccessed27 = load i32* %ArrayAccess26
  %39 = mul i32 %ArrayAccessed27, 10000
  %40 = load [5 x i32]* %x
  %ArrayAccess28 = getelementptr [5 x i32]* %x, i32 0, i32 1
  %ArrayAccessed29 = load i32* %ArrayAccess28
  %41 = mul i32 %ArrayAccessed29, 1000
  %42 = add i32 %39, %41
  %43 = load [5 x i32]* %x
  %ArrayAccess30 = getelementptr [5 x i32]* %x, i32 0, i32 2
  %ArrayAccessed31 = load i32* %ArrayAccess30
  %44 = mul i32 %ArrayAccessed31, 100
  %45 = add i32 %42, %44
  %46 = load [5 x i32]* %x
  %ArrayAccess32 = getelementptr [5 x i32]* %x, i32 0, i32 3
  %ArrayAccessed33 = load i32* %ArrayAccess32
  %47 = mul i32 %ArrayAccessed33, 10
  %48 = add i32 %45, %47
  %49 = load [5 x i32]* %x
  %ArrayAccess34 = getelementptr [5 x i32]* %x, i32 0, i32 4
  %ArrayAccessed35 = load i32* %ArrayAccess34
  %50 = add i32 %48, %ArrayAccessed35
  ret i32 %50
}
