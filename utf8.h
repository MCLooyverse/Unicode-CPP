#ifndef MCL_UNICODE_UTF8_DEFINITIONS_H
#define MCL_UNICODE_UTF8_DEFINITIONS_H


namespace mcl::utf8 {
	struct Unit {
		enum class Type : uint8_t {
			ascii, trailing,
			head2, head3, head4,
			invalid
		} type;
		uint8_t val;

		[[gnu::const]]
		constexpr static uint8_t tcast(Type t)
		{
			return static_cast<uint8_t>(t);
		}
		[[gnu::const]]
		constexpr static Type tcast(uint8_t t)
		{
			return static_cast<Type>(t);
		}

		[[gnu::const]]
		constexpr static int8_t units(Type v)
		{
			switch (v)
			{
				case Type::ascii: return 1;
				case Type::head2: return 2;
				case Type::head3: return 3;
				case Type::head4: return 4;
				default: return -1;
			}
		}

		[[gnu::pure]]
		constexpr int8_t units() const
		{
			return units(this->type);
		}


		[[gnu::pure]]
		constexpr bool test() const {
			return type != Type::invalid;
		}
		[[gnu::pure]]
		constexpr bool trailing() const {
			return type == Type::trailing;
		}
	};

	[[gnu::const]]
	constexpr inline Unit parseByte(uint8_t b)
	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
		auto i = std::min<uint8_t>(std::countl_one(b), 5);
		return {
			//static_cast<Unit::Type>(i),
			(Unit::Type)i,
			b & (0xFF >> i)
		};
#pragma GCC diagnostic pop
	}

	/*
	 * Represents a sequence of UTF-8 units.
	 * N.B.: Can represent an *invalid* sequence.
	 * For example: `110'00000 0'0000000`.  `.test()` the sequence before you
	 * assume it is valid, or get its code `.point()` value.
	 */
	struct CodeSeq {
		Unit seq[4];
		uint8_t len;

		// CodeSeq(std::initializer_list<Unit> s, uint8_t l) : seq{s}, len{l} { }

		[[gnu::pure]]
		bool test() const
		{
			if (!len)
				return 0;

			switch (seq->type)
			{
				case Unit::Type::ascii:
					return 1;
				case Unit::Type::trailing:
				case Unit::Type::invalid:
					return 0;
				case Unit::Type::head2:
					return len == 2
						&& seq[1].trailing();
				case Unit::Type::head3:
					return len == 3
						&& seq[1].trailing()
						&& seq[2].trailing();
				case Unit::Type::head4:
					return len == 4
						&& seq[1].trailing()
						&& seq[2].trailing()
						&& seq[3].trailing();
			}

			std::unreachable();
		}

		[[gnu::pure]]
		uint32_t point() const
		{
			if (!this->test())
				throw std::domain_error("Cannot interpret invalid UTF-8 sequence");

			uint32_t out = 0;
			for (uint8_t i = 0; i < len; ++i)
				(out <<= 6) |= seq[i].val;
			return out;
		}
	};

	[[gnu::pure]]
	inline CodeSeq parseSeq(uint8_t* b, uint8_t* e)
	{
		if (b >= e)
			return CodeSeq{ {}, 0 };

		CodeSeq out{ .seq{ parseByte(*b++), {}, {}, {} }, .len = 1 };
		out.len = out.seq->units();

		if (out.len < 1)
			throw std::domain_error("Cannot parse UTF-8 sequence with unusable start byte");

		for (int i = 1; i < out.len && b < e; ++i)
			out.seq[i] = parseByte(*b++);

		return out;
	}

	/* Assumes full sequence is in valid memory */
	[[gnu::pure]]
	inline CodeSeq parseSeq_Unbounded(uint8_t* b)
	{
		CodeSeq out{ .seq{ parseByte(*b++), {}, {}, {} }, .len = 1 };
		out.len = out.seq->units();

		if (out.len < 1)
			throw std::domain_error("Cannot parse UTF-8 sequence with unusable start byte");

		for (int i = 1; i < out.len; ++i)
			out.seq[i] = parseByte(*b++);

		return out;
	}

}


#endif

