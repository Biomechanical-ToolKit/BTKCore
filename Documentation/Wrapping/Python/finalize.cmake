# Functions
FILE(GLOB files "${BTK_BINARY_DIR}/Documentation/Wrapping/Python/classbtk*.html")
FOREACH(f ${files})
  FILE(READ "${f}" IN)
  STRING(REPLACE 
    "<td>(</td>
          <td class=\"paramtype\">&nbsp;</td>
          <td class=\"paramname\"> <em>self</em>, </td>
        </tr>
        <tr>
          <td class=\"paramkey\"></td>
          <td></td>
          <td class=\"paramtype\">&nbsp;</td>
          <td class=\"paramname\"> <em>args</em></td><td>&nbsp;</td>
        </tr>
        <tr>
          <td></td>
          <td>)</td>
          <td></td><td></td><td></td>"
    "<td>(</td>
          <td class=\"paramtype\">&nbsp;</td>
          <td class=\"paramname\"> <em>self</em>, </td>
          <td>&nbsp;</td>
          <td class=\"paramname\"> <em>args</em></td><td>&nbsp;</td>
          <td>)</td>"
    OUT "${IN}")
  FILE(WRITE "${f}" "${OUT}")
ENDFOREACH(f)

