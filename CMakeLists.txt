cmake_minimum_required(VERSION 3.5)

include(FindPkgConfig)

add_executable(lv2-plugin-checker
  "lv2-plugin-checker.cc")

pkg_check_modules(LILV "lilv-0")
target_include_directories(lv2-plugin-checker PRIVATE "${LILV_INCLUDE_DIRS}")
target_link_libraries(lv2-plugin-checker PRIVATE "${LILV_LIBRARIES}")
