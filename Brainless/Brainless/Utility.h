#ifndef INCLUDED_UTILITY_H
#define INCLUDED_UTILITY_H

class Utility
{
public:

	template<typename TType> static TType clampValue(TType value, TType min, TType max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;
		else
			return value;
	};
};

#endif