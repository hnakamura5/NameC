add_library(NameC_Gen STATIC)

target_sources(NameC_Gen PRIVATE
    Gen.cpp
    Gen/Exprs.cpp
    Gen/Stmts.cpp
    Gen/Types.cpp
    Gen/File.cpp
    Gen/Decl.cpp
    Gen/Scope.cpp
    Gen/Mixins.cpp
    Gen/Context.cpp
    Gen/Directive.cpp

    GenCXX.cpp
    GenCXX/CXXTypes.cpp
    GenCXX/CXXExprs.cpp
    GenCXX/CXXStmts.cpp
    GenCXX/CXXDecl.cpp
    GenCXX/CXXContext.cpp
)

target_link_libraries(NameC_Gen PRIVATE basic_configs)

install(TARGETS NameC_Gen RUNTIME)
