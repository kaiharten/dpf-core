
add_library(plugin SHARED create.cpp interface.h)
target_include_directories(plugin PUBLIC ../../)
target_compile_features(plugin PRIVATE cxx_std_17)
target_link_libraries(plugin dpf-core ${CMAKE_DL_LIBS})
target_compile_options(plugin PRIVATE -fPIC)

add_executable(loader load.cpp interface.h)
target_include_directories(loader PUBLIC ../../)
target_compile_features(loader PRIVATE cxx_std_17)
target_link_libraries(loader dpf-core ${CMAKE_DL_LIBS})