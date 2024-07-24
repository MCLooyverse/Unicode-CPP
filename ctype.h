#ifndef MCL_UNICODE_CHARACTER_TYPE_H
#define MCL_UNICODE_CHARACTER_TYPE_H

#include "UnicodeData.h"
#include <iterator>


namespace mcl {

	/*constexpr*/ auto getData(const uint32_t p) -> const mcl::Point&
	{
		// TODO: Handle non-extant code point
		uint32_t i = p < std::size(unicodeData) ? p : std::size(unicodeData);
		while (unicodeData[i].point != p)
		{
			int diff = p - unicodeData[i].point;
			if (diff > 0)
			{
				if (i + diff < std::size(unicodeData))
					i += diff;
				else
					i += (std::size(unicodeData) - i) / 2; // BAD
			}
			else
			{
				if (i < -diff)
					i /= 2;
				else
					i += diff;
			}
		}

		return unicodeData[i];
	}

	bool isalpha(const uint32_t p) {
		auto cat = getData(p).category;

		return cat >> 4 == 0;
	}


}


#endif

