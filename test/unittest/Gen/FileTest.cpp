#include "NameC.h"
#include <gtest/gtest.h>

#include <fstream>
#include <string>

using namespace namec;

Context C;

TEST(FileTest, FileOutPut) {
  CFile F(C);
  TopLevel *T = F.get_first_top_level();
  FuncDecl *Main =
      T->def_func("main", C.type_int(), {C.decl_var("", C.type_void())});
  FuncScope *S = Main->get_or_add_body();
  S->stmt_return(C.expr_int(0));
  EXPECT_EQ(F.to_string(), "int main(void){return 0;}\n\n");
  std::filesystem::path P = "test.c";
  F.emit_to_file(P);
  std::ifstream IS(P);
  std::string Str;
  std::getline(IS, Str);
  EXPECT_EQ(Str, "int main(void){return 0;}");
  std::getline(IS, Str);
  EXPECT_EQ(Str, "");
  std::getline(IS, Str);
  EXPECT_EQ(Str, "");
  EXPECT_TRUE(IS.eof());
  IS.close();
  std::filesystem::remove(P);
}

TEST(FileTest, HelloWorld) {
  CFile F(C);
  TopLevel *T = F.get_first_top_level();
  T->include_sys("stdio.h");
  VarDecl *ArgC = C.decl_var("argc", C.type_int());
  VarDecl *ArgV = C.decl_array_var("argv", C.type_ptr(C.type_char()), {});
  FuncScope *Main =
      T->def_func("main", C.type_int(), {ArgC, ArgV})->get_or_add_body();
  IfStmt *If = Main->stmt_if(C.EX(C.EX(ArgC), ">", C.EX(1)));
  FuncScope *Then = If->get_then();
  FuncScope *Else = If->get_or_add_else();
  Then->stmt_call(C.EX("printf"), {C.STR("Hello, %s!\\n"), C.IDX(ArgV, 1)});
  Else->stmt_call(C.EX("printf"), {C.STR("Hello, world!\\n")});
  Main->stmt_return(C.EX(0));
  EXPECT_EQ(F.to_string(), "\n#include <stdio.h>\n\n"
                           "int main(int argc,char* argv[]){"
                           "if(((argc)>(1))){"
                           "printf(\"Hello, %s!\\n\",(argv[1]));"
                           "}else{"
                           "printf(\"Hello, world!\\n\");"
                           "}"
                           "return 0;"
                           "}\n\n");
}

TEST(FileTest, VarArgFunc) {
  CFile F(C);
  auto *T = F.get_first_top_level();
  auto *Arg1 = C.decl_var("arg1", C.type_float());
  auto *Arg2 = C.decl_var("arg2", C.type_int());
  auto *VaFunc = T->def_func("vafunc", C.type_void(), {Arg1, Arg2}, true);
  EXPECT_TRUE(VaFunc->is_vararg());
  EXPECT_TRUE(VaFunc->is_forward());
  auto *Body = VaFunc->get_or_add_body();
  EXPECT_FALSE(VaFunc->is_forward());
  auto *Args = Body->stmt_va_list("args");
  Body->stmt_va_start(C.expr_var(Args), C.expr_var(Arg2));
  Body->def_var("Arg3", C.type_int(),
                C.expr_va_arg(C.expr_var(Args), C.type_int()));
  Body->stmt_va_end(C.expr_var(Args));
  EXPECT_EQ(F.to_string(), "void vafunc(float arg1,int arg2,...){"
                           "va_list args;"
                           "va_start(args,arg2);"
                           "int Arg3=va_arg(args,int);"
                           "va_end(args);"
                           "}\n\n");
}

TEST(FileTest, SplitFunc) {
  CFile F(C);
  auto *T = F.get_first_top_level();
  auto *X = C.decl_var("x", C.type_int());
  auto *Func1 = T->def_func_declare("func1", C.type_int(), {X});
  Func1->get_body()->stmt_return(C.expr_var(X));
  T->def_func_define(Func1);
  EXPECT_EQ(F.to_string(), "int func1(int x);\n"
                           "int func1(int x){return x;}\n\n");
}
