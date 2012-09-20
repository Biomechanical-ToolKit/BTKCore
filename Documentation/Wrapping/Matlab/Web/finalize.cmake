# Functions
FILE(GLOB files "${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/classbtk*.html")
FOREACH(f ${files})
  FILE(READ "${f}" IN)
  STRING(REPLACE " Class Reference</h1>" "<h1>" OUT "${IN}")
  STRING(REPLACE "<table class=\"memberdecls\">
</table>
<hr/><a name=\"_details\"></a><h2>Description</h2>" "" OUT "${OUT}")
  FILE(WRITE "${f}" "${OUT}")
ENDFOREACH(f)

# Groups
FILE(GLOB files "${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/group__*.html")
FOREACH(f ${files})
  FILE(READ "${f}" IN)
  STRING(REPLACE "Classes" "Functions" OUT "${IN}")
  STRING(REPLACE "<hr/><a name=\"_details\"></a><h2>Description</h2>" "" OUT "${OUT}")
  STRING(REPLACE "class &nbsp;" "&nbsp;" OUT "${OUT}")
  FILE(WRITE "${f}" "${OUT}")
ENDFOREACH(f)

# Class List -> Function List
FILE(READ "${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/annotated.html" IN)
STRING(REPLACE "<h1>Class List</h1>" "<h1>Function List</h1>" OUT "${IN}")
STRING(REPLACE "Here are the classes, structs, unions and interfaces with brief descriptions:" "Here are the function with brief descriptions:" OUT "${OUT}")
STRING(REPLACE "<img src=\"ftv2cl.png\" alt=\"C\" width=\"24\" height=\"22\" />" "" OUT "${OUT}")
FILE(WRITE "${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/annotated.html" "${OUT}")

# Class Index -> Function Index
FILE(READ "${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/classes.html" IN)
STRING(REPLACE "<h1>Class Index</h1>" "<h1>Function Index</h1>" OUT "${IN}")
FILE(WRITE "${BTK_BINARY_DIR}/Documentation/Wrapping/Matlab/classes.html" "${OUT}")

