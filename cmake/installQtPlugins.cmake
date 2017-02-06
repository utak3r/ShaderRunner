
if (${MSVC_BUILD_CONFIG_DIR} STREQUAL "Debug")
	set (IMGFORMATS
			${QTDIRPLUGINS}/imageformats/qicod.dll
			${QTDIRPLUGINS}/imageformats/qjpegd.dll
			${QTDIRPLUGINS}/imageformats/qsvgd.dll
			${QTDIRPLUGINS}/imageformats/qtgad.dll
			${QTDIRPLUGINS}/imageformats/qtiffd.dll
			${QTDIRPLUGINS}/imageformats/qwbmpd.dll
			${QTDIRPLUGINS}/imageformats/qwebpd.dll
			)
	set (IMGFORMATS_TARGETDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/imageformats")

	set (ICONENGINE ${QTDIRPLUGINS}/iconengines/qsvgicond.dll)
	set (ICONENGINE_TARGETDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/iconengines")

	set (PLATFORM ${QTDIRPLUGINS}/platforms/qwindowsd.dll)
	set (PLATFORM_TARGETDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/platforms")

else (${MSVC_BUILD_CONFIG_DIR} STREQUAL "Debug")
	set (IMGFORMATS
			${QTDIRPLUGINS}/imageformats/qico.dll
			${QTDIRPLUGINS}/imageformats/qjpeg.dll
			${QTDIRPLUGINS}/imageformats/qsvg.dll
			${QTDIRPLUGINS}/imageformats/qtga.dll
			${QTDIRPLUGINS}/imageformats/qtiff.dll
			${QTDIRPLUGINS}/imageformats/qwbmp.dll
			${QTDIRPLUGINS}/imageformats/qwebp.dll
			)
	set (IMGFORMATS_TARGETDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/imageformats")

	set (ICONENGINE ${QTDIRPLUGINS}/iconengines/qsvgicon.dll)
	set (ICONENGINE_TARGETDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/iconengines")

	set (PLATFORM ${QTDIRPLUGINS}/platforms/qwindows.dll)
	set (PLATFORM_TARGETDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/platforms")

endif (${MSVC_BUILD_CONFIG_DIR} STREQUAL "Debug")

file ( INSTALL ${IMGFORMATS} DESTINATION ${IMGFORMATS_TARGETDIR} )
file ( INSTALL ${ICONENGINE} DESTINATION ${ICONENGINE_TARGETDIR} )
file ( INSTALL ${PLATFORM} DESTINATION ${PLATFORM_TARGETDIR} )
