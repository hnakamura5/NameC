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
  set(DOXYGEN_LAYOUT_FILE "${PROJECT_SOURCE_DIR}/doc/DoxygenLayout.xml")
  set(DOXYGEN_GENERATE_TREEVIEW "YES")

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
