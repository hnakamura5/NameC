#include <NameC.h>
#include <gtest/gtest.h>

using namespace namec;

Context C;
Scope S(C);

TEST(StmtTest, RawStmtTest) {
  auto *R = S.stmt_raw("raw_stmt");
  std::stringstream SS;
  R->emit(SS);
  EXPECT_EQ(SS.str(), "raw_stmt");
}

TEST(StmtTest, EmptyTest) {
  auto *E = S.stmt_empty();
  std::stringstream SS;
  E->emit(SS);
  EXPECT_EQ(SS.str(), ";");
}

TEST(StmtTest, DeclStmtTest) {
  VarDecl D("var", C.type_int());
  auto *E = S.stmt_decl(&D);
  std::stringstream SS;
  E->emit(SS);
  EXPECT_EQ(SS.str(), "int var;");
}

TEST(StmtTest, IfStmtTest) {
  auto *I = S.stmt_if(C.expr_raw("cond"));
  auto *Then = I->get_then();
  Then->stmt_raw("raw_then_stmt");
  auto *Else = I->get_or_add_else();
  Else->stmt_raw("raw_else_stmt");
  std::stringstream SS;
  I->emit(SS);
  EXPECT_EQ(SS.str(), "if(cond){raw_then_stmt}else{raw_else_stmt}");
}

TEST(StmtTest, ForStmtTest) {
  auto *F =
      S.stmt_for(C.expr_raw("init"), C.expr_raw("cond"), C.expr_raw("step"));
  auto *Body = F->get_body();
  Body->stmt_raw("raw_body_stmt");
  std::stringstream SS;
  F->emit(SS);
  EXPECT_EQ(SS.str(), "for(init;cond;step){raw_body_stmt}");
}

TEST(StmtTest, WhileStmtTest) {
  auto *W = S.stmt_while(C.expr_raw("cond"));
  auto *Body = W->get_body();
  Body->stmt_raw("raw_body_stmt");
  std::stringstream SS;
  W->emit(SS);
  EXPECT_EQ(SS.str(), "while(cond){raw_body_stmt}");
}

TEST(StmtTest, DoStmtTest) {
  auto *D = S.stmt_do(C.expr_raw("cond"));
  auto *Body = D->get_body();
  Body->stmt_raw("raw_body_stmt");
  std::stringstream SS;
  D->emit(SS);
  EXPECT_EQ(SS.str(), "do{raw_body_stmt}while(cond)");
}

TEST(StmtTest, BlockStmtTest) {
  auto *B = S.stmt_block();
  B->get_scope()->stmt_raw("raw_body_stmt");
  std::stringstream SS;
  B->emit(SS);
  EXPECT_EQ(SS.str(), "{raw_body_stmt}");
}

TEST(StmtTest, ExprStmtTest) {
  auto *E = S.stmt_expr(C.expr_int(1));
  std::stringstream SS;
  E->emit(SS);
  EXPECT_EQ(SS.str(), "1;");
}

TEST(StmtTest, ReturnStmtTest) {
  auto *R = S.stmt_return(C.expr_int(1));
  std::stringstream SS;
  R->emit(SS);
  EXPECT_EQ(SS.str(), "return 1;");
}

TEST(StmtTest, BreakStmtTest) {
  auto *B = S.stmt_break();
  std::stringstream SS;
  B->emit(SS);
  EXPECT_EQ(SS.str(), "break;");
}

TEST(StmtTest, ContinueStmtTest) {
  auto *C = S.stmt_continue();
  std::stringstream SS;
  C->emit(SS);
  EXPECT_EQ(SS.str(), "continue;");
}

TEST(StmtTest, LabelStmtTest) {
  auto *L = S.stmt_label("label");
  std::stringstream SS;
  L->emit(SS);
  EXPECT_EQ(SS.str(), "label:;");
}

TEST(StmtTest, GotoStmtTest) {
  auto *G = S.stmt_goto("label1");
  std::stringstream SS;
  G->emit(SS);
  EXPECT_EQ(SS.str(), "goto label1;");
}
