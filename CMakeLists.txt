# -----------------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# conjure_enum (header only)
# Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
#   by David L. Dight
# see https://github.com/fix8mt/conjure_enum
#
# Lightweight header-only C++20 enum reflection
#
#		Based on magic_enum <https://github.com/Neargye/magic_enum>
#		Copyright (c) 2019 - 2024 Daniil Goncharov <neargye@gmail.com>.
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
cmake_minimum_required (VERSION 3.20)
project (conjure_enum
   LANGUAGES CXX
	HOMEPAGE_URL https://github.com/fix8mt/conjure_enum
   DESCRIPTION "Lightweight C++20 enum reflection"
   VERSION 1.0.0
)

# to disable building unit tests:
# cmake -DBUILD_UNITTESTS=false ..
option(BUILD_UNITTESTS "enable building unit tests" true)
message("-- Build unit tests: ${BUILD_UNITTESTS}")
if(BUILD_UNITTESTS)
	include(FetchContent)
	FetchContent_Declare(Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_SHALLOW ON
		GIT_TAG devel
	)
	FetchContent_MakeAvailable(Catch2)
	list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()

set(files example.cpp)
if(BUILD_UNITTESTS)
   list(APPEND files unittests.cpp)
endif()
foreach(x IN LISTS files)
	cmake_path(GET x STEM LAST_ONLY target)
	add_executable(${target} examples/${x})
	set_target_properties(${target} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED true)
		target_include_directories(${target} PRIVATE include examples)
	if(BUILD_UNITTESTS)
		target_link_libraries(${target} PRIVATE Catch2::Catch2WithMain)
	endif()
	add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_STRIP} ${target})
	cmake_path(GET x FILENAME fname)
	get_target_property(cppstd ${target} CXX_STANDARD)
	message("-- adding ${fname} cxx std: C++${cppstd}")
endforeach()

if(BUILD_UNITTESTS)
	include(Catch)
	enable_testing()
	catch_discover_tests(unittests)
endif()
