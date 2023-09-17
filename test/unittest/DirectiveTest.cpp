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
