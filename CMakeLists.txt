# minimum version required
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(MeshEditor)

# adding ITK, VTK and Qt
#FIND_PACKAGE(ITK REQUIRED)
#INCLUDE(${ITK_USE_FILE})
FIND_PACKAGE (VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )

# Support out-of-source build
#adding sources files
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

# Setting MeshEditorSrcs/MeshEditorHeaders/MeshEditorUI
SET(MeshEditorSrcs
        main.cpp
        mainWindow.cpp
	)
SET(MeshEditorHeaders
        mainWindow.h
	)
SET(MeshEditorUI
        UIs/mainWindow.ui		
	)
#SET(MeshEditorQRC Resources/mainwindow.qrc)

# Convert Qt files to ui and moc to generate qt aditional files 
QT4_WRAP_CPP(MOCSrcs ${MeshEditorHeaders})
QT4_WRAP_UI(UISrcs ${MeshEditorUI})
QT4_ADD_RESOURCES(ICONSrcs ${MeshEditorQRC})
 
# Packaging source code in same files 
SOURCE_GROUP("Resources" FILES ${MeshEditorUI})
SOURCE_GROUP("Generated" FILES ${MOCSrcs} ${UISrcs} ${ICONSrcs})


# adding source files
ADD_EXECUTABLE(MeshEditor ${MeshEditorSrcs} ${MeshEditorHeaders} 
	${UISrcs} ${MOCSrcs} ${ICONSrcs})

# linking QVTK¡¢VTK¡¢ITK
#TARGET_LINK_LIBRARIES(MeshEditor ${VTK_LIBRARIES} ${ITK_LIBRARIES})
TARGET_LINK_LIBRARIES(MeshEditor ${VTK_LIBRARIES})
 