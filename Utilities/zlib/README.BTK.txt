This is a modified zlib (http://zlib.net/) library where only the files required for minizip are included

12/08/10 - Support of the use of Qt QIOdevice added into Minizip
            - Use the function fill_fopen(64)_filefunc for that and cast the QIODevice to a
              const chart* or const void* variable...

12/04/10 - Initial package from zlib 1.2.6
            - The gz* files were removed as they were not necessary for minizip
            - Only the function gzflags was needed and added in the file btk_gzflags.c