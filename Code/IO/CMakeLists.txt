SET(BTKIO_SRCS
  btkAcquisitionFileIO.cpp
  btkAcquisitionFileIOFactory.cpp
  btkAcquisitionFileReader.cpp
  btkBinaryFileStream.cpp
  btkC3DFileIO.cpp
)

ADD_LIBRARY(BTKIO ${BTKIO_SRCS})
