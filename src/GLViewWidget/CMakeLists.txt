add_library (GLViewWidget SHARED
				GLViewWidget_global.h
				GLViewWidget.cpp
				GLViewWidget.h
				)
set_property (TARGET GLViewWidget PROPERTY FOLDER "Shader_App")

add_definitions(-DGLVIEWWIDGET_LIBRARY)

target_include_directories (GLViewWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (GLViewWidget LINK_PUBLIC 
							Qt5::Widgets
							Qt5::OpenGL
							${OPENGL_LIBRARIES}
							)

install (
		TARGETS GLViewWidget
		RUNTIME DESTINATION .
		LIBRARY DESTINATION .
		)
