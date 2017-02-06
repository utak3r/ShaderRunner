if (WIN32)
set (GUI_TYPE WIN32)
endif (WIN32)

add_executable (ShaderRunner ${GUI_TYPE}
					ShaderRunner.rc
					main.cpp
					srMainWindow.cpp
					srMainWindow.h
					srMainWindow.ui
					ShaderRunner.qrc
					)
set_property (TARGET ShaderRunner PROPERTY FOLDER "Shader_App")

source_group("Ui Files" REGULAR_EXPRESSION "${UI_FILES_FILTER}")
source_group("Resource Files" REGULAR_EXPRESSION "${RESOURCE_FILES_FILTER}")

include_directories (
				${PROJECT_ROOT}/src/GLViewWidget
				)

target_link_libraries (ShaderRunner LINK_PUBLIC
							GLViewWidget
							Qt5::Widgets
							)

# Copy Qt libs to runtime folder
add_custom_command(
	TARGET ShaderRunner
	COMMAND ${CMAKE_COMMAND} 
		-DMSVC_BUILD_CONFIG_DIR=${CMAKE_CFG_INTDIR} 
		-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		-DQTDIRBIN=${QTDIRBIN}
		-P "${PROJECT_ROOT}/cmake/installQtLibs.cmake" 
	VERBATIM 
)

# Copy Qt plugins to runtime folder
add_custom_command(
	TARGET ShaderRunner
	COMMAND ${CMAKE_COMMAND} 
		-DMSVC_BUILD_CONFIG_DIR=${CMAKE_CFG_INTDIR} 
		-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		-DQTDIRPLUGINS=${QTDIRPLUGINS}
		-P "${PROJECT_ROOT}/cmake/installQtPlugins.cmake" 
	VERBATIM 
)

if(USE_FFMPEG)
add_custom_command (
	TARGET ShaderRunner
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
	${FFMPEG}/presets
	"${CMAKE_BINARY_DIR}/src/ShaderRunner/.ffmpeg"
	)
endif()

install (TARGETS ShaderRunner
			RUNTIME DESTINATION .
			LIBRARY DESTINATION .
			ARCHIVE DESTINATION .
)

install (
		DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/
		DESTINATION .
		FILES_MATCHING
		PATTERN "Qt5*.dll"
		PATTERN "Qt5*d.dll" EXCLUDE
		)

install (
		DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/imageformats/ 
		DESTINATION imageformats
		FILES_MATCHING
		PATTERN "*.dll"
		PATTERN "*d.dll" EXCLUDE
		)

install (
		FILES ${QTDIRPLUGINS}/iconengines/qsvgicon.dll
		DESTINATION iconengines
		)

install (
		FILES ${QTDIRPLUGINS}/platforms/qwindows.dll
		DESTINATION platforms
		)
