#ifndef MCL_UTF8_STRING_VIEW_H
#define MCL_UTF8_STRING_VIEW_H

#include <cstdint>
#include "iterator.h"

namespace mcl {
	struct utf8_view {
		uint8_t* p;
		size_t sz;


		constexpr utf8::bounded_const_iterator begin() const {
			return { p, p + sz };
		}
		constexpr utf8::bounded_const_iterator cbegin() const {
			return { p, p + sz };
		}
		constexpr utf8::bounded_const_iterator end() const {
			return { e + sz, e + sz };
		}
		constexpr utf8::bounded_const_iterator cend() const {
			return { e + sz, e + sz };
		}
	};
}


#endif

