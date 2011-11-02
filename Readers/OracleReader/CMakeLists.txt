PROJECT(OracleReader)
FIND_PACKAGE(ParaView REQUIRED)
INCLUDE(${PARAVIEW_USE_FILE})

ADD_PARAVIEW_PLUGIN(OracleReaderClientPlugin "1.0"
	GUI_RESOURCES OracleReader.qrc)

ADD_PARAVIEW_PLUGIN(OracleReaderSMPlugin "1.0"
	SERVER_MANAGER_XML OracleReader.xml
	SERVER_MANAGER_SOURCES vtkOracleReader.cpp)
	
INCLUDE_DIRECTORIES(../../common)

set(ORACLE_INCLUDE_DIR "C:/"
CACHE PATH "Select the ORACLE include directory")
INCLUDE_DIRECTORIES(${ORACLE_INCLUDE_DIR})

set(ORACLE_LIB "C:/"
CACHE PATH "Select the ORACLE lib")

target_link_libraries (OracleReaderSMPlugin ${ORACLE_LIB} )