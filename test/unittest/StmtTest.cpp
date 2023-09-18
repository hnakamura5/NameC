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
  auto *Elseif1 = I->add_elseif(C.expr_raw("cond1"));
  Elseif1->stmt_raw("raw_elseif_stmt1");
  auto *Elseif2 = I->add_elseif(C.expr_raw("cond2"));
  Elseif2->stmt_raw("raw_elseif_stmt2");
  auto *Else = I->get_or_add_else();
  Else->stmt_raw("raw_else_stmt");
  EXPECT_EQ(I->to_string(), "if(cond){raw_then_stmt}"
                            "else if(cond1){raw_elseif_stmt1}"
                            "else if(cond2){raw_elseif_stmt2}"
                            "else{raw_else_stmt}");
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
  auto *Case1 = W->add_case(C.expr_int(1), true);
  Case1->get_body()->stmt_raw("raw_case1_stmt;");
  auto *Case2 = W->add_case(C.expr_int(2));
  Case2->get_body()->stmt_raw("raw_case2_stmt;");
  auto *Default = W->add_default();
  Default->get_body()->stmt_raw("raw_default_stmt;");
  EXPECT_EQ(W->to_string(), "switch(cond){"
                            "case 1:raw_case1_stmt;"
                            "case 2:raw_case2_stmt;break;"
                            "default:raw_default_stmt;break;}");
}

TEST(StmtTest, SeqTest) {
  Scope S(C); // Local scope
  auto *S1 = S.stmt_raw("stmt1;");
  auto *S2 = S.stmt_raw("stmt2;");
  EXPECT_EQ(S.to_string(), "stmt1;stmt2;");
}
