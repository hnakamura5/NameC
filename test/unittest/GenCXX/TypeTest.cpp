#include "NameCXX.h"
#include <gtest/gtest.h>

using namespace namecxx;

Context C;

TEST(TypeTest, RawTypeTest) {
  auto *T = C.type_raw("raw_type");
  EXPECT_EQ(T->to_string(), "raw_type");
}

TEST(TypeTest, EmitAndToString) {
  auto *T = C.type_raw("raw_type");
  std::stringstream SS;
  T->emit(SS);
  EXPECT_EQ(SS.str(), T->to_string());
}

TEST(TypeTest, VoidTypeTest) {
  auto *T = C.type_void();
  EXPECT_EQ(T->to_string(), "void");
}

TEST(TypeTest, NamedTypeTest) {
  auto *T = C.type_name(new ClassDecl(C.type_class("class_type", {}), false));
  EXPECT_EQ(T->to_string(), "class_type");
}

TEST(TypeTest, TypeAliasTest) {
  auto *T = C.type_typedef("alias_type", C.type_int());
  EXPECT_EQ(T->to_string(), "alias_type");
}

TEST(TypeTest, PointerTest) {
  auto *T = C.type_ptr(C.type_int());
  EXPECT_EQ(T->to_string(), "int*");
}

TEST(TypeTest, ArrayTest) {
  auto *T = C.type_array(C.type_int(), {C.expr_int(1), C.expr_int(2)});
  EXPECT_EQ(T->to_string(), "int[1][2]");
}

TEST(TypeTest, FunctionTest) {
  auto *T = C.type_func(C.type_int(), {C.type_int(), C.type_float()});
  EXPECT_EQ(T->to_string(), "int(int,float)");
}

TEST(TypeTest, ClassTest) {
  auto *T =
      C.type_class("class_type", {{AccessSpec::Public, C.type_raw("base")}});
  auto *S1 = T->add_private_scope();
  auto *Method1 = S1->def_method("method1", C.type_int(),
                                 {C.decl_var("var1", C.type_int())})
                      ->get_or_add_body();
  Method1->stmt_return(C.expr_raw("var1"));
  auto *Field1 = S1->def_field("field1", C.type_int(), C.expr_int(1));
  EXPECT_EQ(T->to_string(), "class class_type : public base{"
                            "private:\n"
                            "int method1(int var1){return var1;}\n"
                            "int field1=1;\n"
                            "}");
}

TEST(TypeTest, UnionTest) {
  auto *T = C.type_union("union_type");
  auto *S = T->add_public_scope();
  S->def_field("member1", C.type_int());
  S->def_field_template("member2", {C.decl_var("T", C.type_type_name())},
                        C.type_raw("T"));
  EXPECT_EQ(T->to_string(), "union union_type{"
                            "public:\n"
                            "int member1;\n"
                            "template<typename T> T member2;\n"
                            "}");
}

TEST(TypeTest, EnumTest) {
  auto *T = C.type_enum("enum_type");
  T->def_member("member1", C.expr_int(1));
  T->def_member("member2", C.expr_int(2));
  EXPECT_EQ(T->to_string(), "enum enum_type{member1=1,member2=2,}");
}
