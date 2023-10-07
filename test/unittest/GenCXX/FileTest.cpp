#include "NameCXX.h"
#include <gtest/gtest.h>

#include <fstream>
#include <string>

using namespace namecxx;

Context C;

TEST(FileTest, FileOutPut) {
  CXXFile F(C);
  auto *T = F.get_first_top_level();
  auto *Main = T->def_func("main", C.type_int(), {});
  auto *S = Main->get_or_add_body();
  S->stmt_return(C.expr_int(0));
  EXPECT_EQ(F.to_string(), "int main(){return 0;}\n\n");
  std::filesystem::path P = "test.c";
  F.emit_to_file(P);
  std::ifstream IS(P);
  std::string Str;
  std::getline(IS, Str);
  EXPECT_EQ(Str, "int main(){return 0;}");
  std::getline(IS, Str);
  EXPECT_EQ(Str, "");
  std::getline(IS, Str);
  EXPECT_EQ(Str, "");
  EXPECT_TRUE(IS.eof());
  IS.close();
  std::filesystem::remove(P);
}

TEST(FileTest, VarArgFunc) {
  CXXFile F(C);
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

TEST(FileTest, TemplateInstantiations) {
  CXXFile F(C);
  auto *T = F.get_first_top_level();
  auto *TV = C.decl_var("T", C.type_type_name());
  auto *Arg = C.decl_var("x", C.type_raw("T"));
  auto *TCD = T->def_class_template("cls", {TV},
                                    {{AccessSpec::Public, C.type_raw("T")}});
  auto *TC = TCD->get_decl()->get_class();
  TC->add_private_scope()->def_field("field1", C.type_raw("T"));
  auto *Ctor = TC->add_public_scope()->def_ctor({Arg});
  Ctor->add_init("field1", C.expr_var(Arg));
  Ctor->get_or_add_body();
  auto *TF =
      T->def_func_template("func", {TV, C.decl_var("I", C.type_int())},
                           C.type_instantiate(TCD, {C.type_var(TV)}), {Arg})
          ->get_decl()
          ->get_or_add_body();
  TF->stmt_return(C.expr_call(
      C.expr_raw_instantiate(C.QN(TCD), {C.type_var(TV)}), {C.expr_var(Arg)}));
  // TODO: QualName resolution?
  EXPECT_EQ(F.to_string(), "template<typename T> "
                           "class cls : public T{"
                           "private:\n"
                           "T field1;\n"
                           "public:\n"
                           "cls(T x):field1(x){}\n"
                           "};\n"
                           "template<typename T,int I> "
                           "cls<T> func(T x){return cls<T>(x);}\n\n");
}
