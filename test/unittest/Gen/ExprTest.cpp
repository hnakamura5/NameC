#include "NameC.h"
#include <gtest/gtest.h>

using namespace namec;

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

TEST(ExprTest, DesignatedInitTest) {
  auto *Init =
      C.expr_designated_init({{"a", C.expr_int(1)}, {"b", C.expr_int(2)}});
  EXPECT_EQ(Init->to_string(), "{.a=1,.b=2}");
}

TEST(ExprTest, InitListTest) {
  auto *Init = C.expr_init_list({C.expr_int(1), C.expr_int(2)});
  EXPECT_EQ(Init->to_string(), "{1,2}");
}

TEST(ExprTest, GenericSelectionTest) {
  auto *Control = C.expr_int(1);
  auto *Gen = C.expr_generic_selection(Control);
  Gen->add_assoc(C.type_int(), C.expr_int(2));
  Gen->add_assoc(C.type_char(), C.expr_int(3));
  Gen->add_default(C.expr_int(4));
  EXPECT_EQ(Gen->to_string(), "_Generic(1,int:2,char:3,default:4)");
}

// Short form tests.
TEST(ExprTest, ShortFormLiteralTest) {
  EXPECT_EQ(C.EX(1)->to_string(), "1");
  EXPECT_EQ(C.EX(C.EX(1))->to_string(), "(1)");
  EXPECT_EQ(C.EX(2.0f)->to_string(), "2.000000f");
  EXPECT_EQ(C.EX(2.0)->to_string(), "2.000000");
  EXPECT_EQ(C.EX('x')->to_string(), "'x'");
  EXPECT_EQ(C.EX("var")->to_string(), "var");
  EXPECT_EQ(C.STR("str")->to_string(), "\"str\"");
  EXPECT_EQ(C.EX(true)->to_string(), "1");
  EXPECT_EQ(C.EX(nullptr)->to_string(), "((void*)0)");
}

TEST(ExprTest, ShortFormExpressionTest) {
  EXPECT_EQ(C.EX("!", false)->to_string(), "(!0)");
  EXPECT_EQ(C.EX("x", "+", 1)->to_string(), "(x+1)");
  EXPECT_EQ(C.CALL("func", 1, 2)->to_string(), "(func(1,2))");
  EXPECT_EQ(C.IDX("arr", 0, 1)->to_string(), "(arr[0][1])");
}
