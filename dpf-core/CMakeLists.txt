add_library(dpf-core
  SHARED
  core.cpp
  core.h
)
target_compile_features(dpf-core PRIVATE cxx_std_17)
target_compile_options(dpf-core PRIVATE -fPIC)