project(polyhedron)

cmake_minimum_required(VERSION 2.4.5)

set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )

if( COMMAND cmake_policy )
    cmake_policy( SET CMP0003 NEW )
endif()

# Include this package's headers first
include_directories( BEFORE ./ ./include ../../include )

# Add specific Find.cmake modules 
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ) 

SET(MY_BUILD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)  #Variable build path
SET(MY_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})  #Variable src path
INCLUDE_DIRECTORIES(${MY_BUILD_PATH})
INCLUDE_DIRECTORIES(${MY_SRC_PATH})


find_package(CGAL COMPONENTS Qt4)
include( ${CGAL_USE_FILE} )

set( QT_USE_QTXML    TRUE )
set( QT_USE_QTMAIN   TRUE )
set( QT_USE_QTmeshRIPT  TRUE )
set( QT_USE_QTOPENGL  TRUE )
find_package(Qt4)
find_package(OpenGL)



if( QT4_FOUND )
    include( ${QT_USE_FILE} )
    find_package(QGLViewer)
endif()

if(WIN32)
	add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4503 /nologo" )
endif(WIN32)



set( HDRS polyhedron.h viewer.h scene.h window.h )

set( SRCS viewer.cpp main.cpp scene.cpp window.cpp)

set( MOCS moc_window.cxx 
          moc_viewer.cxx )

if( CGAL_Qt4_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND )
    include_directories( ${QGLVIEWER_INCLUDE_DIR} )

    # UI files (Qt Designer files)
    qt4_wrap_ui( DT_UI_FILES polyhedron.ui)

    include(AddFileDependencies)

    # qrc files (resources files, that contain icons, at least)
    qt4_add_resources( DT_RESOURCE_FILES polyhedron.qrc )
  
    # use the Qt MOC preprocessor on classes that derives from QObject
    qt4_generate_moc( window.h moc_window.cxx )
    qt4_generate_moc( viewer.h moc_viewer.cxx )

    # The executable itself.
    add_executable( polyhedron ${SRCS} ${MOCS} ${DT_UI_FILES} ${DT_RESOURCE_FILES} )

    # Link with Qt libraries
    target_link_libraries( polyhedron ${QT_LIBRARIES} )
    
    # Link with CGAL
    target_link_libraries( polyhedron ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

    # Link with libQGLViewer, OpenGL
    target_link_libraries( polyhedron ${QGLVIEWER_LIBRARIES} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
    
    add_to_cached_list( CGAL_EXECUTABLE_TARGETS polyhedron )

else()
    
    set(OT_MISSING_DEPS "")

    if(NOT OPENGL_FOUND)
        set(OT_MISSING_DEPS "OpenGL, ${OT_MISSING_DEPS}")
    endif()

    if(NOT QT4_FOUND)
        set(OT_MISSING_DEPS "Qt4, ${OT_MISSING_DEPS}")
    endif()

    if(NOT QGLVIEWER_FOUND)
        set(OT_MISSING_DEPS "QGLViewer, ${OT_MISSING_DEPS}")
    endif()

    if(NOT CGAL_FOUND)
        set(OT_MISSING_DEPS "the CGAL library, ${OT_MISSING_DEPS}")
    endif()

    if(NOT CGAL_Qt4_FOUND)
        set(OT_MISSING_DEPS "the CGAL Qt4 library, ${OT_MISSING_DEPS}")
    endif()

    message(STATUS "NOTICE: This demo requires ${OT_MISSING_DEPS} and will not be compiled.")

endif()
