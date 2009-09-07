SET(AcquisitionConverter_SRCS
  main.cpp
  )

ADD_EXECUTABLE(AcquisitionConverter ${AcquisitionConverter_SRCS})
TARGET_LINK_LIBRARIES(AcquisitionConverter BTKIO)

