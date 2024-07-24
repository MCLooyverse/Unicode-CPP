#ifndef MCL_UNICODE_UTF8_ITERATOR_H
#define MCL_UNICODE_UTF8_ITERATOR_H

#include <utility>
#include "utf8.h"


namespace mcl::utf8 {
	struct iterator {
		uint8_t* p;

		[[gnu::pure]]
		uint32_t operator*() const
		{
			return parseSeq_Unbounded(p).point();
		}

		iterator& operator++()
		{
			p += parseSeq_Unbounded(p).len;
			return *this;
		}

		iterator operator++(int)
		{
			auto op = p;
			++*this;
			return { op };
		}

		[[gnu::pure]]
		bool operator<(uint8_t* q) const
		{
			return p < q;
		}

		[[gnu::pure]]
		constexpr bool bound_test(uint8_t* e) const
		{
			return p + parseByte(*p).units() < e;
		}
	};

	struct bounded_iterator {
		uint8_t* p;
		uint8_t* e;

		[[gnu::pure]]
		uint32_t operator*() const
		{
			return parseSeq(p, e).point();
		}

		bounded_iterator& operator++()
		{
			p += parseSeq(p, e).len;
			return *this;
		}

		bounded_iterator operator++(int)
		{
			auto op = p;
			++*this;
			return { op };
		}

		[[gnu::pure]]
		bool operator<(uint8_t* q) const
		{
			return p < q;
		}
	};

	struct bounded_const_iterator {
		uint8_t* p;
		uint8_t* e;

		[[gnu::pure]]
		uint32_t operator*() const
		{
			return parseSeq(p, e).point();
		}

		bounded_const_iterator& operator++()
		{
			p += parseSeq(p, e).len;
			return *this;
		}

		bounded_const_iterator operator++(int)
		{
			auto op = p;
			++*this;
			return { op };
		}

		[[gnu::pure]]
		bool operator<(uint8_t* q) const
		{
			return p < q;
		}

		[[gnu::pure]]
		bool operator!=(const bounded_const_iterator& x)
		{
			return this->p != x.p;
		}
	};
}


#endif

