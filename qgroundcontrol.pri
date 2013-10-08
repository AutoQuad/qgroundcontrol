# -------------------------------------------------
# QGroundControl - Micro Air Vehicle Groundstation
# Please see our website at <http://qgroundcontrol.org>
# Maintainer:
# Lorenz Meier <lm@inf.ethz.ch>
# (c) 2009-2011 QGroundControl Developers
# This file is part of the open groundstation project
# QGroundControl is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# QGroundControl is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with QGroundControl. If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------

message(Qt version $$[QT_VERSION])

win32-msvc2008|win32-msvc2010 {
	QMAKE_POST_LINK += $$quote(echo "Copying files"$$escape_expand(\\n))
} else {
	QMAKE_POST_LINK += $$quote(echo "Copying files")
}

# Turn off serial port warnings
DEFINES += _TTY_NOWARN_

# MAC OS X
macx|macx-g++42|macx-g++|macx-llvm: {

        CONFIG += cocoa phonon
#	CONFIG += x86_64 cocoa phonon
#	CONFIG -= x86

	# For release builds remove support for various Qt debugging macros.
	CONFIG(release, debug|release) {
		DEFINES += QT_NO_DEBUG
	}

	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6

        INCLUDEPATH += /Library/Frameworks/SDL.framework/Headers

	LIBS += -framework IOKit \
#		-F$$BASEDIR/libs/lib/Frameworks \
		-framework SDL \
		-framework CoreFoundation \
		-framework ApplicationServices \
		-lm

	ICON = $$BASEDIR/files/images/icons/macx.icns

	# Copy AQ files
        QMAKE_POST_LINK += && mkdir -p $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/aq/bin
        QMAKE_POST_LINK += && cp -rf $$BASEDIR/aq/bin/aq_osx_all/* $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/aq/bin
        QMAKE_POST_LINK += && chmod +x $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/aq/bin/*
        QMAKE_POST_LINK += && mkdir -p $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/aq/mixes
        QMAKE_POST_LINK += && cp -rf $$BASEDIR/aq/mixes/* $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/aq/mixes

	# Copy google earth starter file
	QMAKE_POST_LINK += && cp -f $$BASEDIR/files/images/earth.html $$TARGETDIR/qgroundcontrol.app/Contents/MacOS
	# Copy CSS stylesheets
	QMAKE_POST_LINK += && cp -f $$BASEDIR/files/styles/* $$TARGETDIR/qgroundcontrol.app/Contents/MacOS
	# Copy audio files (this is taken from the mac deployment script)
	QMAKE_POST_LINK += && cp -r $$BASEDIR/files/audio $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/.
    # Copy support files
	QMAKE_POST_LINK += && cp -rf $$BASEDIR/files $$TARGETDIR/qgroundcontrol.app/Contents/MacOS
	# Copy MAVLink
	QMAKE_POST_LINK += && cp -rf $$BASEDIR/libs/mavlink $$TARGETDIR/qgroundcontrol.app/Contents/MacOS
	# Copy libraries
	QMAKE_POST_LINK += && mkdir -p $$TARGETDIR/qgroundcontrol.app/Contents/libs
        QMAKE_POST_LINK += && cp -rf $$BASEDIR/libs/lib/mac32/lib/* $$TARGETDIR/qgroundcontrol.app/Contents/libs
	# Copy frameworks
	QMAKE_POST_LINK += && mkdir -p $$TARGETDIR/qgroundcontrol.app/Contents/Frameworks
	QMAKE_POST_LINK += && cp -rf $$BASEDIR/libs/lib/Frameworks/* $$TARGETDIR/qgroundcontrol.app/Contents/Frameworks


	# Fix library paths inside executable
	QMAKE_POST_LINK += && install_name_tool -change libOpenThreads.dylib "@executable_path/../libs/libOpenThreads.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/qgroundcontrol
	QMAKE_POST_LINK += && install_name_tool -change libosg.dylib "@executable_path/../libs/libosg.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/qgroundcontrol
	QMAKE_POST_LINK += && install_name_tool -change libosgViewer.dylib "@executable_path/../libs/libosgViewer.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/qgroundcontrol
	QMAKE_POST_LINK += && install_name_tool -change libosgGA.dylib "@executable_path/../libs/libosgGA.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/qgroundcontrol
	QMAKE_POST_LINK += && install_name_tool -change libosgDB.dylib "@executable_path/../libs/libosgDB.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/qgroundcontrol
	QMAKE_POST_LINK += && install_name_tool -change libosgText.dylib "@executable_path/../libs/libosgText.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/qgroundcontrol
	QMAKE_POST_LINK += && install_name_tool -change libosgWidget.dylib "@executable_path/../libs/libosgWidget.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/qgroundcontrol

	# Fix library paths within libraries (inter-library dependencies)

	# OSG GA LIBRARY
	QMAKE_POST_LINK += && install_name_tool -change libosgGA.dylib "@executable_path/../libs/libosgGA.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgGA.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgDB.dylib "@executable_path/../libs/libosgDB.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgGA.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgUtil.dylib "@executable_path/../libs/libosgUtil.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgGA.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosg.dylib "@executable_path/../libs/libosg.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgGA.dylib
	QMAKE_POST_LINK += && install_name_tool -change libOpenThreads.dylib "@executable_path/../libs/libOpenThreads.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgGA.dylib

	# OSG DB LIBRARY
	QMAKE_POST_LINK += && install_name_tool -change libosgDB.dylib "@executable_path/../libs/libosgDB.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgDB.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgUtil.dylib "@executable_path/../libs/libosgUtil.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgDB.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosg.dylib "@executable_path/../libs/libosg.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgDB.dylib
	QMAKE_POST_LINK += && install_name_tool -change libOpenThreads.dylib "@executable_path/../libs/libOpenThreads.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgDB.dylib

	# OSG TEXT LIBRARY
	QMAKE_POST_LINK += && install_name_tool -change libosgText.dylib "@executable_path/../libs/libosgText.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgText.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgDB.dylib "@executable_path/../libs/libosgDB.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgText.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgUtil.dylib "@executable_path/../libs/libosgUtil.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgText.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosg.dylib "@executable_path/../libs/libosg.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgText.dylib
	QMAKE_POST_LINK += && install_name_tool -change libOpenThreads.dylib "@executable_path/../libs/libOpenThreads.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgText.dylib

	# OSG UTIL LIBRARY
	QMAKE_POST_LINK += && install_name_tool -change libosg.dylib "@executable_path/../libs/libosg.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgUtil.dylib
	QMAKE_POST_LINK += && install_name_tool -change libOpenThreads.dylib "@executable_path/../libs/libOpenThreads.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgUtil.dylib


	# OSG VIEWER LIBRARY
	QMAKE_POST_LINK += && install_name_tool -change libosgGA.dylib "@executable_path/../libs/libosgGA.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgViewer.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgText.dylib "@executable_path/../libs/libosgText.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgViewer.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgDB.dylib "@executable_path/../libs/libosgDB.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgViewer.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgUtil.dylib "@executable_path/../libs/libosgUtil.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgViewer.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosg.dylib "@executable_path/../libs/libosg.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgViewer.dylib
	QMAKE_POST_LINK += && install_name_tool -change libOpenThreads.dylib "@executable_path/../libs/libOpenThreads.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgViewer.dylib

	# OSG WIDGET LIBRARY
	QMAKE_POST_LINK += && install_name_tool -change libosgGA.dylib "@executable_path/../libs/libosgGA.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgWidget.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgText.dylib "@executable_path/../libs/libosgText.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgWidget.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgDB.dylib "@executable_path/../libs/libosgDB.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgWidget.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgUtil.dylib "@executable_path/../libs/libosgUtil.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgWidget.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosg.dylib "@executable_path/../libs/libosg.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgWidget.dylib
	QMAKE_POST_LINK += && install_name_tool -change libOpenThreads.dylib "@executable_path/../libs/libOpenThreads.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgWidget.dylib
	QMAKE_POST_LINK += && install_name_tool -change libosgViewer.dylib "@executable_path/../libs/libosgViewer.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosgWidget.dylib

	# CORE OSG LIBRARY
	QMAKE_POST_LINK += && install_name_tool -change libOpenThreads.dylib "@executable_path/../libs/libOpenThreads.dylib" $$TARGETDIR/qgroundcontrol.app/Contents/libs/libosg.dylib

	# SDL Framework
	QMAKE_POST_LINK += && install_name_tool -change "@rpath/SDL.framework/Versions/A/SDL" "@executable_path/../Frameworks/SDL.framework/Versions/A/SDL" $$TARGETDIR/qgroundcontrol.app/Contents/MacOS/qgroundcontrol

	# No check for GLUT.framework since it's a MAC default
	message("Building support for OpenSceneGraph")
	DEPENDENCIES_PRESENT += osg
	DEFINES += QGC_OSG_ENABLED
	# Include OpenSceneGraph libraries
	INCLUDEPATH += -framework GLUT \
        -framework Cocoa \
        $$BASEDIR/libs/lib/mac32/include

	LIBS += -framework GLUT \
        -framework Cocoa \
#        -L$$BASEDIR/libs/lib/mac64/lib \
        -L$$BASEDIR/libs/lib/mac32/lib \
        -lOpenThreads \
        -losg \
        -losgViewer \
        -losgGA \
        -losgDB \
        -losgText \
        -losgWidget

        #exists(/usr/local/include/google/protobuf) {
        #	message("Building support for Protocol Buffers")
        #	DEPENDENCIES_PRESENT += protobuf
        #	# Include Protocol Buffers libraries
        #	LIBS += -L/usr/local/lib \
        #    -lprotobuf \
        #    -lprotobuf-lite \
        #    -lprotoc
        #
        #	DEFINES += QGC_PROTOBUF_ENABLED
        #}

	exists(/opt/local/include/libfreenect)|exists(/usr/local/include/libfreenect) {
		message("Building support for libfreenect")
		DEPENDENCIES_PRESENT += libfreenect
		# Include libfreenect libraries
		LIBS += -lfreenect
		DEFINES += QGC_LIBFREENECT_ENABLED
	}
}

# GNU/Linux
linux-g++|linux-g++-64{

	CONFIG -= console
	DEFINES += __STDC_LIMIT_MACROS

	release {
		DEFINES += QT_NO_DEBUG
	}

	INCLUDEPATH += /usr/include \
        /usr/local/include \
        /usr/include/qt4/phonon

	LIBS += \
		-L/usr/lib \
		-L/usr/local/lib64 \
		-lm \
		-lflite_cmu_us_kal \
		-lflite_usenglish \
		-lflite_cmulex \
		-lflite \
		-lSDL \
		-lSDLmain

	exists(/usr/include/osg) | exists(/usr/local/include/osg) {
		message("Building support for OpenSceneGraph")
		DEPENDENCIES_PRESENT += osg
		# Include OpenSceneGraph libraries
		LIBS += -losg \
            -losgViewer \
            -losgGA \
            -losgDB \
            -losgText \
            -lOpenThreads

		DEFINES += QGC_OSG_ENABLED
	}

	exists(/usr/include/osg/osgQt) | exists(/usr/include/osgQt) |
	exists(/usr/local/include/osg/osgQt) | exists(/usr/local/include/osgQt) {
		message("Building support for OpenSceneGraph Qt")
		# Include OpenSceneGraph Qt libraries
		LIBS += -losgQt
		DEFINES += QGC_OSG_QT_ENABLED
	}

	exists(/usr/local/include/google/protobuf) {
		message("Building support for Protocol Buffers")
		DEPENDENCIES_PRESENT += protobuf
		# Include Protocol Buffers libraries
		LIBS += -lprotobuf \
            -lprotobuf-lite \
            -lprotoc

		DEFINES += QGC_PROTOBUF_ENABLED
	}

	exists(/usr/local/include/libfreenect/libfreenect.h) {
		message("Building support for libfreenect")
		DEPENDENCIES_PRESENT += libfreenect
		INCLUDEPATH += /usr/include/libusb-1.0
		# Include libfreenect libraries
		LIBS += -lfreenect
		DEFINES += QGC_LIBFREENECT_ENABLED
	}

	# Validated copy commands
	!exists($$TARGETDIR){
		QMAKE_POST_LINK += && mkdir -p $$TARGETDIR
	}
	DESTDIR = $$TARGETDIR
	# Copy AQ files
        QMAKE_POST_LINK += && mkdir -p $$TARGETDIR/aq/bin
        QMAKE_POST_LINK += && cp -rf $$BASEDIR/aq/bin/aq_unix_all/* $$TARGETDIR/aq/bin
        QMAKE_POST_LINK += && mkdir -p $$TARGETDIR/aq/mixes
        QMAKE_POST_LINK += && cp -rf $$BASEDIR/aq/mixes/* $$TARGETDIR/aq/mixes
        QMAKE_POST_LINK += && chmod +x $$TARGETDIR/aq/bin/*
	QMAKE_POST_LINK += && cp -rf $$BASEDIR/files $$TARGETDIR
	QMAKE_POST_LINK += && cp -rf $$BASEDIR/data $$TARGETDIR

	# osg/osgEarth dynamic casts might fail without this compiler option.
	# see http://osgearth.org/wiki/FAQ for details.
	QMAKE_CXXFLAGS += -Wl,-E
}

linux-g++ {
	message("Building for GNU/Linux 32bit/i386")
        QMAKE_POST_LINK += && mv -f $$TARGETDIR/aq/bin/cal_32 $$TARGETDIR/aq/bin/cal
        QMAKE_POST_LINK += && mv -f $$TARGETDIR/aq/bin/sim3_32 $$TARGETDIR/aq/bin/sim3
}
linux-g++-64 {
	message("Building for GNU/Linux 64bit/x64 (g++-64)")
	exists(/usr/local/lib64) {
		LIBS += -L/usr/local/lib64
	}
        QMAKE_POST_LINK += && mv -f $$TARGETDIR/aq/bin/cal_64 $$TARGETDIR/aq/bin/cal
        QMAKE_POST_LINK += && mv -f $$TARGETDIR/aq/bin/sim3_64 $$TARGETDIR/aq/bin/sim3
}

# Windows (32bit), Visual Studio
win32-msvc2008|win32-msvc2010 {

	win32-msvc2008 {
		message(Building for Windows Visual Studio 2008 (32bit))
	}
	win32-msvc2010 {
		message(Building for Windows Visual Studio 2010 (32bit))
	}

		# Specify multi-process compilation within Visual Studio.
	# (drastically improves compilation times for multi-core computers)
        QMAKE_CFLAGS_DEBUG += /MP
        QMAKE_CXXFLAGS_DEBUG += /MP
        QMAKE_CFLAGS_RELEASE += /MP
        QMAKE_CXXFLAGS_RELEASE += /MP

	# QAxContainer support is needed for the Internet Control
	# element showing the Google Earth window
	CONFIG += qaxcontainer

	# The EIGEN library needs this define
	# to make the internal min/max functions work
	DEFINES += NOMINMAX

	# QWebkit is not needed on MS-Windows compilation environment
	CONFIG -= webkit

	# Specify the inclusion of (U)INT*_(MAX/MIN) macros within Visual Studio
	DEFINES += __STDC_LIMIT_MACROS

	# For release builds remove support for various Qt debugging macros.
	CONFIG(release, debug|release) {
		DEFINES += QT_NO_DEBUG
	}

	# For debug releases we just want the debugging console.
	CONFIG(debug, debug|release) {
		CONFIG += console
	}

	INCLUDEPATH += $$BASEDIR/libs/lib/sdl/msvc/include \
        $$BASEDIR/libs/lib/opal/include \
        $$BASEDIR/libs/lib/msinttypes

	LIBS += -L$$BASEDIR/libs/lib/sdl/msvc/lib \
        -lSDLmain -lSDL \
        -lsetupapi

	exists($$BASEDIR/libs/lib/osg123) {
		message("Building support for OSG")
		DEPENDENCIES_PRESENT += osg

		# Include OpenSceneGraph
		INCLUDEPATH += $$BASEDIR/libs/lib/osgEarth/win32/include \
			$$BASEDIR/libs/lib/osgEarth_3rdparty/win32/OpenSceneGraph-2.8.2/include
		LIBS += -L$$BASEDIR/libs/lib/osgEarth_3rdparty/win32/OpenSceneGraph-2.8.2/lib \
			-losg \
			-losgViewer \
			-losgGA \
			-losgDB \
			-losgText \
			-lOpenThreads
		DEFINES += QGC_OSG_ENABLED
	}

	RC_FILE = $$BASEDIR/qgroundcontrol.rc

	# Copy dependencies
        BASEDIR_WIN = $$replace(BASEDIR,"/","\\")
        TARGETDIR_WIN = $$replace(TARGETDIR,"/","\\")\\release
        QTLIBDLLSFX = "4.dll"
        CONFIG(debug, debug|release) {
            TARGETDIR_WIN = $$replace(TARGETDIR,"/","\\")\\debug
            QTLIBDLLSFX = "d4.dll"
        }

        # Copy AQ files
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\aq\\bin\\aq_win_all\\*" "$$TARGETDIR_WIN\\aq\\bin" /E /I $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\aq\\mixes\\*" "$$TARGETDIR_WIN\\aq\\mixes" /E /I $$escape_expand(\\n))

        # Copy VLC files
        contains(DEFINES, QGC_USE_VLC) {
            QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\libs\\vlc\\plugins\\*"  "$$TARGETDIR_WIN\\plugins" /E /I $$escape_expand(\\n))
            QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\libs\\vlc\\libvlccore.dll" "$$TARGETDIR_WIN"$$escape_expand(\\n))
            QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\libs\\vlc\\libvlc.dll" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        }

        # Copy supporting library DLLs
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\libs\\lib\\sdl\\win32\\SDL.dll" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\libs\\mavlink" "$$TARGETDIR_WIN\\mavlink" /E /I $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\libs\\thirdParty\\libxbee\\lib\\libxbee.dll" "$$TARGETDIR_WIN"$$escape_expand(\\n))

        # Copy application resources
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\files" "$$TARGETDIR_WIN\\files" /E /I $$escape_expand(\\n))
#	QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$BASEDIR_WIN\\models" "$$TARGETDIR_WIN\\models" /E /I $$escape_expand(\\n))

        # Copy Qt DLLs
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\plugins" "$$TARGETDIR_WIN" /E /I $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\phonon$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtCore$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtGui$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtMultimedia$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtNetwork$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtOpenGL$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtSql$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtSvg$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtTest$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtWebKit$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtXml$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy /D /Y "$$(QTDIR)\\bin\\QtXmlPatterns$$QTLIBDLLSFX" "$$TARGETDIR_WIN"$$escape_expand(\\n))

        CONFIG(release, debug|release) {
            QMAKE_POST_LINK += $$quote(del /F "$$TARGETDIR_WIN\\qgroundcontrol.exp"$$escape_expand(\\n))
            QMAKE_POST_LINK += $$quote(del /F "$$TARGETDIR_WIN\\qgroundcontrol.lib"$$escape_expand(\\n))

            # Copy Visual Studio DLLs
            # Note that this is only done for release because the debugging versions of these DLLs cannot be redistributed.
            # I'm not certain of the path for VS2008, so this only works for VS2010.
            win32-msvc2010 {
                QMAKE_POST_LINK += $$quote(xcopy /D /Y "\"C:\\Program Files \(x86\)\\Microsoft Visual Studio 10.0\\VC\\redist\\x86\\Microsoft.VC100.CRT\\*.dll\""  "$$TARGETDIR_WIN\\"$$escape_expand(\\n))
            }
	}
}

# Windows (32bit)
win32-g++ {

	message(Building for Windows Platform (32bit))

	# Special settings for debug
	CONFIG += CONSOLE
	OUTPUT += CONSOLE

	# The EIGEN library needs this define
	# to make the internal min/max functions work
	DEFINES += NOMINMAX

	INCLUDEPATH += $$BASEDIR/libs/lib/sdl/include \
				$$BASEDIR/libs/lib/opal/include

	LIBS += -L$$BASEDIR/libs/lib/sdl/win32 \
			-lmingw32 -lSDLmain -lSDL -mwindows \
			-lsetupapi

	CONFIG += windows



	debug {
		CONFIG += console
	}

	release {
		CONFIG -= console
		DEFINES += QT_NO_DEBUG
	}

	RC_FILE = $$BASEDIR/qgroundcontrol.rc

	# Copy dependencies

	system(cp): {
		# CP command is available, use it instead of copy / xcopy
		message("Using cp to copy image and audio files to executable")
		debug {
			QMAKE_POST_LINK += && cp $$BASEDIR/libs/lib/sdl/win32/SDL.dll $$TARGETDIR/debug/SDL.dll
			QMAKE_POST_LINK += && cp -r $$BASEDIR/files $$TARGETDIR/debug/files
                        QMAKE_POST_LINK += && cp -r $$BASEDIR/libs/mavlink $$TARGETDIR/debug/mavlink
			QMAKE_POST_LINK += && cp -r $$BASEDIR/models $$TARGETDIR/debug/models
		}

		release {
			QMAKE_POST_LINK += && cp $$BASEDIR/libs/lib/sdl/win32/SDL.dll $$TARGETDIR/release/SDL.dll
			QMAKE_POST_LINK += && cp -r $$BASEDIR/files $$TARGETDIR/release/files
                        QMAKE_POST_LINK += && cp -r $$BASEDIR/libs/mavlink $$TARGETDIR/release/mavlink
			QMAKE_POST_LINK += && cp -r $$BASEDIR/models $$TARGETDIR/release/models
		}

	} else {
		# No cp command available, go for copy / xcopy
		# Copy dependencies
		BASEDIR_WIN = $$replace(BASEDIR,"/","\\")
		TARGETDIR_WIN = $$replace(TARGETDIR,"/","\\")

		exists($$TARGETDIR/debug) {
			QMAKE_POST_LINK += && copy /Y \"$$BASEDIR_WIN\\libs\\lib\\sdl\\win32\\SDL.dll\" \"$$TARGETDIR_WIN\\debug\\SDL.dll\"
			QMAKE_POST_LINK += && xcopy \"$$BASEDIR_WIN\\files\" \"$$TARGETDIR_WIN\\debug\\files\\\" /S /E /Y
			QMAKE_POST_LINK += && xcopy \"$$BASEDIR_WIN\\libs\\mavlink\" \"$$TARGETDIR_WIN\\debug\\mavlink\\\" /S /E /Y
			QMAKE_POST_LINK += && xcopy \"$$BASEDIR_WIN\\models\" \"$$TARGETDIR_WIN\\debug\\models\\\" /S /E /Y
		}

		exists($$TARGETDIR/release) {
			QMAKE_POST_LINK += && copy /Y \"$$BASEDIR_WIN\\libs\\lib\\sdl\\win32\\SDL.dll\" \"$$TARGETDIR_WIN\\release\\SDL.dll\"
			QMAKE_POST_LINK += && xcopy \"$$BASEDIR_WIN\\files\" \"$$TARGETDIR_WIN\\release\\files\\\" /S /E /Y
			QMAKE_POST_LINK += && xcopy \"$$BASEDIR_WIN\\libs\\mavlink\" \"$$TARGETDIR_WIN\\release\\mavlink\\\" /S /E /Y
			QMAKE_POST_LINK += && xcopy \"$$BASEDIR_WIN\\models\" \"$$TARGETDIR_WIN\\release\\models\\\" /S /E /Y
		}

	}

	# osg/osgEarth dynamic casts might fail without this compiler option.
	# see http://osgearth.org/wiki/FAQ for details.
	QMAKE_CXXFLAGS += -Wl,-E
}
