PROJECT(OracleReader)
FIND_PACKAGE(ParaView REQUIRED)
INCLUDE(${PARAVIEW_USE_FILE})

ADD_PARAVIEW_PLUGIN(OracleReaderClientPlugin "1.0"
	GUI_RESOURCES OracleReader.qrc)

ADD_PARAVIEW_PLUGIN(OracleReaderSMPlugin "1.0"
	SERVER_MANAGER_XML OracleReader.xml
	SERVER_MANAGER_SOURCES vtkOracleReader.cpp)
	
INCLUDE_DIRECTORIES(../../common)

set(OCCI_INCLUDE_DIR "C:/"
CACHE PATH "Select the OCCI include directory")
INCLUDE_DIRECTORIES(${OCCI_INCLUDE_DIR})

set(OCCI_LIB_DEBUG "C:/"
CACHE PATH "Select the OCCI debug lib")
LINK_DIRECTORIES(${OCCI_LIB_DEBUG})

set(OCCI_LIB_RELEASE "C:/"
CACHE PATH "Select the OCCI release lib")
LINK_DIRECTORIES(${OCCI_LIB_RELEASE})

target_link_libraries (OracleReaderSMPlugin ${OCCI_LIB_RELEASE} )	

target_link_libraries (OracleReaderSMPlugin ${OCCI_LIB_DEBUG} )	