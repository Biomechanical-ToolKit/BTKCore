FILE(READ "${MOKKA_HELP}/index.html" MOKKA_INDEX_IN)
STRING(REPLACE "<div id=\"side-nav\" class=\"ui-resizable side-nav-resizable\">" "<div id=\"side-nav\" class=\"ui-resizable side-nav-resizable invisible\">" MOKKA_INDEX_OUT "${MOKKA_INDEX_IN}")
STRING(REPLACE "<div class=\"title\">Mokka Documentation </div>" "<div class=\"title invisible\">Mokka Documentation </div>" MOKKA_INDEX_OUT "${MOKKA_INDEX_OUT}")
FILE(WRITE "${MOKKA_HELP}/index.html" "${MOKKA_INDEX_OUT}")