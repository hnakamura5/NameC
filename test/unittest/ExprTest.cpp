#include <NameC.h>
#include <gtest/gtest.h>

using namespace namec;

Context C;

TEST(ExprTest, RawExprTest) {
  auto *E = C.expr_raw("raw_val");
  std::stringstream SS;
  E->emit(SS);
  EXPECT_EQ(SS.str(), "raw_val");
}

TEST(ExprTest, VariableTest) {
  VarDecl D("var", C.type_int());
  auto *E = C.expr_var(&D);
  std::stringstream SS;
  E->emit(SS);
  EXPECT_EQ(SS.str(), "var");
}

TEST(ExprTest, SubscriptTest) {
  ArrayVarDecl D("arr", C.type_int(), {C.expr_int(1)});
  auto *E = C.expr_var(&D);
  auto *S = C.expr_subscr(E, C.expr_int(0));
  std::stringstream SS;
  S->emit(SS);
  EXPECT_EQ(SS.str(), "arr[0]");
}

TEST(ExprTest, CallTest) {
  auto *F = C.expr_raw("func");
  auto *E = C.expr_call(F, {C.expr_int(1), C.expr_float(2.0)});
  std::stringstream SS;
  E->emit(SS);
  EXPECT_EQ(SS.str(), "func(1,2.000000f)");
}

TEST(ExprTest, PreUnaryOpTest) {
  auto *Pre = C.expr_pre_unary("!", C.expr_int(1));
  std::stringstream SS;
  Pre->emit(SS);
  EXPECT_EQ(SS.str(), "!1");
}

TEST(ExprTest, PostUnaryOpTest) {
  auto *Post = C.expr_pre_unary("++", C.expr_int(2));
  std::stringstream SS;
  Post->emit(SS);
  EXPECT_EQ(SS.str(), "++2");
}

TEST(ExprTest, BinaryOpTest) {
  auto *Bin = C.expr_binary("+", C.expr_int(1), C.expr_int(2));
  std::stringstream SS;
  Bin->emit(SS);
  EXPECT_EQ(SS.str(), "1+2");
}

TEST(ExprTest, TernaryOpTest) {
  auto *Cond = C.expr_int(1);
  auto *Then = C.expr_int(2);
  auto *Else = C.expr_int(3);
  auto *Tern = C.expr_ternary(Cond, Then, Else);
  std::stringstream SS;
  Tern->emit(SS);
  EXPECT_EQ(SS.str(), "1?2:3");
}

TEST(ExprTest, CastTest) {
  auto *Cast = C.expr_cast(C.type_int(), C.expr_char('x'));
  std::stringstream SS;
  Cast->emit(SS);
  EXPECT_EQ(SS.str(), "(int)'x'");
}

TEST(ExprTest, ParenTest) {
  auto *Bin = C.expr_binary("+", C.expr_int(1), C.expr_int(2));
  auto *Paren = C.expr_paren(Bin);
  std::stringstream SS;
  Paren->emit(SS);
  EXPECT_EQ(SS.str(), "(1+2)");
}
