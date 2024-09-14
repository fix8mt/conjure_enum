//-----------------------------------------------------------------------------------------
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
// SPDX-FileType: SOURCE
//
// conjure_enum (header only)
//   by David L. Dight
// see https://github.com/fix8mt/conjure_enum
//
// Lightweight header-only C++20 enum and typename reflection
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice (including the next paragraph)
// shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//----------------------------------------------------------------------------------------
#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <fix8/conjure_enum.hpp>
#include <fix8/conjure_enum_bitset.hpp>
#include <fix8/conjure_type.hpp>

//-----------------------------------------------------------------------------------------
class foobat{};

#define ENUMS One, Two, Three, Four, Five, Six, Seven, Eight, Nine

namespace
{
	enum class NineEnums : int { ENUMS };
	enum NineEnums1 : int { ENUMS };
	namespace TEST1
	{
		enum class NineEnums : int { ENUMS };
		enum NineEnums1 : int { ENUMS };
		class foo{};
	}
	class foo{};
}

namespace TEST
{
	enum class NineEnums : int { ENUMS };
	enum NineEnums1 : int { ENUMS };
	namespace TEST1
	{
		enum class NineEnums : int { ENUMS };
		enum NineEnums1 : int { ENUMS };
		class foo{};
	}
	class foo{};
}

namespace test::rsp::gd
{
	class AFrame
	{
	public:
		enum class PreFrames { one };
	};
}

namespace test::util
{
	template <typename E>
	[[nodiscard]] constexpr size_t countOf() { return FIX8::conjure_enum<E>::count(); }
}

enum class numbers
{
   blah_0, blah_1, blah_2, blah_3, blah_4,
   blah_5, blah_6, blah_7, blah_8, blah_9,
   blah_10, blah_11, blah_12, blah_13, blah_14,
   blah_15, blah_16, blah_17, blah_18, blah_19,
   blah_20, blah_21, blah_22, blah_23, blah_24,
   blah_25, blah_26, blah_27, blah_28, blah_29,
   blah_30, blah_31, blah_32, blah_33, blah_34,
   blah_35, blah_36, blah_37, blah_38, blah_39,
   blah_40, blah_41, blah_42, blah_43, blah_44,
   blah_45, blah_46, blah_47, blah_48, blah_49,
   blah_50, blah_51, blah_52, blah_53, blah_54,
   blah_55, blah_56, blah_57, blah_58, blah_59,
   blah_60, blah_61, blah_62, blah_63, blah_64,
   blah_65, blah_66, blah_67, blah_68, blah_69,
   blah_70, blah_71, blah_72, blah_73, blah_74,
   blah_75, blah_76, blah_77, blah_78, blah_79,
   blah_80, blah_81, blah_82, blah_83, blah_84,
   blah_85, blah_86, blah_87, blah_88, blah_89,
   blah_90, blah_91, blah_92, blah_93, blah_94,
   blah_95, blah_96, blah_97, blah_98, blah_99,
   blah_100, blah_101, blah_102, blah_103, blah_104,
   blah_105, blah_106, blah_107, blah_108, blah_109,
   blah_110, blah_111, blah_112, blah_113, blah_114,
   blah_115, blah_116, blah_117, blah_118, blah_119,
   blah_120, blah_121, blah_122, blah_123, blah_124,
   blah_125, blah_126, blah_127, blah_128, blah_129,
   blah_130, blah_131, blah_132, blah_133, blah_134,
   blah_135, blah_136, blah_137, blah_138, blah_139,
   blah_140, blah_141, blah_142, blah_143, blah_144,
   blah_145, blah_146, blah_147, blah_148, blah_149,
   blah_150, blah_151, blah_152, blah_153, blah_154,
   blah_155, blah_156, blah_157, blah_158, blah_159,
   blah_160, blah_161, blah_162, blah_163, blah_164,
   blah_165, blah_166, blah_167, blah_168, blah_169,
   blah_170, blah_171, blah_172, blah_173, blah_174,
   blah_175, blah_176, blah_177, blah_178, blah_179,
   blah_180, blah_181, blah_182, blah_183, blah_184,
   blah_185, blah_186, blah_187, blah_188, blah_189,
   blah_190, blah_191, blah_192, blah_193, blah_194,
   blah_195, blah_196, blah_197, blah_198, blah_199,
   blah_200, blah_201, blah_202, blah_203, blah_204,
   blah_205, blah_206, blah_207, blah_208, blah_209,
   blah_210, blah_211, blah_212, blah_213, blah_214,
   blah_215, blah_216, blah_217, blah_218, blah_219,
   blah_220, blah_221, blah_222, blah_223, blah_224,
   blah_225, blah_226, blah_227, blah_228, blah_229,
   blah_230, blah_231, blah_232, blah_233, blah_234,
   blah_235, blah_236, blah_237, blah_238, blah_239,
   blah_240, blah_241, blah_242, blah_243, blah_244,
   blah_245, blah_246, blah_247, blah_248, blah_249,
   blah_250, blah_251, blah_252, blah_253, blah_254,
   blah_255, blah_256, blah_257, blah_258, blah_259,
   blah_260, blah_261, blah_262, blah_263, blah_264,
   blah_265, blah_266, blah_267, blah_268, blah_269,
   blah_270, blah_271, blah_272, blah_273, blah_274,
   blah_275, blah_276, blah_277, blah_278, blah_279,
   blah_280, blah_281, blah_282, blah_283, blah_284,
   blah_285, blah_286, blah_287, blah_288, blah_289,
   blah_290, blah_291, blah_292, blah_293, blah_294,
   blah_295, blah_296, blah_297, blah_298, blah_299,
   blah_300, blah_301, blah_302, blah_303, blah_304,
   blah_305, blah_306, blah_307, blah_308, blah_309,
   blah_310, blah_311, blah_312, blah_313, blah_314,
   blah_315, blah_316, blah_317, blah_318, blah_319,
   blah_320, blah_321, blah_322, blah_323, blah_324,
   blah_325, blah_326, blah_327, blah_328, blah_329,
   blah_330, blah_331, blah_332, blah_333, blah_334,
   blah_335, blah_336, blah_337, blah_338, blah_339,
   blah_340, blah_341, blah_342, blah_343, blah_344,
   blah_345, blah_346, blah_347, blah_348, blah_349,
   blah_350, blah_351, blah_352, blah_353, blah_354,
	ce_first=blah_0,ce_last=blah_354
};

//-----------------------------------------------------------------------------------------
using namespace FIX8;

//-----------------------------------------------------------------------------------------
// run as: ctest --output-on-failure
//-----------------------------------------------------------------------------------------
TEST_CASE("anonymous entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<NineEnums, std::string_view>>
      ({
			{ NineEnums::One, "NineEnums::One" },
			{ NineEnums::Two, "NineEnums::Two" },
			{ NineEnums::Three, "NineEnums::Three" },
			{ NineEnums::Four, "NineEnums::Four" },
			{ NineEnums::Five, "NineEnums::Five" },
			{ NineEnums::Six, "NineEnums::Six" },
			{ NineEnums::Seven, "NineEnums::Seven" },
			{ NineEnums::Eight, "NineEnums::Eight" },
			{ NineEnums::Nine, "NineEnums::Nine" },
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<NineEnums1, std::string_view>>
      ({
			{ One, "One" },
			{ Two, "Two" },
			{ Three, "Three" },
			{ Four, "Four" },
			{ Five, "Five" },
			{ Six, "Six" },
			{ Seven, "Seven" },
			{ Eight, "Eight" },
			{ Nine, "Nine" },
      })
   };
	REQUIRE(conjure_enum<NineEnums>::entries == compentries);
	REQUIRE(conjure_enum<NineEnums1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("namespaced entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<TEST::NineEnums, std::string_view>>
      ({
			{ TEST::NineEnums::One, "TEST::NineEnums::One" },
			{ TEST::NineEnums::Two, "TEST::NineEnums::Two" },
			{ TEST::NineEnums::Three, "TEST::NineEnums::Three" },
			{ TEST::NineEnums::Four, "TEST::NineEnums::Four" },
			{ TEST::NineEnums::Five, "TEST::NineEnums::Five" },
			{ TEST::NineEnums::Six, "TEST::NineEnums::Six" },
			{ TEST::NineEnums::Seven, "TEST::NineEnums::Seven" },
			{ TEST::NineEnums::Eight, "TEST::NineEnums::Eight" },
			{ TEST::NineEnums::Nine, "TEST::NineEnums::Nine" },
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<TEST::NineEnums1, std::string_view>>
      ({
			{ TEST::One, "TEST::One" },
			{ TEST::Two, "TEST::Two" },
			{ TEST::Three, "TEST::Three" },
			{ TEST::Four, "TEST::Four" },
			{ TEST::Five, "TEST::Five" },
			{ TEST::Six, "TEST::Six" },
			{ TEST::Seven, "TEST::Seven" },
			{ TEST::Eight, "TEST::Eight" },
			{ TEST::Nine, "TEST::Nine" },
      })
   };
	REQUIRE(conjure_enum<TEST::NineEnums>::entries == compentries);
	REQUIRE(conjure_enum<TEST::NineEnums1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested anonymous entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<TEST1::NineEnums, std::string_view>>
      ({
			{ TEST1::NineEnums::One, "TEST1::NineEnums::One" },
			{ TEST1::NineEnums::Two, "TEST1::NineEnums::Two" },
			{ TEST1::NineEnums::Three, "TEST1::NineEnums::Three" },
			{ TEST1::NineEnums::Four, "TEST1::NineEnums::Four" },
			{ TEST1::NineEnums::Five, "TEST1::NineEnums::Five" },
			{ TEST1::NineEnums::Six, "TEST1::NineEnums::Six" },
			{ TEST1::NineEnums::Seven, "TEST1::NineEnums::Seven" },
			{ TEST1::NineEnums::Eight, "TEST1::NineEnums::Eight" },
			{ TEST1::NineEnums::Nine, "TEST1::NineEnums::Nine" },
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<TEST1::NineEnums1, std::string_view>>
      ({
			{ TEST1::One, "TEST1::One" },
			{ TEST1::Two, "TEST1::Two" },
			{ TEST1::Three, "TEST1::Three" },
			{ TEST1::Four, "TEST1::Four" },
			{ TEST1::Five, "TEST1::Five" },
			{ TEST1::Six, "TEST1::Six" },
			{ TEST1::Seven, "TEST1::Seven" },
			{ TEST1::Eight, "TEST1::Eight" },
			{ TEST1::Nine, "TEST1::Nine" },
      })
   };
	REQUIRE(conjure_enum<TEST1::NineEnums>::entries == compentries);
	REQUIRE(conjure_enum<TEST1::NineEnums1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested namespaced entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<TEST::TEST1::NineEnums, std::string_view>>
      ({
			{ TEST::TEST1::NineEnums::One, "TEST::TEST1::NineEnums::One" },
			{ TEST::TEST1::NineEnums::Two, "TEST::TEST1::NineEnums::Two" },
			{ TEST::TEST1::NineEnums::Three, "TEST::TEST1::NineEnums::Three" },
			{ TEST::TEST1::NineEnums::Four, "TEST::TEST1::NineEnums::Four" },
			{ TEST::TEST1::NineEnums::Five, "TEST::TEST1::NineEnums::Five" },
			{ TEST::TEST1::NineEnums::Six, "TEST::TEST1::NineEnums::Six" },
			{ TEST::TEST1::NineEnums::Seven, "TEST::TEST1::NineEnums::Seven" },
			{ TEST::TEST1::NineEnums::Eight, "TEST::TEST1::NineEnums::Eight" },
			{ TEST::TEST1::NineEnums::Nine, "TEST::TEST1::NineEnums::Nine" },
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<TEST::TEST1::NineEnums1, std::string_view>>
      ({
			{ TEST::TEST1::One, "TEST::TEST1::One" },
			{ TEST::TEST1::Two, "TEST::TEST1::Two" },
			{ TEST::TEST1::Three, "TEST::TEST1::Three" },
			{ TEST::TEST1::Four, "TEST::TEST1::Four" },
			{ TEST::TEST1::Five, "TEST::TEST1::Five" },
			{ TEST::TEST1::Six, "TEST::TEST1::Six" },
			{ TEST::TEST1::Seven, "TEST::TEST1::Seven" },
			{ TEST::TEST1::Eight, "TEST::TEST1::Eight" },
			{ TEST::TEST1::Nine, "TEST::TEST1::Nine" },
      })
   };
	REQUIRE(conjure_enum<TEST::TEST1::NineEnums>::entries == compentries);
	REQUIRE(conjure_enum<TEST::TEST1::NineEnums1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("anonymous enum")
{
	REQUIRE(conjure_type<NineEnums>::as_string_view() == "NineEnums");
	REQUIRE(conjure_type<NineEnums1>::as_string_view() == "NineEnums1");
	REQUIRE(conjure_type<TEST::NineEnums>::as_string_view() == "TEST::NineEnums");
	REQUIRE(conjure_type<TEST::NineEnums1>::as_string_view() == "TEST::NineEnums1");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested anonymous enum")
{
	REQUIRE(conjure_type<TEST1::NineEnums>::as_string_view() == "TEST1::NineEnums");
	REQUIRE(conjure_type<TEST1::NineEnums1>::as_string_view() == "TEST1::NineEnums1");
	REQUIRE(conjure_type<TEST::TEST1::NineEnums>::as_string_view() == "TEST::TEST1::NineEnums");
	REQUIRE(conjure_type<TEST::TEST1::NineEnums1>::as_string_view() == "TEST::TEST1::NineEnums1");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested structured enum")
{
	REQUIRE(test::util::countOf<test::rsp::gd::AFrame::PreFrames>() == 1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("anonymous type")
{
	REQUIRE(conjure_type<foobat>::as_string_view() == "foobat");
	REQUIRE(conjure_type<TEST::foo>::as_string_view() == "TEST::foo");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested type")
{
	REQUIRE(conjure_type<TEST1::foo>::as_string_view() == "TEST1::foo");
	REQUIRE(conjure_type<TEST::TEST1::foo>::as_string_view() == "TEST::TEST1::foo");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum limits")
{
	REQUIRE(conjure_enum<numbers>::get_enum_min_value() == conjure_enum<numbers>::get_actual_enum_min_value());
	REQUIRE(conjure_enum<numbers>::get_enum_max_value() == conjure_enum<numbers>::get_actual_enum_max_value());
	REQUIRE(conjure_enum<numbers>::get_enum_min_value() == 0);
	REQUIRE(conjure_enum<numbers>::get_enum_max_value() == 354);
}

