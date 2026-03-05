#ifndef INC_FONT_HPP_
#define INC_FONT_HPP_

#include "etl/array_view.h"

class Font {
public:
	constexpr etl::array_view<const uint8_t> operator[](const char& c) const {
		return etl::array_view<const uint8_t>{s_font[c - 32]};
	}

	static Font& GetInstance() {
		static Font f;
		return f;
	}

private:
	static const uint8_t s_font[][8];

	Font() = default;
	~Font() = default;

	Font(Font&) = delete;
	void operator=(const Font&) = delete;
};

#endif /* INC_FONT_HPP_ */
