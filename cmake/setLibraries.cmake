#
# Qt5
#
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Designer REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)

find_package(OpenGL)

#
# FFmpeg
#
if(USE_FFMPEG)
find_library(LibAVDevice
	NAMES avdevice
	HINTS ${FFMPEG}/lib
	NO_DEFAULT_PATH
)
find_library(LibAVFormat
	NAMES avformat
	HINTS ${FFMPEG}/lib
	NO_DEFAULT_PATH
)
find_library(LibAVFilter
	NAMES avfilter
	HINTS ${FFMPEG}/lib
	NO_DEFAULT_PATH
)
find_library(LibAVCodec
	NAMES avcodec
	HINTS ${FFMPEG}/lib
	NO_DEFAULT_PATH
)
find_library(LibAVUtil
	NAMES avutil
	HINTS ${FFMPEG}/lib
	NO_DEFAULT_PATH
)
find_library(LibSwScale
	NAMES swscale
	HINTS ${FFMPEG}/lib
	NO_DEFAULT_PATH
)
set(LibFFmpeg ${LibAVDevice} ${LibAVFormat} ${LibAVFilter} ${LibAVCodec} ${LibAVUtil} ${LibSwScale})
endif()
