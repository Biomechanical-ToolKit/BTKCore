SET(BTK_MINIZIP_INCLUDE_DIR
    "${BTK_SOURCE_DIR}/Utilities/zlib"
    "${BTK_SOURCE_DIR}/Utilities/zlib/contrib/minizip"
    )
    
SET(BTK_MINIZIP_SRCS
    # Original files in zlib
    "${BTK_SOURCE_DIR}/Utilities/zlib/adler32.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/compress.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/crc32.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/deflate.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/inflate.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/infback.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/inftrees.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/inffast.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/trees.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/uncompr.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/zutil.c"
    # Modifed files to use only functions required by minizip
    "${BTK_SOURCE_DIR}/Utilities/zlib/btk_gzflags.c"
    # Original files in minizip
    "${BTK_SOURCE_DIR}/Utilities/zlib/contrib/minizip/ioapi.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/contrib/minizip/unzip.c"
    "${BTK_SOURCE_DIR}/Utilities/zlib/contrib/minizip/zip.c"
    )

# File to support directly a QIODevice with the unzip functions
IF(QT4_FOUND)
  SET(BTK_MINIZIP_SRCS
      ${BTK_MINIZIP_SRCS}
      "${BTK_SOURCE_DIR}/Utilities/zlib/contrib/minizip/btk_ioapi_qiodevice.cpp"
      )
ENDIF(QT4_FOUND)