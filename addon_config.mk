# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file 
# system
# 
# The PG can be used to detect errors in this file, just create a new project with this addon 
# and the PG will write to the console the kind of error and in which line it is

meta:
	ADDON_NAME = ofxSharedMemory
	ADDON_DESCRIPTION = Shared Memory addon for openFrameworks, using kyr0/libsharedmemory
	ADDON_AUTHOR = Fumiya Funatsu
	ADDON_TAGS = "utility"
	ADDON_URL = https://github.com/funatsufumiya/ofxSharedMemory

common:
	# dependencies with other addons, a list of them separated by spaces 
	# or use += in several lines
	# ADDON_DEPENDENCIES =
	
	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	# ADDON_INCLUDES =
	
	# any special flag that should be passed to the compiler when using this
	# addon
	# ADDON_CFLAGS =
	
	# any special flag that should be passed to the linker when using this
	# addon, also used for system libraries with -lname
	# ADDON_LDFLAGS =
	
	# linux only, any library that should be included in the project using
	# pkg-config
	# ADDON_PKG_CONFIG_LIBRARIES =
	
	# osx/iOS only, any framework that should be included in the project
	# ADDON_FRAMEWORKS =
	
	# source files, these will be usually parsed from the file system looking
	# in the src folders in libs and the root of the addon. if your addon needs
	# to include files in different places or a different set of files per platform
	# they can be specified here
	# ADDON_SOURCES =
	# ADDON_SOURCES_EXCLUDE = src/EngineVk.cpp
	
	# some addons need resources to be copied to the bin/data folder of the project
	# specify here any files that need to be copied, you can use wildcards like * and ?
	# ADDON_DATA = 
	
	# when parsing the file system looking for libraries exclude this for all or
	# a specific platform
	# ADDON_LIBS_EXCLUDE =
	
linux64:
	
linux:

linuxarmv6l:
	#ADDON_CFLAGS += -DOFXIMGUI_DEBUG
	#ADDON_CFLAGS += -DIMGUI_IMPL_OPENGL_ES2
	#ADDON_CFLAGS += -DIMGUI_IMPL_OPENGL_ES3
	#ADDON_CFLAGS += -DUSE_PI_LEGACY
	#ADDON_LDFLAGS += -lGL

	
linuxarmv7l:
	#ADDON_CFLAGS += -DOFXIMGUI_DEBUG
	#ADDON_CFLAGS += -DIMGUI_IMPL_OPENGL_ES2
	#ADDON_CFLAGS += -DIMGUI_IMPL_OPENGL_ES3
	#ADDON_CFLAGS += -DUSE_PI_LEGACY

msys2:

android/armeabi:	
	
android/armeabi-v7a:	

ios:
	# osx/iOS only, any framework that should be included in the project
	#ADDON_CFLAGS += -DIMGUI_IMPL_OPENGL_ES2
#	ADDON_SOURCES_EXCLUDE += libs/imgui/backends/imgui_impl_opengl2.cpp
#	ADDON_SOURCES_EXCLUDE += libs/imgui/backends/imgui_impl_opengl2.h
#	ADDON_SOURCES_EXCLUDE += libs/imgui/backends/imgui_impl_opengl3.cpp
#	ADDON_SOURCES_EXCLUDE += libs/imgui/backends/imgui_impl_opengl3.h
	# ADDON_SOURCES_EXCLUDE += libs/imgui/backends/imgui_impl_glfw.cpp
	# ADDON_SOURCES_EXCLUDE += libs/imgui/backends/imgui_impl_glfw.h
	# ADDON_SOURCES_EXCLUDE += src/EngineGLFW.cpp
	# ADDON_SOURCES_EXCLUDE += src/EngineGLFW.h
