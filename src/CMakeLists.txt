add_library(NameC STATIC)

target_sources(NameC PRIVATE
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
    GenCXX/CXXFile.cpp
    GenCXX/CXXScope.cpp
    GenCXX/CXXDirective.cpp
    GenCXX/CXXMixins.cpp
    GenCXX/CXXCommon.cpp
)

target_link_libraries(NameC PRIVATE basic_configs)

