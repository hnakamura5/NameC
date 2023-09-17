#include "NameC.h"
#include <gtest/gtest.h>

using namespace namec;

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
  auto *T = C.type_name(new StructDecl(C.type_struct("struct_type")));
  EXPECT_EQ(T->to_string(), "struct struct_type");
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

TEST(TypeTest, StructTest) {
  auto *T = C.type_struct("struct_type");
  T->def_member("member1", C.type_int());
  T->def_member("member2", C.type_float());
  EXPECT_EQ(T->to_string(), "struct struct_type{int member1;float member2;}");
}

TEST(TypeTest, UnionTest) {
  auto *T = C.type_union("union_type");
  T->def_member("member1", C.type_int());
  T->def_member("member2", C.type_float());
  EXPECT_EQ(T->to_string(), "union union_type{int member1;float member2;}");
}

TEST(TypeTest, EnumTest) {
  auto *T = C.type_enum("enum_type");
  T->def_member("member1", C.expr_int(1));
  T->def_member("member2", C.expr_int(2));
  EXPECT_EQ(T->to_string(), "enum enum_type{member1=1,member2=2,}");
}
