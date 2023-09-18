#include "NameC.h"
#include <gtest/gtest.h>

#include <fstream>
#include <string>

using namespace namec;

Context C;

TEST(FileTest, FileOutPut) {
  CFile F(C);
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
