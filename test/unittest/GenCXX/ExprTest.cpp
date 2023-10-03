#include "NameCXX.h"
#include <gtest/gtest.h>

using namespace namecxx;

Context C;

TEST(ExprTest, RawExprTest) {
  auto *E = C.expr_raw("raw_val");
  EXPECT_EQ(E->to_string(), "raw_val");
}

TEST(ExprTest, EmitAndToString) {
  auto *E = C.expr_raw("raw_val");
  std::stringstream SS;
  E->emit(SS);
  EXPECT_EQ(SS.str(), E->to_string());
}

TEST(ExprTest, VariableTest) {
  VarDecl D("var", C.type_int());
  auto *E = C.expr_var(&D);
  EXPECT_EQ(E->to_string(), "var");
}

TEST(ExprTest, SubscriptTest) {
  ArrayVarDecl D("arr", C.type_int(), {C.expr_int(1)});
  auto *E = C.expr_var(&D);
  auto *S = C.expr_subscr(E, C.expr_int(0));
  EXPECT_EQ(S->to_string(), "arr[0]");
}

TEST(ExprTest, CallTest) {
  auto *F = C.expr_raw("func");
  auto *E = C.expr_call(F, {C.expr_int(1), C.expr_float(2.0)});
  EXPECT_EQ(E->to_string(), "func(1,2.000000f)");
}

TEST(ExprTest, PreUnaryOpTest) {
  auto *Pre = C.expr_pre_unary("!", C.expr_int(1));
  EXPECT_EQ(Pre->to_string(), "!1");
}

TEST(ExprTest, PostUnaryOpTest) {
  auto *Post = C.expr_post_unary("++", C.expr_int(2));
  EXPECT_EQ(Post->to_string(), "2++");
}

TEST(ExprTest, BinaryOpTest) {
  auto *Bin = C.expr_binary("+", C.expr_int(1), C.expr_int(2));
  EXPECT_EQ(Bin->to_string(), "1+2");
}

TEST(ExprTest, TernaryOpTest) {
  auto *Cond = C.expr_int(1);
  auto *Then = C.expr_int(2);
  auto *Else = C.expr_int(3);
  auto *Ter = C.expr_ternary(Cond, Then, Else);
  EXPECT_EQ(Ter->to_string(), "1?2:3");
}

TEST(ExprTest, CastTest) {
  auto *Cast = C.expr_cast(C.type_int(), C.expr_char('x'));
  EXPECT_EQ(Cast->to_string(), "(int)'x'");
}

TEST(ExprTest, ParenTest) {
  auto *Bin = C.expr_binary("+", C.expr_int(1), C.expr_int(2));
  auto *Paren = C.expr_paren(Bin);
  EXPECT_EQ(Paren->to_string(), "(1+2)");
}

TEST(ExprTest, InitListTest) {
  auto *Init = C.expr_init_list({C.expr_int(1), C.expr_int(2)});
  EXPECT_EQ(Init->to_string(), "{1,2}");
}

TEST(ExprTest, NewExprTest) {
  auto *New = C.expr_new(C.type_int(), {});
  EXPECT_EQ(New->to_string(), "new int()");
}

TEST(ExprTest, DeleteExprTest) {
  auto *Delete = C.expr_delete(C.expr_raw("deleted"));
  EXPECT_EQ(Delete->to_string(), "delete deleted");
}

TEST(ExprTest, UserDefinedLiteralTest) {
  auto *UDL = C.expr_user_defined_literal(C.expr_int(2), "hf");
  EXPECT_EQ(UDL->to_string(), "2_hf");
}

TEST(ExprTest, QualNameExprTest) {
  auto *QNE = C.expr_qual_name(C.QN("ns", "var"));
  EXPECT_EQ(QNE->to_string(), "ns::var");
}

TEST(ExprTest, LambdaExprTest) {
  auto *X = C.decl_var("x", C.type_int());
  auto *Lambda =
      C.expr_lambda({C.expr_raw("copy"), C.expr_addr(C.expr_raw("ref"))}, {X},
                    true, C.type_int());

  Lambda->get_body()->stmt_return(
      C.expr_binary("+", C.expr_var(X), C.expr_int(1)));
  EXPECT_EQ(Lambda->to_string(), "[copy,&ref](int x,...)->int{return x+1;}");
}

TEST(ExprTest, InstantiateExprTest) {
  auto *Inst = C.expr_raw_instantiate(
      "Template", {C.type_int(), C.type_float(), C.expr_int(1)});
  EXPECT_EQ(Inst->to_string(), "Template<int,float,1>");
}

TEST(ExprTest, PackExpansionExprTest) {
  auto *Pack = C.expr_pack_expansion(C.expr_raw("Pack"));
  EXPECT_EQ(Pack->to_string(), "Pack...");
}

TEST(ExprTest, FoldExprTest) {
  auto *FoldL = C.expr_fold("+", C.expr_raw("Pack"), C.expr_int(1), true);
  EXPECT_EQ(FoldL->to_string(), "(1+ ... +Pack)");
  auto *FoldR = C.expr_fold("+", C.expr_raw("Pack"), C.expr_int(1), false);
  EXPECT_EQ(FoldR->to_string(), "(Pack+ ... +1)");
}
