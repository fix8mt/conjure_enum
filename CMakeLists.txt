# -----------------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
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
cmake_minimum_required (VERSION 3.20)
project (conjure_enum
   LANGUAGES CXX
	HOMEPAGE_URL https://github.com/fix8mt/conjure_enum
   DESCRIPTION "Lightweight C++20 enum and typename reflection"
   VERSION 1.1.0)

message("${CMAKE_PROJECT_NAME} version ${CMAKE_PROJECT_VERSION}")

# to disable strip:
# cmake -DBUILD_STRIP_EXE=false ..
option(BUILD_STRIP_EXE "enable stripping of non-unittest executables" true)
message("-- Build: strip exes ${BUILD_STRIP_EXE}")

# to disable warnings:
# cmake -DBUILD_ALL_WARNINGS=false ..
option(BUILD_ALL_WARNINGS "enable building with all warnings" true)
message("-- Build: with all warnings ${BUILD_ALL_WARNINGS}")

# to disable building unit tests:
# cmake -DBUILD_UNITTESTS=false ..
option(BUILD_UNITTESTS "enable building unit tests" true)
message("-- Build: unit tests ${BUILD_UNITTESTS}")

# to enable clang build profiler:
# cmake -DBUILD_CLANG_PROFILER=true ..
# see examples/cbenchmark.sh
option(BUILD_CLANG_PROFILER "enable clang build profiler" false)
message("-- Build: clang profiler ${BUILD_CLANG_PROFILER}")
if(BUILD_CLANG_PROFILER)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(-ftime-trace)
	else()
		message(WARNING "BUILD_CLANG_PROFILER only available with Clang")
	endif()
endif()

function(build loc x)
	add_executable(${x} ${loc}/${x}.cpp)
	if(NOT ${x} STREQUAL srcloctest)
		add_dependencies(${x} srcloctest) # srcloctest should be built even if errors with conjure_enum
	endif()
	set_target_properties(${x} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED true)
	target_include_directories(${x} PRIVATE include)
	if(BUILD_ALL_WARNINGS)
		target_compile_options(${x} PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:/W4>
			$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>)
	endif()
	get_target_property(cppstd ${x} CXX_STANDARD)
	message("-- adding ${x}.cpp CXX_STANDARD: C++${cppstd} (${CMAKE_CXX_COMPILER_ID})")
endfunction()

foreach(x srcloctest example statictest cbenchmark)
	build(examples ${x})
	if(BUILD_STRIP_EXE)
		add_custom_command(TARGET ${x} POST_BUILD COMMAND ${CMAKE_STRIP} ${x})
	endif()
endforeach()

if(BUILD_UNITTESTS)
	include(FetchContent)
	FetchContent_Declare(Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_SHALLOW ON
		GIT_TAG devel)
	FetchContent_MakeAvailable(Catch2)
	list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
	target_compile_features(Catch2 PRIVATE cxx_std_20)
	include(Catch)
	enable_testing()
	foreach(x unittests edgetests)
		build(utests ${x})
		target_link_libraries(${x} PRIVATE Catch2::Catch2WithMain)
		catch_discover_tests(${x})
	endforeach()
endif()
