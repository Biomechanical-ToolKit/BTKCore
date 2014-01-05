FIND_PACKAGE(PythonInterp REQUIRED QUIET)

MESSAGE(STATUS "Packaging Python Binding...")

SET(BTK_PYTHON_PACKAGE_SOURCE_DIR "${BTK_SOURCE_DIR}/Packaging/Python/setuptools")
SET(BTK_PYTHON_PACKAGE_BINARY_DIR "${BTK_BINARY_DIR}/Packaging/Python")

SET(BTK_PYTHON_PACKAGE_LONG_DESCRIPTION "Python binding for the C++ library Biomechanical ToolKit (BTK) using SWIG 2.x. This package will allow you to read, modify, process, and write acquisition data recorded by a motion capture system (BTS Bioengineering, Codamotion, Motion Analysis, Vicon, etc).")

INCLUDE(GetPrerequisites)
GET_PREREQUISITES("${BTK_SWIG_PYTHON_MODULE_LOCATION}" BTK_PACKAGE_DEPENDENCY 1 0 "" "")

# Feed the variable BTK_PYTHON_PACKAGE_DATA
# And generate the file MANIFEST.in
GET_FILENAME_COMPONENT(BTK_PYTHON_PACKAGE_DATA ${BTK_SWIG_PYTHON_MODULE_LOCATION} NAME)
FILE(WRITE "${BTK_PYTHON_PACKAGE_BINARY_DIR}/MANIFEST.in" "include btk/${BTK_PYTHON_PACKAGE_DATA}\n")
SET(BTK_PYTHON_PACKAGE_DATA "'${BTK_PYTHON_PACKAGE_DATA}'")
FOREACH(dep ${BTK_PACKAGE_DEPENDENCY})
  GET_FILENAME_COMPONENT(_NAME ${dep} NAME)
  SET(BTK_PYTHON_PACKAGE_DATA "${BTK_PYTHON_PACKAGE_DATA},'${_NAME}'")
  FILE(APPEND "${BTK_PYTHON_PACKAGE_BINARY_DIR}/MANIFEST.in" "include btk/${_NAME}\n")
ENDFOREACH(dep)

# setuptools configuration files
MESSAGE(STATUS " -> Configuring setuptools")
CONFIGURE_FILE(${BTK_PYTHON_PACKAGE_SOURCE_DIR}/setup.py.in 
               ${BTK_PYTHON_PACKAGE_BINARY_DIR}/setup.py @ONLY IMMEDIATE)
FILE(COPY ${BTK_PYTHON_PACKAGE_SOURCE_DIR}/setup.cfg DESTINATION ${BTK_PYTHON_PACKAGE_BINARY_DIR})
FILE(WRITE "${BTK_PYTHON_PACKAGE_BINARY_DIR}/README" ${BTK_PYTHON_PACKAGE_LONG_DESCRIPTION})

# Package files
MESSAGE(STATUS " -> Copying package files and dependencies")
FILE(MAKE_DIRECTORY "${BTK_PYTHON_PACKAGE_BINARY_DIR}/btk")
FILE(COPY "${BTK_BINARY_DIR}/bin/btk.py" DESTINATION "${BTK_PYTHON_PACKAGE_BINARY_DIR}/btk")
FILE(RENAME "${BTK_PYTHON_PACKAGE_BINARY_DIR}/btk/btk.py" "${BTK_PYTHON_PACKAGE_BINARY_DIR}/btk/__init__.py")
SET(libs ${BTK_SWIG_PYTHON_MODULE_LOCATION} ${BTK_PACKAGE_DEPENDENCY})
FOREACH(lib ${libs})
  GET_FILENAME_COMPONENT(_NAME ${lib} NAME)
  # The command FILE(COPY ...) cannot be used here as we want to copy the original file and not a symlink
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy "${lib}" "${BTK_PYTHON_PACKAGE_BINARY_DIR}/btk/${_NAME}")
ENDFOREACH(lib)

# Create package
IF(APPLE)
  # But first, fixup dependencies for MacOS X
  MESSAGE(STATUS " -> Fixing up dependencies")
  # - Create keys
  SET(changes "")
  FOREACH(dep ${BTK_PACKAGE_DEPENDENCY})
    GET_FILENAME_COMPONENT(key ${dep} NAME_WE)
    SET(${key}_ITEM "${dep}")
    STRING(REPLACE "${BTK_BINARY_DIR}/bin" "@loader_path" ${key}_RESOLVED_ITEM "${dep}")
  ENDFOREACH(dep)
  GET_FILENAME_COMPONENT(BTK_SWING_PYTHON_MODULE_FILENAME ${BTK_SWIG_PYTHON_MODULE_LOCATION} NAME)
  # - Edit dynamic links
  FOREACH(lib ${libs})
    GET_FILENAME_COMPONENT(_NAME ${lib} NAME)
    GET_PREREQUISITES("${BTK_PYTHON_PACKAGE_BINARY_DIR}/btk/${_NAME}" _prs 1 0 "" "")
    IF(NOT "${_prs}" STREQUAL "")
      SET(changes "")
      FOREACH(_pr ${_prs})
        GET_FILENAME_COMPONENT(key ${_pr} NAME_WE)
        SET(changes ${changes} "-change" "${${key}_ITEM}" "${${key}_RESOLVED_ITEM}")
      ENDFOREACH(_pr ${_prs})
      EXECUTE_PROCESS(COMMAND install_name_tool ${changes} -id "@loader_path/${_NAME}" "${BTK_PYTHON_PACKAGE_BINARY_DIR}/btk/${_NAME}")
    ENDIF(NOT "${_prs}" STREQUAL "")
  ENDFOREACH(lib)
  # Create Package
  MESSAGE(STATUS " -> Create EGG package")
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} setup.py --quiet bdist_egg
                  WORKING_DIRECTORY ${BTK_PYTHON_PACKAGE_BINARY_DIR})
ELSEIF(UNIX)
  IF(ARCH_64_BIT)
    SET(RPM_ARCH "x86_64")
  ELSE(ARCH_64_BIT)
    SET(RPM_ARCH "i386")
  ENDIF(ARCH_64_BIT)
  # The next line force the installation to /usr/local/lib/pythonx.y/dist-packages
  # but the script linux-post-install.sh will move the installed files to an existing python
  # local site package path
  FILE(APPEND "${BTK_PYTHON_PACKAGE_BINARY_DIR}/setup.cfg" "\n[install]\ninstall-lib=/usr/local/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/dist-packages")
  CONFIGURE_FILE(${BTK_PYTHON_PACKAGE_SOURCE_DIR}/linux-post-install.sh.in 
                 ${BTK_PYTHON_PACKAGE_BINARY_DIR}/linux-post-install.sh @ONLY IMMEDIATE)
  CONFIGURE_FILE(${BTK_PYTHON_PACKAGE_SOURCE_DIR}/linux-pre-uninstall.sh.in 
                 ${BTK_PYTHON_PACKAGE_BINARY_DIR}/linux-pre-uninstall.sh @ONLY IMMEDIATE)
 CONFIGURE_FILE(${BTK_PYTHON_PACKAGE_SOURCE_DIR}/linux-post-uninstall.sh.in 
                ${BTK_PYTHON_PACKAGE_BINARY_DIR}/linux-post-uninstall.sh @ONLY IMMEDIATE)
  MESSAGE(STATUS " -> Create RPM package")
  EXECUTE_PROCESS(COMMAND bash -c "${PYTHON_EXECUTABLE} setup.py bdist_rpm --binary-only --force-arch ${RPM_ARCH} --post-install linux-post-install.sh --pre-uninstall linux-pre-uninstall.sh --post-uninstall linux-post-uninstall.sh >/dev/null 2>&1"
                  WORKING_DIRECTORY ${BTK_PYTHON_PACKAGE_BINARY_DIR})
  MESSAGE(STATUS " -> Create DEB package")
  EXECUTE_PROCESS(COMMAND bash "${BTK_PYTHON_PACKAGE_SOURCE_DIR}/../linux-create-deb-package.sh"
                  WORKING_DIRECTORY "${BTK_BINARY_DIR}/dist")
ELSEIF(WIN32)
  MESSAGE(STATUS " -> Create Windows installer")
  MESSAGE(STATUS "    WARNING: This installer cannot detect the dependencies.")
  MESSAGE(STATUS "             The Python package 'numpy' must be installed manually.")
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} setup.py --quiet bdist_wininst
                  WORKING_DIRECTORY ${BTK_PYTHON_PACKAGE_BINARY_DIR})
ELSE(APPLE)
  MESSAGE(WARNING "Unsupported platform to fixup dependencies (if any)")
ENDIF(APPLE)
