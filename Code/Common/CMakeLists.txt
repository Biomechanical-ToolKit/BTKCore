SET(BTKCommon_SRCS
  btkAcquisition.cpp
  btkDataObject.cpp
  btkMarker.cpp
  btkMetaDataEntry.cpp
  btkMetaDataEntryValue.cpp
  btkObject.cpp
  btkProcessObject.cpp  
)

ADD_LIBRARY(BTKCommon ${BTK_COMMON_BUILD_TYPE} ${BTKCommon_SRCS})
