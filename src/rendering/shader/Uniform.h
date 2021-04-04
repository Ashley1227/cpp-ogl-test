#pragma once

struct Uniform {
	const int location;

	operator const int() const {
		return location;
	}
};

