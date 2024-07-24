#ifndef MCL_UNICODE_CHARACTER_PROPERTY_H
#define MCL_UNICODE_CHARACTER_PROPERTY_H

#include <cstdint>
#include <string>
#include <vector>
#include <boost/preprocessor.hpp>

#include "unicode-config.h"

namespace mcl {
	struct Point {
		uint32_t point;
		std::string name;

		enum GeneralCategory : uint8_t {
			kLetterUppercase = 0x00,
			kLetterLowercase,
			kLetterTitlecase,
			kLetterModifier,
			kLetterOther,

			kMarkNonspacing = 0x10,
			kMarkSpacingCombining,
			kMarkEnclosing,

			kNumberDecimalDigit = 0x20,
			kNumberLetter,
			kNumberOther,

			kPunctuationConnector = 0x30,
			kPunctuationDash,
			kPunctuationOpen,
			kPunctuationClose,
			kPunctuationInitialQuote,
			kPunctuationFinalQuote,
			kPunctuationOther,

			kSymbolMath = 0x40,
			kSymbolCurrency,
			kSymbolModifier,
			kSymbolOther,
			kSeparatorSpace,
			kSeparatorLine,
			kSeparatorParagraph,

			kOtherControl = 0x50,
			kOtherFormat,
			kOtherSurrogate,
			kOtherPrivateUse,
			kOtherNotAssigned,
		} category;

		uint8_t combining;

		/*
		enum class Bidi {
			L, LRE, LRO,
			R, AL, RLE, RLO,
			PDF,
			EN, ES, ET,
			AN,
			CS,
			NSM,
			BN,
			B, S,
			WS,
			ON
		} bidi;
		*/

		std::string bidi;


		struct Decomp {
			enum class Type {
				canonical,
				font, noBreak,
				initialForm, medialForm, finalForm, isolatedForm,
				circle, super, sub,
				vertical,
				wide, narrow, small,
				square,
				fraction,
				compat
			} type;

			std::vector<uint32_t> seq;
		} decomp;

		struct Numeric {
			enum class Type {
				decimal, digit, numeric, none
			} type;

			long num = 0;
			long den = 1;
		} numeric;

		bool mirrored;

		std::string alias;

		std::string comment;

		uint32_t toupper;
		uint32_t tolower;
		uint32_t totitle;
	};

#define FST(x, y) x
#define SND(x, y) y

#define APPEND_CASE(_, acc, x) acc case FST x: \
	return SND x; break;
#define ENUM_TO_PAIR(_, scope, e) \
	(scope::e, BOOST_PP_CAT(BOOST_PP_STRINGIZE(e), s))
#define SHOW_ENUM(scope, name, ...) \
	constexpr std::string show ## name(scope::name e) { \
		using std::literals::string_literals::operator""s; \
		using enum scope::name; \
		switch (e) { \
			BOOST_PP_SEQ_FOLD_LEFT(APPEND_CASE, , \
				BOOST_PP_SEQ_TRANSFORM(ENUM_TO_PAIR, scope::name, \
					BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__) \
				) \
			) \
		} \
		return "<unknown value>"s; \
	}
	// constexpr operator std::string(scope::name e) { return scope::name(e); }



	SHOW_ENUM(Point, GeneralCategory,
			kLetterUppercase,
			kLetterLowercase,
			kLetterTitlecase,
			kLetterModifier,
			kLetterOther,
			kMarkNonspacing,
			kMarkSpacingCombining,
			kMarkEnclosing,

			kNumberDecimalDigit,
			kNumberLetter,
			kNumberOther,

			kPunctuationConnector,
			kPunctuationDash,
			kPunctuationOpen,
			kPunctuationClose,
			kPunctuationInitialQuote,
			kPunctuationFinalQuote,
			kPunctuationOther,

			kSymbolMath,
			kSymbolCurrency,
			kSymbolModifier,
			kSymbolOther,
			kSeparatorSpace,
			kSeparatorLine,
			kSeparatorParagraph,

			kOtherControl,
			kOtherFormat,
			kOtherSurrogate,
			kOtherPrivateUse,
			kOtherNotAssigned
	)

	SHOW_ENUM(Point::Decomp, Type,
			canonical,
			font, noBreak,
			initialForm, medialForm, finalForm, isolatedForm,
			circle, super, sub,
			vertical,
			wide, narrow, small,
			square,
			fraction,
			compat
	)

	SHOW_ENUM(Point::Numeric, Type,
			decimal, digit, numeric, none
	)

#undef APPEND_CASE
#undef SHOW_ENUM

	extern const Point unicodeData[UNICODE_DATA_LENGTH];
}


#endif

