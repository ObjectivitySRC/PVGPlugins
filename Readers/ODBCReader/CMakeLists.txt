PROJECT(ODBCReader)
FIND_PACKAGE(ParaView REQUIRED)
INCLUDE(${PARAVIEW_USE_FILE})
	
QT4_WRAP_CPP(MOC_SRCS pqODBCReader.h) 

ADD_PARAVIEW_OBJECT_PANEL(IFACES IFACE_SRCS CLASS_NAME pqODBCReader
                         XML_NAME ODBCReader XML_GROUP sources)
	
	# create a plugin for this panel
ADD_PARAVIEW_PLUGIN(GUIODBCReader "1.0" 
                    GUI_INTERFACES ${IFACES} 
                    GUI_RESOURCES pqUI.qrc
                    SOURCES pqODBCReader.cpp ${MOC_SRCS} ${IFACE_SRCS})
                    
ADD_PARAVIEW_PLUGIN(ODBCReaderClientPlugin "1.0"
	GUI_RESOURCES pqODBCReader.qrc)

ADD_PARAVIEW_PLUGIN(ODBCReaderServerManagerPlugin "1.0"
	SERVER_MANAGER_XML ODBCReader.xml
	SERVER_MANAGER_SOURCES vtkODBCReader.cpp)

target_link_libraries(ODBCReaderServerManagerPlugin)