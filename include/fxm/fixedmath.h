#pragma once

#include "fixedtype.h"

namespace Mathfx
{
	template <typename T, int F> constexpr Fixed<T, F> SafeAdd(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::SafeAdd(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> SafeSub(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::SafeSub(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> SafeMul(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::SafeMul(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> SafeDiv(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::SafeDiv(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> SafeMod(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::SafeMod(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> FastAdd(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::FastAdd(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> FastSub(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::FastSub(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> FastMul(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::FastMul(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> FastDiv(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::FastDiv(x, y); }
	template <typename T, int F> constexpr Fixed<T, F> FastMod(Fixed<T, F> x, Fixed<T, F> y) { return Fixed<T, F>::FastMod(x, y); }

	template <typename T, int F>
	constexpr bool ApproxEqual(Fixed<T, F> x, Fixed<T, F> y, int ignoreBits = Fixed<T, F>::EpsilonBits)
	{
		using fixed = Fixed<T, F>;
		using uraw = typename fixed::uraw;

		FXMATH_ASSERT(ignoreBits >= 0 && ignoreBits < fixed::NumBits && "Invalid argument.");

		uraw mask = static_cast<uraw>(-1) << ignoreBits;
		return ((x.rawValue & mask) == (y.rawValue & mask));
	}

	template <typename T, int F>
	constexpr bool ApproxZero(Fixed<T, F> x, int ignoreBits = Fixed<T, F>::EpsilonBits)
	{
		using fixed = Fixed<T, F>;
		return ApproxEqual(x, fixed::Zero, ignoreBits);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Min(Fixed<T, F> x, Fixed<T, F> y)
	{
		return (x < y) ? x : y;
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Max(Fixed<T, F> x, Fixed<T, F> y)
	{
		return (x > y) ? x : y;
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Clamp(Fixed<T, F> x, Fixed<T, F> min, Fixed<T, F> max)
	{
		return (x < min) ? min : (x > max ? max : x);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Clamp01(Fixed<T, F> x)
	{
		return Clamp(x, 0, 1);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Repeat(Fixed<T, F> x, Fixed<T, F> length)
	{
		using fixed = Fixed<T, F>;
		return Clamp(x - Floor(x / length) * length, fixed::Zero, length);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Sign(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;

		if (x.rawValue == 0) return fixed::Zero;
		if (x.rawValue < 0) return fixed::NegOne;
		return fixed::One;
	}

	template <typename T, int F>
	constexpr int SignToInt(Fixed<T, F> x)
	{
		if (x.rawValue < 0) return -1;
		if (x.rawValue > 0) return 1;
		return 0;
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Abs(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;

		if (x.rawValue == fixed::RawMinValue)
		{
			return fixed::MaxValue;
		}

		raw mask = x.rawValue >> fixed::SignShift;
		return Fixed<T, F>((x.rawValue + mask) ^ mask);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> FastAbs(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;
		raw mask = x.rawValue >> fixed::SignShift;
		return fixed((x.rawValue + mask) ^ mask);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Fraction(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		return fixed(x.rawValue & fixed::FractionMask);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Floor(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		return fixed(x.rawValue & fixed::WholeMask);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Ceiling(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		bool isWholeNumber = (x.rawValue & fixed::FractionMask) == 0;
		return isWholeNumber ? x : SafeAdd(Floor(x), fixed::One);
	}

	template <typename T, int F>
	constexpr Fixed<T, F> Round(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;

		raw fraction = x.rawValue & fixed::FractionMask;
		fixed whole = Floor(x);
		if (fraction < fixed::RawHalf)
		{
			return whole;
		}
		if (fraction > fixed::RawHalf)
		{
			return SafeAdd(whole, fixed::One);
		}
		// If exactly half go to nearest non-zero even number or 1
		return (whole.rawValue != 0 && (whole.rawValue & fixed::RawOne) == 0) ? whole : SafeAdd(whole, fixed::One);
	}

	template <typename T, int F>
	constexpr int FloorToInt(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		return static_cast<int>(Floor(x).rawValue >> fixed::FractionShift);
	}

	template <typename T, int F>
	constexpr int CeilingToInt(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		return static_cast<int>(Ceiling(x).rawValue >> fixed::FractionShift);
	}

	template <typename T, int F>
	constexpr int RoundToInt(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		return static_cast<int>(Round(x).rawValue >> fixed::FractionShift);
	}

	template <typename T, int F>
	Fixed<T, F> Pow2(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		if (x.rawValue == 0)
		{
			return fixed::One;
		}

		bool isNegative = x.rawValue < 0;
		if (isNegative)
		{
			x = -x;
		}

		if (x == fixed::One)
		{
			return isNegative ? fixed::One / fixed::Int(2) : fixed::Int(2);
		}
		if (x >= fixed::Log2Max)
		{
			return isNegative ? fixed::OneOverMaxValue : fixed::MaxValue;
		}
		if (x <= fixed::Log2Min)
		{
			return isNegative ? fixed::MaxValue : fixed::Zero;
		}

		int integerPart = FloorToInt(x);
		fixed frac = Fraction(x);

		fixed result = fixed::One;
		fixed term = fixed::One;
		int i = 1;
		while (term.rawValue != 0)
		{
			term = FastMul(FastMul(frac, term), fixed::Ln2) / fixed::Int(i);
			result += term;
			i++;
		}

		result = fixed(result.rawValue << integerPart);
		if (isNegative)
		{
			result = fixed::One / result;
		}

		return result;
	}

	template <typename T, int F>
	Fixed<T, F> Log2(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;
		using uraw = typename fixed::uraw;

		FXMATH_ASSERT(x.rawValue > 0 && "Invalid argument.");

		raw b = static_cast<uraw>(1) << (fixed::FractionShift - 1);
		raw y = 0;

		raw rawX = x.rawValue;
		while (rawX < fixed::RawOne)
		{
			rawX <<= 1;
			y -= fixed::RawOne;
		}

		while (rawX >= (fixed::RawOne << 1))
		{
			rawX >>= 1;
			y += fixed::RawOne;
		}

		fixed z(rawX);

		for (int i = 0; i < fixed::FractionShift; ++i)
		{
			z = FastMul(z, z);
			if (z.rawValue >= (fixed::RawOne << 1))
			{
				z = fixed(z.rawValue >> 1);
				y += b;
			}
			b >>= 1;
		}

		return fixed(y);
	}

	template <typename T, int F>
	Fixed<T, F> Ln(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		return FastMul(Log2(x), fixed::Ln2);
	}

	template <typename T, int F>
	Fixed<T, F> Pow(Fixed<T, F> base, Fixed<T, F> exp)
	{
		using fixed = Fixed<T, F>;

		if (base == fixed::One)
		{
			return fixed::One;
		}
		if (exp.rawValue == 0)
		{
			return fixed::One;
		}
		if (base.rawValue == 0)
		{
			FXMATH_ASSERT(exp.rawValue > 0 && "Divide by zero.");
			return fixed::Zero;
		}

		fixed lg2 = Log2(base);
		fixed prod = SafeMul(exp, lg2);
		fixed result = Pow2(prod);
		return result;
	}

	template <typename T, int F>
	Fixed<T, F> Sqrt(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;
		using uraw = typename fixed::uraw;

		raw xr = x.rawValue;
		FXMATH_ASSERT(xr >= 0 && "Invalid argument.");

		uraw num = static_cast<uraw>(xr);
		uraw result = static_cast<uraw>(0);

		uraw bit = static_cast<uraw>(1) << (fixed::NumBits - 2);

		while (bit > num)
		{
			bit >>= 2;
		}

		while (bit != 0)
		{
			if (num >= result + bit)
			{
				num -= result + bit;
				result = (result >> 1) + bit;
			}
			else
			{
				result >>= 1;
			}
			bit >>= 2;
		}

		if (num > (static_cast<uraw>(1) << fixed::HalfNumBits) - 1)
		{
			num -= result;
			num = (num << fixed::HalfNumBits) - fixed::RawHalf;
			result = (result << fixed::HalfNumBits) + fixed::RawHalf;
		}
		else
		{
			num <<= fixed::HalfNumBits;
			result <<= fixed::HalfNumBits;
		}

		bit = static_cast<uraw>(1) << (fixed::HalfNumBits - 2);

		while (bit != 0)
		{
			if (num >= result + bit)
			{
				num -= result + bit;
				result = (result >> 1) + bit;
			}
			else
			{
				result >>= 1;
			}
			bit >>= 2;
		}


		if (num > result)
		{
			++result;
		}

		return fixed(static_cast<raw>(result));
	}

	/**
	 * \brief Calculates Fixed<T, F> point square root more than twice as afast as Sqrt but at the cost of some accuracy for large numbers.
	 * For small numbers should be accurate to at least 0.0001 for Fixed<T, F>64 and probably smaller than that for a lot of cases.
	 * For large numbers (> 1e9) gets less accurate, mostly within ~10 or so but sometimes off by as much as a couple thousand.
	 * \param x Number to take square root of.
	 * \return Square root of \p x
	 */
	template <typename T, int F>
	Fixed<T, F> FastSqrt(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;
		using uraw = typename fixed::uraw;

		raw xr = x.rawValue;

		if (xr == 0)
		{
			return fixed::Zero;
		}

		FXMATH_ASSERT(xr > 0 && "Invalid argument.");

		uint64_t t, q, b, r;
		r = static_cast<uraw>(xr);
		b = (0x4000000000000000ULL >> (64 - fixed::NumBits));
		q = 0;
		while (b > 0x40)
		{
			t = q + b;
			if (r >= t)
			{
				r -= t;
				q = t + b; // equivalent to q += 2*b
			}
			r <<= 1;
			b >>= 1;
		}
		q >>= (fixed::NumBits / 4);
		raw qr = static_cast<raw>(q);
		fixed result(qr);
		return result;
	}

	template <>
	Fixed<int32_t, 16> FastSqrt(Fixed<int32_t, 16> x)
	{
		using fixed = Fixed<int32_t, 16>;
		using raw = fixed::raw;
		using uraw = fixed::uraw;

		raw xr = x.rawValue;

		if (xr == 0)
		{
			return fixed::Zero;
		}

		FXMATH_ASSERT(xr > 0 && "Invalid argument.");

		uint32_t t, q, b, r;
		r = static_cast<uraw>(xr);
		b = 0x40000000;
		q = 0;
		while (b > 0x40)
		{
			t = q + b;
			if (r >= t)
			{
				r -= t;
				q = t + b;
			}
			r <<= 1;
			b >>= 1;
		}
		q >>= 8;
		raw qr = static_cast<raw>(q);
		fixed result(qr);
		return result;
	}

	namespace internal
	{
		template <typename T, int F>
		Fixed<T, F> ClampSinToQuadrant(Fixed<T, F> x, bool& flipH, bool& flipV)
		{
			using fixed = Fixed<T, F>;
			using raw = typename fixed::raw;

			raw rawLargePi = fixed::LargePi.rawValue;

			raw clampedTwoPi = x.rawValue;
			for (int i = 0; i < fixed::LargePiBits; ++i)
			{
				clampedTwoPi %= (rawLargePi >> i);
			}
			if (x.rawValue < 0)
			{
				clampedTwoPi += fixed::TwoPi.rawValue;
			}

			raw clampedPi = clampedTwoPi;
			while (clampedPi >= fixed::Pi.rawValue) clampedPi -= fixed::Pi.rawValue;

			raw clampedPiOver2 = clampedPi;
			while (clampedPiOver2 >= fixed::PiOver2.rawValue) clampedPiOver2 -= fixed::PiOver2.rawValue;

			flipV = clampedTwoPi >= fixed::Pi.rawValue;
			flipH = clampedPi >= fixed::PiOver2.rawValue;

			return fixed(clampedPiOver2);
		}
	}

	template <typename T, int F>
	Fixed<T, F> Sin(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;

		bool flipH, flipV;
		fixed clamped = internal::ClampSinToQuadrant(x, flipH, flipV);

		fixed scaledIndex = FastMul(clamped, fixed::GetLutInterval());
		fixed roundedIndex = Round(scaledIndex);
		fixed indexError = FastSub(scaledIndex, roundedIndex);

		int index = static_cast<int>(roundedIndex);
		int nextIndex = index + SignToInt(indexError);
		fixed nearest = fixed::SinLookupTable[flipH ? fixed::SinLookupTable.size() - 1 - index : index];
		fixed nextNearest = fixed::SinLookupTable[flipH ? fixed::SinLookupTable.size() - 1 - nextIndex : nextIndex];

		raw delta = FastMul(indexError, FastAbs(FastSub(nearest, nextNearest))).rawValue;
		raw interpolated = nearest.rawValue + (flipH ? -delta : delta);
		raw finalValue = flipV ? -interpolated : interpolated;
		return fixed(finalValue);
	}

	template <typename T, int F>
	Fixed<T, F> FastSin(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;

		bool flipH, flipV;
		fixed clamped = internal::ClampSinToQuadrant(x, flipH, flipV);

		size_t scaledIndex = static_cast<size_t>(FastMul(clamped, fixed::GetLutInterval()));
		if (scaledIndex >= fixed::TrigLookupTableSize)
		{
			scaledIndex = fixed::TrigLookupTableSize - 1;
		}
		fixed nearest = fixed::SinLookupTable[flipH ? fixed::SinLookupTable.size() - 1 - scaledIndex : scaledIndex];
		fixed finalValue = flipV ? -nearest : nearest;
		return finalValue;
	}

	template <typename T, int F>
	Fixed<T, F> Cos(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;
		raw xr = x.rawValue;
		raw angle = xr + (xr > 0 ? -fixed::Pi.rawValue - fixed::PiOver2.rawValue : fixed::PiOver2.rawValue);
		return Sin(fixed(angle));
	}

	template <typename T, int F>
	Fixed<T, F> FastCos(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;

		raw xr = x.rawValue;
		raw angle = xr + (xr > 0 ? -fixed::Pi.rawValue - fixed::PiOver2.rawValue : fixed::PiOver2.rawValue);
		return FastSin(fixed(angle));
	}

	template <typename T, int F>
	Fixed<T, F> Tan(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;

		raw clampedPi = x.rawValue % fixed::Pi.rawValue;
		bool flip = false;
		if (clampedPi < 0)
		{
			flip = true;
			clampedPi = -clampedPi;
		}

		if (clampedPi > fixed::PiOver2.rawValue)
		{
			flip = !flip;
			clampedPi = fixed::PiOver2.rawValue - (clampedPi - fixed::PiOver2.rawValue);
		}

		fixed clamped(clampedPi);

		fixed scaledIndex = FastMul(clamped, fixed::GetLutInterval());
		fixed roundedIndex = Round(scaledIndex);
		fixed indexError = FastSub(scaledIndex, roundedIndex);

		int index = static_cast<int>(roundedIndex);
		int nextIndex = index + SignToInt(indexError);
		fixed nearest = fixed::TanLookupTable[index];
		fixed nextNearest = fixed::TanLookupTable[nextIndex];

		raw delta = FastMul(indexError, FastAbs(FastSub(nearest, nextNearest))).rawValue;
		raw interpolated = nearest.rawValue + delta;
		raw finalValue = flip ? -interpolated : interpolated;
		return Fixed<T, F>(finalValue);
	}

	template <typename T, int F>
	Fixed<T, F> Atan(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;

		if (x == fixed::Zero)
		{
			return fixed::Zero;
		}

		bool isNegative = x < fixed::Zero;
		if (isNegative)
		{
			x = -x;
		}

		constexpr fixed two = fixed(fixed::RawOne + fixed::RawOne);
		constexpr fixed three = fixed(fixed::RawOne + fixed::RawOne + fixed::RawOne);

		bool invert = x > fixed::One;
		if (invert)
		{
			x = fixed::One / x;
		}

		fixed result = fixed::One;
		fixed term = fixed::One;

		fixed xSq = x * x;
		fixed xSq2 = xSq * two;
		fixed xSqPlusOne = xSq + fixed::One;
		fixed xSq12 = xSqPlusOne * two;
		fixed dividend = xSq2;
		fixed divisor = xSqPlusOne * three;

		for (int i = 0; i < (fixed::FractionShift - 4); ++i)
		{
			term *= dividend / divisor;
			result += term;
			dividend += xSq2;
			divisor += xSq12;

			if (term == fixed::Zero)
			{
				break;
			}
		}

		result = result * x / xSqPlusOne;

		if (invert)
		{
			result = fixed::PiOver2 - result;
		}

		if (isNegative)
		{
			result = -result;
		}
		return result;
	}

	template <typename T, int F>
	Fixed<T, F> Acos(Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;

		FXMATH_ASSERT(x >= fixed::NegOne && x <= fixed::One && "Argument out of range.");

		if (x == fixed::Zero)
		{
			return fixed::PiOver2;
		}

		Fixed<T, F> result = Atan(Sqrt(fixed::One - x * x) / x);
		if (x < fixed::Zero)
		{
			return result + fixed::Pi;
		}
		return result;
	}

	template <typename T, int F>
	Fixed<T, F> Asin(Fixed<T, F> x)
	{
		return Fixed<T, F>::PiOver2 - Acos(x);
	}

	template <typename T, int F>
	Fixed<T, F> Atan2(Fixed<T, F> y, Fixed<T, F> x)
	{
		using fixed = Fixed<T, F>;
		using raw = typename fixed::raw;

		raw yr = y.rawValue;
		raw xr = x.rawValue;

		if (xr == 0)
		{
			if (yr > 0)
			{
				return fixed::PiOver2;
			}
			if (yr == 0)
			{
				return fixed::Zero;
			}
			return -fixed::PiOver2;
		}

		fixed atan;
		fixed z = y / x;

		fixed base = fixed::Float(0.28);

		// Overflow
		if (SafeAdd(fixed::One, SafeMul(SafeMul(base, z), z)) == fixed::MaxValue)
		{
			return y < fixed::Zero ? -fixed::PiOver2 : fixed::PiOver2;
		}

		if (Abs(z) < fixed::One)
		{
			atan = z / (fixed::One + SafeMul(base, SafeMul(z, z)));
			if (xr < 0)
			{
				if (yr < 0)
				{
					return SafeSub(atan, fixed::Pi);
				}
				return SafeAdd(atan, fixed::Pi);
			}
		}
		else
		{
			atan = SafeSub(fixed::PiOver2, (z / (SafeAdd(SafeMul(z, z), base))));
			if (yr < 0)
			{
				return SafeSub(atan, fixed::Pi);
			}
		}
		return atan;
	}

	template <typename T, int F>
	Fixed<T, F> Lerp(Fixed<T, F> a, Fixed<T, F> b, Fixed<T, F> t)
	{
		return a + (b - a) * t;
	}

	template <typename T, int F>
	Fixed<T, F> LerpClamped(Fixed<T, F> a, Fixed<T, F> b, Fixed<T, F> t)
	{
		return a + (b - a) * Clamp01(t);
	}

	template <typename T, int F>
	Fixed<T, F> LerpInverse(Fixed<T, F> a, Fixed<T, F> b, Fixed<T, F> value)
	{
		using fixed = Fixed<T, F>;
		if (a == b)
		{
			return fixed::Zero;
		}
		fixed t = (value - a) / (b - a);
		return Clamp01(t);
	}

	template <typename T, int F>
	Fixed<T, F> MoveTowards(Fixed<T, F> value, Fixed<T, F> target, Fixed<T, F> delta, Fixed<T, F>& amountMoved)
	{
		using fixed = Fixed<T, F>;

		fixed result = target;
		if (value < target)
		{
			result = Min(value + delta, target);
		}
		else if (value > target)
		{
			result = Max(value - delta, target);
		}

		amountMoved = result - value;
		return result;
	}

	template <typename T, int F>
	Fixed<T, F> TrueModulo(Fixed<T, F> a, Fixed<T, F> b)
	{
		fixed r = a % b;
		return (r < 0) ? r + b : r;
	}
}
