FILE(READ "${BTK_BINARY_DIR}/Documentation/Readme/index.html" README_IN)
STRING(REPLACE "index.html" "Readme.html" README_OUT "${README_IN}")
FILE(WRITE "${BTK_SOURCE_DIR}/Readme.html" "${README_OUT}")

