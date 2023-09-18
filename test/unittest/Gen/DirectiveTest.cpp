#include "NameC.h"
#include <gtest/gtest.h>

using namespace namec;

Context C;
Scope S(C);

TEST(DirectiveTest, RawDirectiveTest) {
  TopLevel F(C);
  auto *R = F.directive_raw("raw_directive");
  // Note that TopLevel output adds newline at the end.
  EXPECT_EQ(F.to_string(), "raw_directive\n");
}

TEST(DirectiveTest, EmitAndToString) {
  TopLevel F(C);
  auto *R = F.directive_raw("raw_directive");
  std::stringstream SS;
  R->emit(SS);
  EXPECT_EQ(SS.str(), R->to_string());
}

TEST(DirectiveTest, ScopeAndTopLevel) {
  TopLevel F(C);
  auto *R1 = F.directive_raw("raw_directive");
  auto *R2 = S.directive_raw("raw_directive");
  EXPECT_EQ(R1->to_string(), R2->to_string());
}

TEST(DirectiveTest, IncludeTest) {
  TopLevel F(C);
  auto *R = F.include("stdio.h");
  EXPECT_EQ(F.to_string(), "\n#include \"stdio.h\"\n\n");
}

TEST(DirectiveTest, SystemIncludeTest) {
  TopLevel F(C);
  auto *R = F.include_sys("stdio.h");
  EXPECT_EQ(F.to_string(), "\n#include <stdio.h>\n\n");
}

TEST(DirectiveTest, DefineFlagTest) {
  TopLevel F(C);
  auto *R = F.def_macro_flag("macro");
  EXPECT_EQ(F.to_string(), "\n#define macro\n\n");
}

TEST(DirectiveTest, DefineTest) {
  TopLevel F(C);
  auto *R = F.def_macro_value("macro", "value");
  EXPECT_EQ(F.to_string(), "\n#define macro value\n\n");
}

TEST(DirectiveTest, DefineFuncTest) {
  TopLevel F(C);
  auto *R = F.def_macro_func("macro", {"arg1", "arg2"});
  auto *Body = R->get_body();
  Body->stmt_raw("raw_body_directive1;");
  Body->stmt_raw("raw_body_directive2;");
  EXPECT_EQ(F.to_string(), "\n#define macro(arg1,arg2) "
                           "raw_body_directive1;"
                           "raw_body_directive2;\n\n");
}

TEST(DirectiveTest, UndefTest) {
  TopLevel F(C);
  auto *R = F.def_macro_undef("macro");
  EXPECT_EQ(F.to_string(), "\n#undef macro\n\n");
}

TEST(DirectiveTest, PragmaTest) {
  TopLevel F(C);
  auto *R = F.pragma("pragma");
  EXPECT_EQ(F.to_string(), "\n#pragma pragma\n\n");
}

TEST(DirectiveTest, InsertNewlineTest) {
  TopLevel F(C);
  auto *R = F.insert_newline(3);
  EXPECT_EQ(F.to_string(), "\n\n\n\n");
}

TEST(DirectiveTest, InsertSpaceTest) {
  TopLevel F(C);
  auto *R = F.insert_space(2);
  EXPECT_EQ(F.to_string(), "  \n");
}

TEST(DirectiveTest, InsertTabTest) {
  TopLevel F(C);
  auto *R = F.insert_tab(2);
  EXPECT_EQ(F.to_string(), "\t\t\n");
}

TEST(DirectiveTest, InsertCommentTest) {
  TopLevel F(C);
  auto *R = F.insert_comment("comment");
  EXPECT_EQ(F.to_string(), "/* comment */\n");
}

TEST(DirectiveTest, InsertLineCommentTest) {
  TopLevel F(C);
  auto *R = F.insert_line_comment("comment");
  EXPECT_EQ(F.to_string(), "// comment\n\n");
}

TEST(DirectiveTest, IfDirectiveTest) {
  TopLevel F(C);
  auto *I = F.directive_if(C.expr_raw("cond"));
  auto *Then = I->get_then();
  Then->directive_raw("raw_then_directive1");
  Then->directive_raw("raw_then_directive2");
  auto *Elif1 = I->add_elif(C.expr_raw("cond1"));
  Elif1->directive_raw("raw_elif1_directive1");
  Elif1->directive_raw("raw_elif1_directive2");
  auto *Elif2 = I->add_elif(C.expr_raw("cond2"));
  Elif2->directive_raw("raw_elif2_directive1");
  Elif2->directive_raw("raw_elif2_directive2");
  auto *Else = I->get_or_add_else();
  Else->directive_raw("raw_else_directive1");
  Else->directive_raw("raw_else_directive2");
  EXPECT_EQ(F.to_string(), "\n#if cond\n"
                           "raw_then_directive1\n"
                           "raw_then_directive2\n"
                           "\n#elif cond1\n"
                           "raw_elif1_directive1\n"
                           "raw_elif1_directive2\n"
                           "\n#elif cond2\n"
                           "raw_elif2_directive1\n"
                           "raw_elif2_directive2\n"
                           "\n#else\n"
                           "raw_else_directive1\n"
                           "raw_else_directive2\n"
                           "\n#endif\n\n");
}

TEST(DirectiveTest, IfdefDirectiveTest) {
  TopLevel F(C);
  auto *I = F.directive_ifdef("cond");
  auto *Then = I->get_then();
  Then->directive_raw("raw_then_directive1");
  Then->directive_raw("raw_then_directive2");
  auto *Elif1 = I->add_elif(C.expr_raw("cond1"));
  Elif1->directive_raw("raw_elif1_directive1");
  Elif1->directive_raw("raw_elif1_directive2");
  auto *Elif2 = I->add_elif(C.expr_raw("cond2"));
  Elif2->directive_raw("raw_elif2_directive1");
  Elif2->directive_raw("raw_elif2_directive2");
  auto *Else = I->get_or_add_else();
  Else->directive_raw("raw_else_directive1");
  Else->directive_raw("raw_else_directive2");
  EXPECT_EQ(F.to_string(), "\n#ifdef cond\n"
                           "raw_then_directive1\n"
                           "raw_then_directive2\n"
                           "\n#elif cond1\n"
                           "raw_elif1_directive1\n"
                           "raw_elif1_directive2\n"
                           "\n#elif cond2\n"
                           "raw_elif2_directive1\n"
                           "raw_elif2_directive2\n"
                           "\n#else\n"
                           "raw_else_directive1\n"
                           "raw_else_directive2\n"
                           "\n#endif\n\n");
}

TEST(DirectiveTest, IfndefDirectiveTest) {
  TopLevel F(C);
  auto *I = F.directive_ifndef("cond");
  auto *Then = I->get_then();
  Then->directive_raw("raw_then_directive1");
  Then->directive_raw("raw_then_directive2");
  auto *Elif1 = I->add_elif(C.expr_raw("cond1"));
  Elif1->directive_raw("raw_elif1_directive1");
  Elif1->directive_raw("raw_elif1_directive2");
  auto *Elif2 = I->add_elif(C.expr_raw("cond2"));
  Elif2->directive_raw("raw_elif2_directive1");
  Elif2->directive_raw("raw_elif2_directive2");
  auto *Else = I->get_or_add_else();
  Else->directive_raw("raw_else_directive1");
  Else->directive_raw("raw_else_directive2");
  EXPECT_EQ(F.to_string(), "\n#ifndef cond\n"
                           "raw_then_directive1\n"
                           "raw_then_directive2\n"
                           "\n#elif cond1\n"
                           "raw_elif1_directive1\n"
                           "raw_elif1_directive2\n"
                           "\n#elif cond2\n"
                           "raw_elif2_directive1\n"
                           "raw_elif2_directive2\n"
                           "\n#else\n"
                           "raw_else_directive1\n"
                           "raw_else_directive2\n"
                           "\n#endif\n\n");
}
