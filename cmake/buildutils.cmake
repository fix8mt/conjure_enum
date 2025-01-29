# ----------------------------------------------------------------------------------------
# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
# SPDX-FileType: SOURCE
#
# cmake utils
# Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
#   by David L. Dight
# see https://github.com/fix8mt/uri
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
# cmake build utils
# min cmake version 3.20 (Mar 24, 2021)
# ----------------------------------------------------------------------------------------
function(fix8_setbuildtype define_prefix default_type)
	if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
		if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			add_compile_definitions(${define_prefix}_DEBUG_BUILD)
		elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
			add_compile_definitions(${define_prefix}_RELWITHDEBINFO_BUILD)
		elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
			add_compile_definitions(${define_prefix}_MINSIZEREL_BUILD)
		elseif(${default_type} STREQUAL "Release")
			add_compile_definitions(${define_prefix}_RELEASE_BUILD)
		else()
			message(FATAL_ERROR "Unsupported build type ${default_type}")
		endif()
		message("-- ${CMAKE_PROJECT_NAME} version ${CMAKE_PROJECT_VERSION}, build type ${CMAKE_BUILD_TYPE}")
	else()
		set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE ${default_type})
		fix8_setbuildtype(${define_prefix} ${default_type})
	endif()
endfunction()

# ----------------------------------------------------------------------------------------
macro(fix8_addoption option_string)
	string(REPLACE "|" ";" part ${option_string})
	list(GET part 0 opt_name)
	list(GET part 1 opt_description)
	list(GET part 2 opt_default)
	if(NOT DEFINED ${opt_name})
		set(${opt_name} ${opt_default})
	endif()
	option(${opt_name} "${opt_description}" ${${opt_name}})
	message("-- Build: ${opt_description}: ${${opt_name}}")
endmacro()

# ----------------------------------------------------------------------------------------
macro(fix8_fetch modname parturl tag)
	include(FetchContent)
	message(STATUS "Downloading ${modname}...")
	FetchContent_Declare(${modname} GIT_REPOSITORY https://github.com/${parturl}.git GIT_SHALLOW ON GIT_TAG ${tag})
	FetchContent_MakeAvailable(${modname})
endmacro()
