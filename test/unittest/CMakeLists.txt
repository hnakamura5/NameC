enable_testing()
include(GoogleTest)

macro (define_gen_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name}
    GTest::gtest_main
    NameC_Gen
  )
  target_link_libraries(${name} basic_configs)
  gtest_discover_tests(${name})
endmacro()

define_gen_test(ExprTest)
define_gen_test(StmtTest)
define_gen_test(TypeTest)
define_gen_test(DirectiveTest)
define_gen_test(FileTest)
