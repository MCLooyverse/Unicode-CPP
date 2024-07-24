#pragma once

[[gnu::const]]
std::string utf8(uint32_t c)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
	if (c < 0x80)
		return { (char)c };
	else if (c < 0x800)
		return { {
			0b110'00000 | c >> 6,
			0b10'000000 | c & 0x3F
		} };
	else if (c < 0x10000)
		return { {
			0b1110'0000 | c >> 12,
			0b10'000000 | (c >> 6) & 0x3F,
			0b10'000000 | c & 0x3F
		} };
	else if (c < 0x200000)
		return { {
			0b11110'000 | c >> 18,
			0b10'000000 | (c >> 12) & 0x3F,
			0b10'000000 | (c >> 6) & 0x3F,
			0b10'000000 | c & 0x3F
		} };
	else
		throw std::domain_error("Invalid Unicode code point");
#pragma GCC diagnostic pop
}
