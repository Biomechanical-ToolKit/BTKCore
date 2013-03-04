IF(NOT BTK_INSTALL_NO_DEVELOPMENT)
  # Eigen
  INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/Utilities/eigen3/Eigen
          DESTINATION ${BTK_INSTALL_INCLUDE_DIR_CM24}/Utilities
          COMPONENT Development
          PATTERN ".svn" EXCLUDE
          PATTERN ".DS_Store" EXCLUDE
          PATTERN "CMakeLists.txt" EXCLUDE
          PATTERN ".cpp" EXCLUDE)
  # btkEigen
  INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/Utilities/eigen3/btkEigen
          DESTINATION ${BTK_INSTALL_INCLUDE_DIR_CM24}/Utilities
          COMPONENT Development
          PATTERN ".svn" EXCLUDE
          PATTERN ".DS_Store" EXCLUDE)
ENDIF(NOT BTK_INSTALL_NO_DEVELOPMENT)