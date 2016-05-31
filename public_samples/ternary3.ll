; ModuleID = 'ternary3.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i1 @bar() {
bar:
  br i1 false, label %IFthenBB, label %IFelseBB

IFthenBB:                                         ; preds = %bar
  ret i1 false

IFelseBB:                                         ; preds = %bar
  br i1 true, label %IFthenBB1, label %IFelseBB2

IFfooterBB:                                       ; preds = %IFfooterBB3
  unreachable

IFthenBB1:                                        ; preds = %IFelseBB
  ret i1 true

IFelseBB2:                                        ; preds = %IFelseBB
  ret i1 false

IFfooterBB3:                                      ; No predecessors!
  br label %IFfooterBB
}

define i1 @foo() {
foo:
  br i1 true, label %TrueBB, label %FalseBB

TrueBB:                                           ; preds = %foo
  br label %CondFooterBB

FalseBB:                                          ; preds = %foo
  br label %CondFooterBB

CondFooterBB:                                     ; preds = %FalseBB, %TrueBB
  %PHINode = phi i1 [ false, %TrueBB ], [ true, %FalseBB ]
  br i1 %PHINode, label %IFthenBB, label %IFelseBB

IFthenBB:                                         ; preds = %CondFooterBB
  ret i1 true

IFelseBB:                                         ; preds = %CondFooterBB
  br i1 true, label %IFthenBB1, label %IFelseBB2

IFfooterBB:                                       ; preds = %IFfooterBB3
  unreachable

IFthenBB1:                                        ; preds = %IFelseBB
  %FunctionCall = call i1 @bar()
  br i1 %FunctionCall, label %TrueBB4, label %FalseBB5

IFelseBB2:                                        ; preds = %IFelseBB
  ret i1 false

IFfooterBB3:                                      ; No predecessors!
  br label %IFfooterBB

TrueBB4:                                          ; preds = %IFthenBB1
  br label %CondFooterBB6

FalseBB5:                                         ; preds = %IFthenBB1
  br label %CondFooterBB6

CondFooterBB6:                                    ; preds = %FalseBB5, %TrueBB4
  %PHINode7 = phi i1 [ false, %TrueBB4 ], [ true, %FalseBB5 ]
  ret i1 %PHINode7
}

define i1 @test() {
test:
  br i1 true, label %TrueBB, label %FalseBB

TrueBB:                                           ; preds = %test
  br i1 false, label %TrueBB1, label %FalseBB2

FalseBB:                                          ; preds = %test
  %FunctionCall5 = call i1 @bar()
  br label %CondFooterBB

CondFooterBB:                                     ; preds = %CondFooterBB3, %FalseBB
  %PHINode6 = phi i1 [ %PHINode, %TrueBB ], [ %FunctionCall5, %FalseBB ]
  ret i1 %PHINode6

TrueBB1:                                          ; preds = %TrueBB
  %FunctionCall = call i1 @test()
  br label %CondFooterBB3

FalseBB2:                                         ; preds = %TrueBB
  %FunctionCall4 = call i1 @foo()
  br label %CondFooterBB3

CondFooterBB3:                                    ; preds = %FalseBB2, %TrueBB1
  %PHINode = phi i1 [ %FunctionCall, %TrueBB1 ], [ %FunctionCall4, %FalseBB2 ]
  br label %CondFooterBB
}
