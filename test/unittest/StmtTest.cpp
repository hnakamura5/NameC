#include "NameC.h"
#include <gtest/gtest.h>

using namespace namec;

Context C;
Scope S(C);

TEST(StmtTest, RawStmtTest) {
  auto *R = S.stmt_raw("raw_stmt");
  EXPECT_EQ(R->to_string(), "raw_stmt");
}

TEST(StmtTest, EmitAndToString) {
  auto *R = S.stmt_raw("raw_stmt");
  std::stringstream SS;
  R->emit(SS);
  EXPECT_EQ(SS.str(), R->to_string());
}

TEST(StmtTest, EmptyTest) {
  auto *E = S.stmt_empty();
  EXPECT_EQ(E->to_string(), ";");
}

TEST(StmtTest, DeclStmtTest) {
  VarDecl D("var", C.type_int());
  auto *E = S.stmt_decl(&D);
  EXPECT_EQ(E->to_string(), "int var;");
}

TEST(StmtTest, IfStmtTest) {
  auto *I = S.stmt_if(C.expr_raw("cond"));
  auto *Then = I->get_then();
  Then->stmt_raw("raw_then_stmt");
  auto *Else = I->get_or_add_else();
  Else->stmt_raw("raw_else_stmt");
  EXPECT_EQ(I->to_string(), "if(cond){raw_then_stmt}else{raw_else_stmt}");
}

TEST(StmtTest, ForStmtTest) {
  auto *F =
      S.stmt_for(C.expr_raw("init"), C.expr_raw("cond"), C.expr_raw("step"));
  auto *Body = F->get_body();
  Body->stmt_raw("raw_body_stmt");
  EXPECT_EQ(F->to_string(), "for(init;cond;step){raw_body_stmt}");
}

TEST(StmtTest, WhileStmtTest) {
  auto *W = S.stmt_while(C.expr_raw("cond"));
  auto *Body = W->get_body();
  Body->stmt_raw("raw_body_stmt");
  EXPECT_EQ(W->to_string(), "while(cond){raw_body_stmt}");
}

TEST(StmtTest, DoStmtTest) {
  auto *D = S.stmt_do(C.expr_raw("cond"));
  auto *Body = D->get_body();
  Body->stmt_raw("raw_body_stmt");
  EXPECT_EQ(D->to_string(), "do{raw_body_stmt}while(cond)");
}

TEST(StmtTest, BlockStmtTest) {
  auto *B = S.stmt_block();
  B->get_scope()->stmt_raw("raw_body_stmt");
  EXPECT_EQ(B->to_string(), "{raw_body_stmt}");
}

TEST(StmtTest, ExprStmtTest) {
  auto *E = S.stmt_expr(C.expr_int(1));
  EXPECT_EQ(E->to_string(), "1;");
}

TEST(StmtTest, ReturnStmtTest) {
  auto *R = S.stmt_return(C.expr_int(1));
  EXPECT_EQ(R->to_string(), "return 1;");
}

TEST(StmtTest, BreakStmtTest) {
  auto *B = S.stmt_break();
  EXPECT_EQ(B->to_string(), "break;");
}

TEST(StmtTest, ContinueStmtTest) {
  auto *C = S.stmt_continue();
  EXPECT_EQ(C->to_string(), "continue;");
}

TEST(StmtTest, LabelStmtTest) {
  auto *L = S.stmt_label("label");
  EXPECT_EQ(L->to_string(), "label:;");
}

TEST(StmtTest, GotoStmtTest) {
  auto *G = S.stmt_goto("label1");
  EXPECT_EQ(G->to_string(), "goto label1;");
}

TEST(StmtTest, SwitchStmtTest) {
  auto *W = S.stmt_switch(C.expr_raw("cond"));
  auto *Body = W->get_body();
  Body->stmt_raw("raw_body_stmt");
  EXPECT_EQ(W->to_string(), "switch(cond){raw_body_stmt}");
}

TEST(StmtTest, SeqTest) {
  Scope S(C); // Local scope
  auto *S1 = S.stmt_raw("stmt1;");
  auto *S2 = S.stmt_raw("stmt2;");
  EXPECT_EQ(S.to_string(), "stmt1;stmt2;");
}