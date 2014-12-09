# Eigen
INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/ThirdParties/eigen3/Eigen
        INCLUDES DESTINATION include
        COMPONENT dev
        PATTERN ".svn" EXCLUDE
        PATTERN ".DS_Store" EXCLUDE
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN ".cpp" EXCLUDE)
# Eigen_unsupported
INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/ThirdParties/eigen3/Eigen_unsupported
        INCLUDES DESTINATION include
        COMPONENT dev
        PATTERN ".svn" EXCLUDE
        PATTERN ".DS_Store" EXCLUDE
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN ".cpp" EXCLUDE)
# btkEigen
INSTALL(DIRECTORY ${BTK_SOURCE_DIR}/ThirdParties/eigen3/btkEigen
        INCLUDES DESTINATION include
        COMPONENT dev
        PATTERN ".svn" EXCLUDE
        PATTERN ".DS_Store" EXCLUDE)