PROJECT(PVGPlugins)
FIND_PACKAGE(ParaView REQUIRED)
INCLUDE(${PARAVIEW_USE_FILE})

##########################################################################
# version control
SET (PVGPlugins_VERSION_MAJOR 1)
SET (PVGPlugins_VERSION_MINOR 0)
SET (PVGPlugins_VERSION_PATCH 0)

SET(PVGPlugins_VERSION "${PVGPlugins_VERSION_MAJOR}.${PVGPlugins_VERSION_MINOR}")
SET(PVGPlugins_VERSION_FULL "${PVGPlugins_VERSION}.${PVGPlugins_VERSION_PATCH}")
##########################################################################



##########################################################################
# this variable can be used in the code and will contain the path
# of DRILLHOLES root directory. (src/..) 
SET(PVGPlugins_ROOT_DIR \"${CMAKE_SOURCE_DIR}/..\")

SET(PVGPlugins_INSTALL_BIN_DIR bin)
##########################################################################


##########################################################################
# Setup Qt
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtXml QtSql)
IF(NOT QT4_FOUND)
	MESSAGE(SEND_ERROR "Qt not found.  Please check the QT_QMAKE_EXECUTABLE variable.")
ELSE(NOT QT4_FOUND)
	# don't go greater than supported version    
  STRING(REGEX MATCH "^4\\.4\\.[0-9]+" qt_version4_4_tmp "${QTVERSION}")
	IF (NOT qt_version4_4_tmp)
	STRING(REGEX MATCH "^4\\.5\\.[0-9]+" qt_version4_5_tmp "${QTVERSION}")
	IF (NOT qt_version4_5_tmp)
	  STRING(REGEX MATCH "^4\\.6\\.[0-9]+" qt_version4_6_tmp "${QTVERSION}")
	  IF (NOT qt_version4_6_tmp)
		MESSAGE(SEND_ERROR "Qt ${QTVERSION} not supported.  Please use Qt 4.5 (you may need to clean your dirtied cache).")     		
	  ENDIF (NOT qt_version4_6_tmp) 
	ENDIF (NOT qt_version4_5_tmp)    
  ENDIF (NOT qt_version4_4_tmp)    
ENDIF(NOT QT4_FOUND)

# help CMake find debug QtUiTools on Windows
IF(WIN32 AND QT_QTUITOOLS_FOUND)
  IF(NOT QT_QTUITOOLS_LIBRARY_DEBUG MATCHES "QtUiToolsd")
	SET(MY_QT_QTUITOOLS_LIBRARY_DEBUG "${QT_LIBRARY_DIR}/QtUiToolsd.lib")
	IF(EXISTS "${MY_QT_QTUITOOLS_LIBRARY_DEBUG}")
	  SET(QT_QTUITOOLS_LIBRARY_DEBUG "${MY_QT_QTUITOOLS_LIBRARY_DEBUG}" CACHE FILEPATH "QtUiTools debug library" FORCE)
	  SET(QT_QTUITOOLS_LIBRARY optimized ${QT_QTUITOOLS_LIBRARY_RELEASE}
							   debug ${QT_QTUITOOLS_LIBRARY_DEBUG} CACHE FILEPATH "The Qt QtUiTools library" FORCE)
	ENDIF(EXISTS "${MY_QT_QTUITOOLS_LIBRARY_DEBUG}")
  ENDIF(NOT QT_QTUITOOLS_LIBRARY_DEBUG MATCHES "QtUiToolsd")
ENDIF(WIN32 AND QT_QTUITOOLS_FOUND)

# This ensures that the Qt includes dir etc are set properly.
# We don't need to find the Qt4 package or include the use file
# in any of the SUBDIRS in this directory.
SET(QT_USE_QTUITOOLS TRUE)
SET(QT_USE_QTXML TRUE)  # uitools depends on xml
SET(QT_USE_QTSQL TRUE) # we need sql drivers
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${QT_QTSQL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${QT_QTXML_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${QT_QTUITOOLS_INCLUDE_DIR})
##########################################################################


MACRO(paraview_build_optional_plugin name comment subdirectory default)
  OPTION(PARAVIEW_BUILD_PLUGIN_${name} "Build ${comment}" ${default})
  MARK_AS_ADVANCED(CLEAR PARAVIEW_BUILD_PLUGIN_${name})
  IF(PARAVIEW_BUILD_PLUGIN_${name})
    MESSAGE(STATUS "Plugin: ${comment} enabled")
    ADD_SUBDIRECTORY("${subdirectory}")
  ELSE(PARAVIEW_BUILD_PLUGIN_${name})
    MESSAGE(STATUS "Plugin: ${comment} disabled")
  ENDIF(PARAVIEW_BUILD_PLUGIN_${name})
ENDMACRO(paraview_build_optional_plugin)

#paraview_build_optional_plugin(
#    PVGFilters "ParaViewGeo-Filters" Filters ON)
#
#paraview_build_optional_plugin(
#    PVGReaders "ParaViewGeo-Readers" Readers ON)
#
#paraview_build_optional_plugin(
#    PVGWriters "ParaViewGeo-Writers" Writers ON)

OPTION(ParaViewGeo_Filters "Build ParaViewGeo Filters" ON)
OPTION(ParaViewGeo_Readers "Build ParaViewGeo Readers" ON)
OPTION(ParaViewGeo_Writers "Build ParaViewGeo Writers" ON)

IF (ParaViewGeo_Filters)
	ADD_SUBDIRECTORY(Filters)
ENDIF(ParaViewGeo_Filters)

IF (ParaViewGeo_Readers)
	ADD_SUBDIRECTORY(Readers)
ENDIF(ParaViewGeo_Readers)

IF(ParaViewGeo_Writers)
	ADD_SUBDIRECTORY(Writers)
ENDIF(ParaViewGeo_Writers)


#########################################################################
# the name that appears in the installation wizard
#set(CPACK_PACKAGE_INSTALL_DIRECTORY "ParaViewGeo Plugins ${PVGPlugins_VERSION_FULL}")


set(CPACK_PACKAGE_NAME "ParaViewGeo Plugins")
set(CPACK_PACKAGE_VENDOR "www.objectivity.ca")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ParaViewGeo Plugins")
set(CPACK_PACKAGE_VERSION ${PVGPlugins_VERSION_FULL})


set(CPACK_COMPONENTS_ALL PVGFilters PVGReaders PVGWriters)

include (CPack)
