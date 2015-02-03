project (delaunay)

cmake_minimum_required(VERSION 2.4.5)

set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )

if( COMMAND cmake_policy )
  cmake_policy( SET CMP0003 NEW )  
endif()

set( QT_USE_QTXML    TRUE )
set( QT_USE_QTMAIN   TRUE )
set( QT_USE_QTSCRIPT  TRUE )
set( QT_USE_QTOPENGL  TRUE )

find_package(CGAL COMPONENTS)
find_package(Qt4)
find_package(GLUT)
find_package(OpenGL)

set( HDRS glviewer.h scene.h window.h  include/cdt.h)

set( SRCS glviewer.cpp main.cpp window.cpp)

#set( MOCS moc_glviewer.cxx moc_window.cxx )

set(SRC_MOC window.h glviewer.h)

if(WIN32)
	add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4503 /nologo" )
endif(WIN32)



if( CGAL_FOUND AND QT4_FOUND AND OPENGL_FOUND )
    include(${CGAL_USE_FILE})
    include(${QT_USE_FILE})

    SET(MY_BUILD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)  #Variable build path
    SET(MY_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})  #Variable src path
    INCLUDE_DIRECTORIES(${MY_BUILD_PATH})
    INCLUDE_DIRECTORIES(${MY_SRC_PATH})


    # UI files (Qt Designer files)
    qt4_wrap_ui( DT_UI_FILES delaunay.ui)

    # qrc files (resources files, that contain icons, at least)
    qt4_add_resources( DT_RESOURCE_FILES delaunay.qrc )

    # use the Qt MOC preprocessor on classes that derives from QObject
    qt4_wrap_cpp(MOCS ${SRC_MOC} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED OPTIONS)

    # use the Qt MOC preprocessor on classes that derives from QObject
    #qt4_generate_moc( window.h moc_window.cxx )
    #qt4_generate_moc( glviewer.h moc_glviewer.cxx )

    # The executable itself.
    add_executable( delaunay ${SRCS} ${MOCS} ${DT_UI_FILES} ${DT_RESOURCE_FILES} )

    # Link with Qt libraries
    target_link_libraries( delaunay ${QT_LIBRARIES} )
    
    # Link with Glut and OpenGL
    target_link_libraries( delaunay ${OPENGL_LIBRARY} ${GLUT_LIBRARY}  )

    # Link with CGAL
    target_link_libraries( delaunay ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

else()
  message(STATUS "NOTICE: This program requires CGAL and Qt4, and will not be compiled.")

endif()
