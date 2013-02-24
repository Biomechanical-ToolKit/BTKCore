# Adapt some part of the HTML files describing the classes.
FILE(GLOB files "${BTK_BINARY_DIR}/Documentation/Wrapping/Python/classbtk*.html")

FOREACH(f ${files})
  # Read the file
  FILE(READ "${f}" IN)
  # Replace the multi-line arguments (self, args) by one line
  STRING(REPLACE 
    "<td>(</td>
          <td class=\"paramtype\">&#160;</td>
          <td class=\"paramname\"><em>self</em>, </td>
        </tr>
        <tr>
          <td class=\"paramkey\"></td>
          <td></td>
          <td class=\"paramtype\">&#160;</td>
          <td class=\"paramname\"><em>args</em>&#160;</td>
        </tr>
        <tr>
          <td></td>
          <td>)</td>
          <td></td><td></td>"
    "<td>(</td>
          <td class=\"paramtype\">&nbsp;</td>
          <td class=\"paramname\"> <em>self</em>, </td>
          <td>&nbsp;</td>
          <td class=\"paramname\"> <em>args</em></td><td>&nbsp;</td>
          <td>)</td>"
    OUT "${IN}")
  # Add a space between 'self' and the parenthesis for methods with a single argment (to be the same than with tow arguments)
  STRING(REPLACE 
     "<td class=\"paramname\"><em>self</em></td><td>)</td>"
     "<td class=\"paramname\"><em>self</em></td><td>&nbsp;</td><td>)</td>"
     OUT "${OUT}")
  STRING(REPLACE "${brief}" "" OUT "${OUT}")
  # Rewrite the file
  FILE(WRITE "${f}" "${OUT}")
ENDFOREACH(f)

# Adapt the content of the file "namespacebtk.html" to remove the class btk::_object and btk::SwigPyIterator
SET(f "${BTK_BINARY_DIR}/Documentation/Wrapping/Python/namespacebtk.html")
FILE(READ "${f}" IN)
STRING(REPLACE "<tr class=\"memitem:\"><td class=\"memItemLeft\" align=\"right\" valign=\"top\">class &#160;</td><td class=\"memItemRight\" valign=\"bottom\"><b>_object</b></td></tr>
<tr class=\"memdesc:\"><td class=\"mdescLeft\">&#160;</td><td class=\"mdescRight\">Abstract interface for the Python wrapping of BTK. <br/></td></tr>
<tr class=\"separator:\"><td class=\"memSeparator\" colspan=\"2\">&#160;</td></tr>
<tr class=\"memitem:\"><td class=\"memItemLeft\" align=\"right\" valign=\"top\">class &#160;</td><td class=\"memItemRight\" valign=\"bottom\"><b>SwigPyIterator</b></td></tr>
<tr class=\"separator:\"><td class=\"memSeparator\" colspan=\"2\">&#160;</td></tr>"
"" OUT "${IN}")
FILE(WRITE "${f}" "${OUT}")