enable_testing()
include(GoogleTest)

macro (define_gen_test dir name)
  add_executable(${dir}_${name} ${dir}/${name}.cpp)
  target_link_libraries(${dir}_${name}
    GTest::gtest_main
    NameC
  )
  target_link_libraries(${dir}_${name} basic_configs)
  gtest_discover_tests(${dir}_${name})
endmacro()

define_gen_test(Gen ExprTest)
define_gen_test(Gen StmtTest)
define_gen_test(Gen TypeTest)
define_gen_test(Gen DirectiveTest)
define_gen_test(Gen FileTest)

define_gen_test(GenCXX ExprTest)
define_gen_test(GenCXX StmtTest)
define_gen_test(GenCXX TypeTest)
define_gen_test(GenCXX DirectiveTest)
define_gen_test(GenCXX FileTest)

