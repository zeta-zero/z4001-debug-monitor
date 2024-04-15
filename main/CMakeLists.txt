set(COMPONENT_ADD_INCLUDEDIRS
    comm
    hals
    hals/base
    hals/src
    drivers
    drivers/third
    units
    apps)
file(GLOB_RECURSE SOURCES 
     *.c
     *.cpp
     comm/*.cpp
     hals/base/*.cpp
     hals/src/*.cpp
     drivers/*.cpp
     drivers/third/*/*.c
     units/*.cpp
     apps/*.cpp)
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS ".")

