PROJECT(PVGReaders)
INCLUDE(${PARAVIEW_USE_FILE})

FIND_PACKAGE(Qt4)
IF(QT4_FOUND)

INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
${VTK_INCLUDE_DIR}
${PARAVIEW_INCLUDE_DIRS}
${ParaView_SOURCE_DIR}/VTK/GUISupport/Qt
${pqComponents_SOURCE_DIR}
${pqComponents_BINARY_DIR}
${pqCore_SOURCE_DIR}
${pqCore_BINARY_DIR}
${QtWidgets_SOURCE_DIR}
${QtWidgets_BINARY_DIR}
${QtChart_SOURCE_DIR}
${QtChart_BINARY_DIR}
${QtTesting_SOURCE_DIR}
${QtTesting_BINARY_DIR}
${QtPython_SOURCE_DIR}
${QtPython_BINARY_DIR}
ArcMapReader
BlockModelReader
DataMineReader
DrillHoleReader
DXFReader
Ellipsoid
#EPSReader
FDEM
FLAC3DReader
3DECReader
GocadReader
GXFReader
Map3dBlockReader
#ODBCReader
PointSetReader
SGeMSReader
ShapeFileReader
SOTResult
TriReader
LineNetworkReader

../common
)

# Only include directories if on Windows platform
IF(PV_WIN32)
	INCLUDE_DIRECTORIES(
	VulcanReader
	MineSightReader
	GeoReferencedImage
	GeoReferencedImage/geotiff/include
	GeoReferencedImage/geotiff/lib
	)
ENDIF(PV_WIN32)

SET( PVGReaders_SM_SRCS
ArcMapReader/vtkArcMapReader.cxx
BlockModelReader/vtkBlockModelReader.cxx
DataMineReader/vtkDataMineDummyReader.cxx 
DataMineReader/vtkDataMineWireFrameReader.cxx 
DataMineReader/vtkDataMinePerimeterReader.cxx  
DataMineReader/vtkDataMinePointReader.cxx  
DataMineReader/vtkDataMineBlockReader.cxx 
DataMineReader/vtkDataMineDrillHoleReader.cxx  
DataMineReader/vtkDataMineReader.cxx 
DrillHoleReader/vtkDrillHoleReader.cxx
DXFReader/vtkDXFReader.cxx
Ellipsoid/vtkEllipsoid.cxx 
Ellipsoid/vtkElliptic.cxx
#EPSReader/vtkEPSReader.cxx
FDEM/vtkFDEMReader.cxx
FLAC3DReader/vtkFLAC3DReader.cpp
3DECReader/vtk3DECReader.cpp
GocadReader/vtkGocadReader.cxx 
GocadReader/vtkGocadAtomic.cxx 
GocadReader/vtkGocadTSurf.cxx 
GocadReader/vtkGocadPLine.cxx 
GocadReader/vtkGocadVSet.cxx 
GocadReader/vtkGocadTSolid.cxx 
GocadReader/vtkGocadVoxet.cxx
GocadReader/vtkGocadSGrid.cxx
GXFReader/vtkGXFReader.cxx
Map3dBlockReader/vtkMap3dBlockReader.cxx
#ODBCReader/vtkODBCReader.cpp
PointSetReader/vtkPointSetReader.cpp
SGeMSReader/vtkSGeMSReader.cxx
SGeMSReader/vtkSGeMSCgridReader.cxx
ShapeFileReader/vtkShapeFileReader.cxx
SOTResult/vtkSOTResult.cxx
TriReader/vtkTriReader.cxx
LineNetworkReader/vtkLineNetworkReader.cpp
)

# Include .cxx files in on Windows platform
IF(PV_WIN32)

  SET( WIN_SM_SRCS
	VulcanReader/vtkVulcanTriReader.cxx
	VulcanReader/vtkVulcanGridReader.cxx
	VulcanReader/vtkVulcanBlockModelReader.cxx
	MineSightReader/vtkMineSightReader.cxx
	GeoReferencedImage/vtkGeoReferencedImageReader.cxx
	)

  FILE(GLOB GEOTIFF_EXTRAS1 "${CMAKE_CURRENT_SOURCE_DIR}/GeoReferencedImage/geotiff/include/*.c")
  FILE(GLOB GEOTIFF_EXTRAS2 "${CMAKE_CURRENT_SOURCE_DIR}/GeoReferencedImage/geotiff/include/*.h")
  FILE(GLOB GEOTIFF_EXTRAS3 "${CMAKE_CURRENT_SOURCE_DIR}/GeoReferencedImage/geotiff/lib/*.lib")

  # Only add GeoTiff if on Windows
  SET( WIN_SM_EXTRA_SRCS
	${GEOTIFF_EXTRAS1}
	${GEOTIFF_EXTRAS2}
	${GEOTIFF_EXTRAS3}
	)
ENDIF(PV_WIN32)

SET( PVGReaders_EXTRA_SRCS
DataMineReader/dmfile.cxx
DataMineReader/PropertyStorage.cxx
DataMineReader/PointMap.cxx
DXFReader/vtkDXFObject.cxx 
DXFReader/vtkDXFLayer.cxx 
DXFReader/vtkDXFParser.cxx 
DXFReader/vtkDXFBlock.cxx
DXFReader/vtkDXFObjectMap.cxx 
FDEM/FDEMStorage.cxx 
FDEM/FDEMFill.cxx 
FDEM/FDEMPoint.cxx 
FDEM/FDEMCrackJoint.cxx 
FDEM/TriangleProperties.cxx 
GocadReader/GocadColorMap.cxx
ShapeFileReader/shpread.c
ShapeFileReader/dimension.cxx
ShapeFileReader/hgrid.cxx
ShapeFileReader/hpolygon.cxx
ShapeFileReader/mgconst.cxx
ShapeFileReader/mgexcept.cxx

../common/StringUtilities.cpp
)

SET(PVGReaders_GUI_SRCS
DataMineReader/pqDataMineWireFrameReader.cxx
DrillHoleReader/pqDrillHoleReader.cxx
#EPSReader/pqEPSReader.cxx
FLAC3DReader/pqFLAC3DReader.cpp
3DECReader/pq3DECReader.cpp
GocadReader/pqGocadReader.cxx
#ODBCReader/pqODBCReader.cpp
PointSetReader/pqPointSetReader.cxx
SOTResult/pqSOTResult.cxx
LineNetworkReader/pqLineNetworkReader.cxx
)

SET(MOC_LIST
DataMineReader/pqDataMineWireFrameReader.h
DrillHoleReader/pqDrillHoleReader.h
#EPSReader/pqEPSReader.h
FLAC3DReader/pqFLAC3DReader.h
3DECReader/pq3DECReader.h
GocadReader/pqGocadReader.h
#ODBCReader/pqODBCReader.h
PointSetReader/pqPointSetReader.h
SOTResult/pqSOTResult.h
LineNetworkReader/pqLineNetworkReader.h
)

#wrap the moc for the cross platform stuff
QT4_WRAP_CPP(MOC_SRCS ${MOC_LIST})

#create mocs, and wrap for windows only
IF(PV_WIN32)
 # Only add GeoTiff if on Windows
  SET( WIN_GUI_SRCS
	GeoReferencedImage/pqGeoReferencedImageReader.cxx
	)
  
  SET( WIN_MOC_LIST
	GeoReferencedImage/pqGeoReferencedImageReader.h
	)
	QT4_WRAP_CPP(WIN_MOC_SRCS ${WIN_MOC_LIST})
ENDIF(PV_WIN32)



ADD_PARAVIEW_OBJECT_PANEL(IFACES IFACE_SRCS CLASS_NAME pqDataMineWireFrameReader
                         XML_NAME DataMineWireFrameReader XML_GROUP sources)

#ADD_PARAVIEW_OBJECT_PANEL(IFACES2 IFACE_SRCS2 CLASS_NAME pqEPSReader
#                         XML_NAME SOTEPSReader XML_GROUP sources)
                         
ADD_PARAVIEW_OBJECT_PANEL(IFACES3 IFACE_SRCS3 CLASS_NAME pqSOTResult
                         XML_NAME SOTResult XML_GROUP sources)
                         
ADD_PARAVIEW_OBJECT_PANEL(IFACES4 IFACE_SRCS4 CLASS_NAME pqDrillHoleReader
                         XML_NAME DrillHoleReader XML_GROUP sources)
						 
ADD_PARAVIEW_OBJECT_PANEL(IFACES5 IFACE_SRCS5 CLASS_NAME pqPointSetReader
                         XML_NAME PointSetReader XML_GROUP sources)	

ADD_PARAVIEW_OBJECT_PANEL(IFACES6 IFACE_SRCS6 CLASS_NAME pqGocadReader
                         XML_NAME GocadReader XML_GROUP sources)	

ADD_PARAVIEW_OBJECT_PANEL(IFACES7 IFACE_SRCS7 CLASS_NAME pqLineNetworkReader
                         XML_NAME LineNetworkReader XML_GROUP sources)		

#ADD_PARAVIEW_OBJECT_PANEL(IFACES8 IFACE_SRCS8 CLASS_NAME pqODBCReader
#                         XML_NAME ODBCReader XML_GROUP sources)		

ADD_PARAVIEW_OBJECT_PANEL(IFACES9 IFACE_SRCS9 CLASS_NAME pqFLAC3DReader
                         XML_NAME FLAC3DReader XML_GROUP sources)				

ADD_PARAVIEW_OBJECT_PANEL(IFACES10 IFACE_SRCS10 CLASS_NAME pq3DECReader
                         XML_NAME 3DECReader XML_GROUP sources)																 

# Only add GeoTiff if on Windows
IF(PV_WIN32)
	ADD_PARAVIEW_OBJECT_PANEL(WIN_IFACES WIN_IFACE_SRCS CLASS_NAME pqGeoReferencedImageReader
		                  XML_NAME GeoReferencedImageReader XML_GROUP sources)
ENDIF(PV_WIN32)

# Append EPSReader panel
#LIST(APPEND IFACES ${IFACES2})
#LIST(APPEND IFACE_SRCS ${IFACE_SRCS2})
# Append SOTResult panel
LIST(APPEND IFACES ${IFACES3})
LIST(APPEND IFACE_SRCS ${IFACE_SRCS3})
# Append DrillHoleReader panel
LIST(APPEND IFACES ${IFACES4})
LIST(APPEND IFACE_SRCS ${IFACE_SRCS4})
#Append PointSetReader panel
LIST(APPEND IFACES ${IFACES5})
LIST(APPEND IFACE_SRCS ${IFACE_SRCS5})
#Append GocadReader panel
LIST(APPEND IFACES ${IFACES6})
LIST(APPEND IFACE_SRCS ${IFACE_SRCS6})

LIST(APPEND IFACES ${IFACES7})
LIST(APPEND IFACE_SRCS ${IFACE_SRCS7})

#LIST(APPEND IFACES ${IFACES8})
#LIST(APPEND IFACE_SRCS ${IFACE_SRCS8})

LIST(APPEND IFACES ${IFACES9})
LIST(APPEND IFACE_SRCS ${IFACE_SRCS9})

LIST(APPEND IFACES ${IFACES10})
LIST(APPEND IFACE_SRCS ${IFACE_SRCS10})

ADD_PARAVIEW_PLUGIN(PVGReadersSMPlugin "1.0"
			SERVER_MANAGER_XML ReadersSM.xml
			SERVER_MANAGER_SOURCES  ${PVGReaders_SM_SRCS}
			SERVER_SOURCES  ${PVGReaders_EXTRA_SRCS}
			)
			
ADD_PARAVIEW_PLUGIN(PVGReadersGUIPlugin "1.0"
             GUI_INTERFACES ${IFACES} 
             GUI_RESOURCES GUI.qrc
             SOURCES ${PVGReaders_GUI_SRCS} ${MOC_SRCS} ${IFACE_SRCS}
			)

ADD_PARAVIEW_PLUGIN(PVGReadersCPlugin "1.0"
	GUI_RESOURCE_FILES ReadersClient.xml)

#CPACK CODE TO BUNDLE THE DLLS CORRECTLY
INSTALL(TARGETS PVGReadersSMPlugin PVGReadersGUIPlugin PVGReadersCPlugin
	RUNTIME DESTINATION bin/plugins COMPONENT Runtime
	LIBRARY DESTINATION bin/plugins 
	ARCHIVE DESTINATION bin/plugins 
  )
  
set(CPACK_COMPONENT_PVGREADERS_DISPLAY_NAME "ParaViewGeo Readers")  
set(CPACK_COMPONENT_PVGREADERS_DESCRIPTION 
   "Readers for importing mining data into ParaView")
	

# Build Vulcan/MineSight plugins on Windows platform
IF(PV_WIN32)
	ADD_PARAVIEW_PLUGIN(WindowsReadersSMPlugin "1.0"
        SERVER_MANAGER_XML WindowsReadersSM.xml
        SERVER_MANAGER_SOURCES  ${WIN_SM_SRCS}
        SERVER_SOURCES ${WIN_SM_EXTRA_SRCS}
        )
        
	ADD_PARAVIEW_PLUGIN(WindowsReadersCPlugin "1.0"
		GUI_RESOURCE_FILES WindowsReadersClient.xml
		)

	ADD_PARAVIEW_PLUGIN(WindowsReadersGUIPlugin "1.0"
		GUI_INTERFACES ${WIN_IFACES} 
        GUI_RESOURCES WinGUI.qrc
        SOURCES ${WIN_GUI_SRCS} ${WIN_MOC_SRCS} ${WIN_IFACE_SRCS}
		)
    
	LINK_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/VulcanReader/lib)
	LINK_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/MineSightReader/lib)
	
	target_link_libraries (WindowsReadersSMPlugin ${CMAKE_CURRENT_SOURCE_DIR}/VulcanReader/lib/mtkExportedDGD.lib )
	target_link_libraries (WindowsReadersSMPlugin ${CMAKE_CURRENT_SOURCE_DIR}/VulcanReader/lib/mtkExportedBlockModel.lib )
	target_link_libraries (WindowsReadersSMPlugin ${CMAKE_CURRENT_SOURCE_DIR}/VulcanReader/lib/mtkExportedGrid.lib )
	target_link_libraries (WindowsReadersSMPlugin ${CMAKE_CURRENT_SOURCE_DIR}/VulcanReader/lib/mtkExportedTri.lib )
	target_link_libraries (WindowsReadersSMPlugin ${CMAKE_CURRENT_SOURCE_DIR}/MineSightReader/lib/mintecMSDAL-vc8.lib)
		
	#CPACK CODE TO BUNDLE THE DLLS CORRECTLY	
	INSTALL(TARGETS WindowsReadersSMPlugin WindowsReadersCPlugin WindowsReadersGUIPlugin
		RUNTIME DESTINATION bin/plugins COMPONENT Runtime
		LIBRARY DESTINATION bin/plugins 
		ARCHIVE DESTINATION bin/plugins 
		)	
	
	#SPECIAL CODE JUST FOR VULCAN
	#TO COPY THE EXTRA DLLS/INCLUDES OVER	
	FILE(GLOB CPACK_VULCAN_LIB "${CMAKE_CURRENT_SOURCE_DIR}/VulcanReader/lib/*.lib")
	FILE(GLOB CPACK_VULCAN_DLLS "${CMAKE_CURRENT_SOURCE_DIR}/VulcanReader/lib/*.dll")
	FILE(GLOB CPACK_VULCAN_MANIFEST "${CMAKE_CURRENT_SOURCE_DIR}/VulcanReader/lib/*.manifest")
	LIST(APPEND CPACK_VULCAN_EXTRAS ${CPACK_VULCAN_DLLS} ${CPACK_VULCAN_LIB} ${CPACK_VULCAN_MANIFEST})

	INSTALL(FILES ${CPACK_VULCAN_EXTRAS}
		DESTINATION bin
		COMPONENT Runtime
		)	
	
	#MINESIGHT
	FILE(GLOB MINESIGHT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/MineSightReader/lib/*.lib")
	FILE(GLOB MINESIGHT_DLLS "${CMAKE_CURRENT_SOURCE_DIR}/MineSightReader/lib/*.dll")
	LIST(APPEND MINESIGHT_EXTRAS ${MINESIGHT_LIB} ${MINESIGHT_DLLS})
			
	INSTALL(FILES ${MINESIGHT_EXTRAS}
		DESTINATION bin
		COMPONENT Runtime		
		)

	LINK_DIRECTORIES ( ${CMAKE_CURRENT_SOURCE_DIR}/GeoReferencedImage/geotiff/lib )

	# LINK GEOTIFF LIBRARIES
	target_link_libraries (WindowsReadersSMPlugin ${CMAKE_CURRENT_SOURCE_DIR}/GeoReferencedImage/geotiff/lib/geotiff_i.lib )
	target_link_libraries (WindowsReadersSMPlugin ${CMAKE_CURRENT_SOURCE_DIR}/GeoReferencedImage/geotiff/lib/libtiff_i.lib )
	  
	#SPECIAL CODE JUST FOR GEOTIFF
	#TO COPY THE EXTRA DLLS/INCLUDES OVER	
	FILE(GLOB CPACK_GEOTIFF_LIB "${CMAKE_CURRENT_SOURCE_DIR}/GeoReferencedImage/geotiff/lib/*.lib")
	FILE(GLOB CPACK_GEOTIFF_DLL "${CMAKE_CURRENT_SOURCE_DIR}/GeoReferencedImage/geotiff/bin/*.dll")
	LIST(APPEND CPACK_GEOTIFF_EXTRAS ${CPACK_GEOTIFF_LIB} ${CPACK_GEOTIFF_DLL}) 
	
	INSTALL(FILES ${CPACK_GEOTIFF_EXTRAS}
	  DESTINATION bin
	  COMPONENT Runtime
	  )
ENDIF(PV_WIN32)
ENDIF(QT4_FOUND)

		

