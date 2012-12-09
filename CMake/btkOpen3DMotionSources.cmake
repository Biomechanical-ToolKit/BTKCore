# The source code is cut in several parts to be able to create some modules
# or to keep only the minimum required number of files into BTK to support Codamotion file formats

# Note: The files commented are not included as they have no any symbol inside

# There are some options to compile Open3DMotion:
#  - USE_O3DM_NO_FILE_FORMAT_REGISTRATION: Special BTK option to remove the part of the code which register all the know file formats.
#  - USE_O3DM_MDF_FILE_FORMAT: Include the MDF file format
#  - USE_O3DM_XMOVE_FILE_FORMAT: Include the XMOVE file format

SET(BTK_O3DM_ROOT "${BTK_SOURCE_DIR}/Utilities/Open3DMotion/src")
SET(BTK_O3DM_INCLUDE_DIR "${BTK_O3DM_ROOT}")

ADD_DEFINITIONS(-DOPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)

# CORE
# ----
SET(BTK_O3DM_OpenORM_CORE_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Branches/TreeCompound.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Branches/TreeCompoundNode.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Branches/TreeList.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/ORMIOException.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/MemoryHandler.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/MemoryHandlerNull.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/TreeBinary.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/TreeBool.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/TreeFloat64.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/TreeInt32.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/TreeSimpleValue.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Leaves/TreeString.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapArrayCompound.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapArrayFloat64.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapArrayInt32.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapArraySimpleValue.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapBinary.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapBool.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapCompound.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapCompoundElement.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapCompoundRefMap.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapElement.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapFloat64.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapInt32.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapOptional.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapSimpleValue.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapString.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/MapVectorXYZ.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/RichBinary/BinaryFieldSpec.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/RichBinary/BinaryStructure.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/RichBinary/BinIter.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactory.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/RichBinary/NoSuchFieldException.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/Mappings/RichBinary/RichBinary.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/TreeValue.cpp")
    
SET(BTK_O3DM_Biomechanics_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/EventArray.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/EventGroup.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/EventIDTime.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/EventNameID.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/ForcePlate.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/Gait/GaitEvents.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/TimeRange.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/TimeSequence.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/Trial.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Biomechanics/Trial/TSFactory.cpp")
    
SET(BTK_O3DM_MotionFile_CORE_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/FileFormatOptions.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/MotionFileException.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/MotionFileFormat.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/MotionFileHandler.cpp")
    
IF(USE_O3DM_NO_FILE_FORMAT_REGISTRATION)
  SET(BTK_O3DM_MotionFile_CORE_SRCS
      ${BTK_O3DM_MotionFile_CORE_SRCS}
      "${BTK_O3DM_ROOT}/btkOpen3DMotion/MotionFileFormatList_Register.cpp")
ELSE(USE_O3DM_NO_FILE_FORMAT_REGISTRATION)
  SET(BTK_O3DM_MotionFile_CORE_SRCS
      ${BTK_O3DM_MotionFile_CORE_SRCS}
      "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Register.cpp")
ENDIF(USE_O3DM_NO_FILE_FORMAT_REGISTRATION)
        
# C3D File format
# ---------------
SET(BTK_O3DM_MotionFile_FORMAT_C3D_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/C3D/C3DMachine.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/C3D/C3DRecord.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/C3D/C3DTSFactory.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/C3D/FileFormatC3D.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/C3D/FileFormatOptionsC3D.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/C3D/MapC3DParameters.cpp")
    
# MDF File format
# ---------------
SET(BTK_O3DM_MotionFile_FORMAT_MDF_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/MDF/FileFormatMDF.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/MDF/FileFormatOptionsMDF.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/MDF/MDFDescriptor.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/MDF/ForcePlateMDF.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/RigidTransform3.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/LinearSolve3.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/MathsException.cpp")
    
# XMOVE File format
# -----------------
SET(BTK_O3DM_MotionFile_FORMAT_XMOVE_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/XMove/FileFormatOptionsXMove.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionFile/Formats/XMove/FileFormatXMove.cpp")
    
SET(BTK_O3DM_OpenORM_IO_XML_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/ReadWriteXML.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/ReadWriteXMLBinary.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/ReadWriteXMLBool.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/ReadWriteXMLCompound.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/ReadWriteXMLFloat64.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/ReadWriteXMLInt32.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/ReadWriteXMLList.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/ReadWriteXMLString.cpp"
    # "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/XMLReadException.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/XMLReadingMachine.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/XMLReadWriteMachine.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/XML/XMLWritingMachine.cpp")
    
# MOBL File format (Motion Bundle)
# --------------------------------
SET(BTK_O3DM_MotionBundle_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionBundle/MOBL.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/MotionBundle/MotionBundleHandler.cpp")
    
SET(BTK_O3DM_OpenORM_IO_BSON_SRCS
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/BSON/BSONGZStreamReader.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/BSON/BSONObjectIdHolder.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/BSON/BSONReader.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/BSON/BSONReadException.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/BSON/BSONStreamReader.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/OpenORM/IO/BSON/BSONTimestampHolder.cpp")

# Maths
# -----
SET(BTK_O3DM_Maths_SRCS    
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/LinearSolve3.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/MathsException.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/Matrix.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/Matrix3x3.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/RigidTransform3.cpp"
    "${BTK_O3DM_ROOT}/Open3DMotion/Maths/Vector3.cpp")
    
# Construction of the sources to compile
SET(BTK_O3DM_SRCS
    ${BTK_O3DM_OpenORM_CORE_SRCS}
    ${BTK_O3DM_Biomechanics_SRCS}
    ${BTK_O3DM_MotionFile_CORE_SRCS})
    
IF(USE_O3DM_MDF_FILE_FORMAT)
  SET(BTK_O3DM_SRCS
      ${BTK_O3DM_SRCS}
      ${BTK_O3DM_MotionFile_FORMAT_MDF_SRCS})
ENDIF(USE_O3DM_MDF_FILE_FORMAT)

IF(USE_O3DM_XMOVE_FILE_FORMAT)
  SET(BTK_O3DM_SRCS
      ${BTK_O3DM_SRCS}
      ${BTK_O3DM_MotionFile_FORMAT_XMOVE_SRCS}
      ${BTK_O3DM_OpenORM_IO_XML_SRCS})
ENDIF(USE_O3DM_XMOVE_FILE_FORMAT)
        