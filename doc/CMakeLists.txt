# https://cmake.org/cmake/help/latest/module/FindDoxygen.html
find_package(Doxygen
             OPTIONAL_COMPONENTS dot) # TODO: Really require dot?

# Only skip building document when doxygen is not found.
if (DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc/")
  set(DOXYGEN_FULL_PATH_NAMES "YES")
  set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}")
  set(DOXYGEN_STRIP_FROM_INC_PATH "${PROJECT_SOURCE_DIR}")
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
# We do not require graph
  #set(DOXYGEN_HAVE_DOT "NO")
  #set(DOXYGEN_CALL_GRAPH "NO")
  #set(DOXYGEN_CALLER_GRAPH "NO")
  #set(DOXYGEN_EXTRACT_ALL "NO")

  doxygen_add_docs(doxygen_doc
      "${PROJECT_SOURCE_DIR}/README.md"
      "${PROJECT_SOURCE_DIR}/src"
      "${PROJECT_SOURCE_DIR}/include"
      ALL
      COMMENT "Generating doxygen document"
  )

  install(DIRECTORY "${CMAKE_BINARY_DIR}/doc/html/"
      DESTINATION "doc")
else()
  message(WARNING "Doxygen not found, skip generating document.")
endif()

