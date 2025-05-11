# -----------------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: Copyright (C) 2024-25 Fix8 Market Technologies Pty Ltd
# SPDX-FileType: SOURCE
#
# conjure_enum (header only)
#   by David L. Dight
# see https://github.com/fix8mt/conjure_enum
#
# Lightweight header-only C++20 enum and typename reflection
#
# Licensed under the MIT License <http://opensource.org/licenses/MIT>.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is furnished
# to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next paragraph)
# shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# ----------------------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.21+)
message(STATUS "CMake version ${CMAKE_VERSION}")
include(cmake/buildutils.cmake)
project (conjure_enum
   LANGUAGES CXX
	HOMEPAGE_URL https://github.com/fix8mt/${PROJECT_NAME}
   DESCRIPTION "Lightweight C++20 enum and typename reflection"
   VERSION 1.2.0)

if(NOT MSVC)
	fix8_setbuildtype(CONJURE_ENUM Release)
endif()
fix8_addoption("BUILD_UNITTESTS|enable building unit tests|true")
fix8_addoption("BUILD_STRIP_EXE|enable stripping of non-unittest executables|true")
fix8_addoption("BUILD_ALL_WARNINGS|enable building with all warnings|true")
fix8_addoption("BUILD_CLANG_PROFILER|enable clang build profiler|false")

fix8_build(examples srcloctest 20)
foreach(x example statictest cbenchmark)
	fix8_build(examples ${x} 20)
	add_dependencies(${x} srcloctest) # srcloctest should be built even if errors with conjure_enum
	if(BUILD_STRIP_EXE)
		add_custom_command(TARGET ${x} POST_BUILD COMMAND ${CMAKE_STRIP} ${x})
	endif()
endforeach()

# make include visible to inheriting projects
add_library(${PROJECT_NAME} INTERFACE)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/fix8/*.hpp")
target_sources(${PROJECT_NAME} INTERFACE FILE_SET HEADERS FILES ${HEADER_FILES})
install(TARGETS ${PROJECT_NAME} FILE_SET HEADERS DESTINATION .)
target_include_directories(${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)

if(BUILD_UNITTESTS)
	fix8_fetch(Catch2 catchorg/Catch2 devel)
	target_compile_features(Catch2 PRIVATE cxx_std_17)
	list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
	include(Catch)
	enable_testing()
	foreach(x unittests edgetests)
		fix8_build(utests ${x} 20)
		target_link_libraries(${x} PRIVATE Catch2::Catch2WithMain)
		catch_discover_tests(${x})
	endforeach()
endif()
