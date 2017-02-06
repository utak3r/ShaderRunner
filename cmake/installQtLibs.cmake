
if (${MSVC_BUILD_CONFIG_DIR} STREQUAL "Debug")
	set (QT_DISTRIB_FILES
			${QTDIRBIN}/Qt5Cored.dll
			${QTDIRBIN}/Qt5Guid.dll
			${QTDIRBIN}/Qt5NetworkD.dll
			${QTDIRBIN}/Qt5OpenGLd.dll
			${QTDIRBIN}/Qt5Qmld.dll
			${QTDIRBIN}/Qt5Quickd.dll
			${QTDIRBIN}/Qt5Svgd.dll
			${QTDIRBIN}/Qt5Widgetsd.dll
			${QTDIRBIN}/Qt5Xmld.dll
			${QTDIRBIN}/Qt5Testd.dll
			)
	set (QT_DISTRIB_FILES_TARGETDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
else (${MSVC_BUILD_CONFIG_DIR} STREQUAL "Debug")
	set (QT_DISTRIB_FILES
			${QTDIRBIN}/Qt5Core.dll
			${QTDIRBIN}/Qt5Gui.dll
			${QTDIRBIN}/Qt5Network.dll
			${QTDIRBIN}/Qt5OpenGL.dll
			${QTDIRBIN}/Qt5Qml.dll
			${QTDIRBIN}/Qt5Quick.dll
			${QTDIRBIN}/Qt5Svg.dll
			${QTDIRBIN}/Qt5Widgets.dll
			${QTDIRBIN}/Qt5Xml.dll
			${QTDIRBIN}/Qt5Test.dll
			)
	set (QT_DISTRIB_FILES_TARGETDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
endif (${MSVC_BUILD_CONFIG_DIR} STREQUAL "Debug")

file ( INSTALL ${QT_DISTRIB_FILES} DESTINATION ${QT_DISTRIB_FILES_TARGETDIR} )
