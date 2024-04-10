idf_component_register(INCLUDE_DIRS include)
add_prebuilt_library(zlib "libzlib.a")
target_link_libraries(${COMPONENT_LIB} INTERFACE zlib)