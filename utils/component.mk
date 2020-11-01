#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#
COMPONENT_SRCDIRS := src
COMPONENT_ADD_INCLUDEDIRS = include

CXXFLAGS += -std=c++14
CXXFLAGS += -std=c++17

CXXFLAGS += -D LOG_LOCAL_LEVEL=ESP_LOG_DEBUG