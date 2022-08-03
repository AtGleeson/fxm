#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>
#include <cassert>
#include <bit>
#include <numbers>
#include <array>
#include <cmath>

#ifndef FXMATH_ASSERT
#define FXMATH_ASSERT(x) {		\
		if (!(x)) {			\
			__debugbreak();		\
		}						\
		assert(x);				\
	}
#endif

template <typename T, T(*Proj)(int, size_t), int... Is>
constexpr auto MakeLookupTableHelper(std::integer_sequence<int, Is...>)
{
#pragma warning(push)
#pragma warning(disable : 6262)
	return std::array<T, sizeof...(Is)>({ Proj(Is, sizeof...(Is))... });
#pragma warning(pop)
}

/**
 * \brief Compile time creation of a lookup table of size \p Size containing elements of type \p T.
 * Each element of the table is passed into the provided projection function \p Proj
 * which takes the current index of the table and the length of the table and return the type stored in the table.
 * \tparam T Type to store in the lookup table
 * \tparam Proj Projection function to apply to every index in the lookup table.
 * \tparam Size Number of elements in the lookup table.
 * \return std::array<T, Size> where every element is the result of \p Proj applied to the index of the element.
 */
template <typename T, T(*Proj)(int, size_t), size_t Size>
constexpr auto MakeLookupTable()
{
	// std::make_integer_sequence<int, Size> represents a compile-time array in the form of { 0, 1, 2, 3, ..., Size - 1 }
	return MakeLookupTableHelper<T, Proj>(std::make_integer_sequence<int, Size>());
}

/**
 * \brief Fixed point numerical type with customizable backing integral type and number of bits of precision
 * \tparam T Backing type
 * \tparam F Fractional number bits
 */
template <typename T, int F>
struct Fixed
{
	// The only actual instance data in the whole struct
	T rawValue = 0;

	// Static Constants/Type Aliases
	using fixed = Fixed<T, F>;
	using raw = T;
	using uraw = std::make_unsigned_t<T>;

	static constexpr raw RawMaxValue = std::numeric_limits<T>::max();
	static constexpr raw RawMinValue = std::numeric_limits<T>::min();
	static constexpr raw RawOne = static_cast<T>(1) << F;
	static constexpr raw RawTwo = RawOne + RawOne;
	static constexpr raw RawThree = RawTwo + RawOne;
	static constexpr raw RawHalf = RawOne >> 1;

	static constexpr int AbsoluteMaxBits = 64;
	static constexpr int NumBits = sizeof(T) * 8;

	static_assert(std::is_integral_v<T>, "Backing type must be integral type.");
	// Easier to just assume there will never be any more than 64 bits right now
	static_assert(NumBits <= 64, "Backing type exceeds maximum size.");
	static_assert(F < NumBits - 2, "At least two bits of whole precision are required.");
	static_assert(F % 2 == 0, "Fractional bits must be multiple of 2");

	static constexpr int HalfNumBits = NumBits / 2;
	static constexpr int FractionShift = F;

	static constexpr uraw AllMask = static_cast<uraw>(-1);
	static constexpr uraw FractionMask = RawOne - 1;
	static constexpr uraw WholeMask = ~FractionMask;
	static constexpr int SignShift = NumBits - 1;
	static constexpr uraw SignMask = static_cast<T>(1) << SignShift;
	static constexpr raw EpsilonBits = FractionShift / 8;
	static constexpr int LargePiBits = FractionShift - 3;
	static constexpr raw LargePiMulti = 1 << LargePiBits;

	static constexpr raw RawLog2Max = static_cast<raw>(0x1F) << FractionShift;
	static constexpr raw RawLog2Min = -(static_cast<raw>(0x20) << FractionShift);
	static constexpr size_t TrigLookupTableSize = 1 << 12; // (1 << N) == 2^n : (1 << 12) == 2^12 == 4096

	static const std::array<fixed, TrigLookupTableSize> SinLookupTable;
	static const std::array<fixed, TrigLookupTableSize> TanLookupTable;

	// static Fixed constants
	static const Fixed<T, F> Zero;
	static const Fixed<T, F> One;
	static const Fixed<T, F> Half;
	static const Fixed<T, F> NegOne;
	static const Fixed<T, F> MinValue;
	static const Fixed<T, F> MaxValue;
	static const Fixed<T, F> OneOverMaxValue;
	static const Fixed<T, F> Log2Max;
	static const Fixed<T, F> Log2Min;
	static const Fixed<T, F> Pi;
	static const Fixed<T, F> TwoPi;
	static const Fixed<T, F> PiOver2;
	static const Fixed<T, F> OneOverPi;
	static const Fixed<T, F> OneOverTwoPi;
	static const Fixed<T, F> LargePi;
	static const Fixed<T, F> Ln2;
	static const Fixed<T, F> LutSize;
	static const Fixed<T, F> Deg2Rad;
	static const Fixed<T, F> Rad2Deg;

	// Constructors
	constexpr Fixed() = default;

	// Integral constructors are for constructing from a raw value only, use the static Int/Float methods for creating fixed representations of other numerical types
	explicit constexpr Fixed(raw value) : rawValue(value) {}
	explicit constexpr Fixed(uraw value) : rawValue(static_cast<raw>(value)) {}

	// Static methods for casting from other numerical types, explicitly not implemented as constructors to avoid ambiguity between intended raw value construction.
	static constexpr fixed Int(int value) { return fixed(static_cast<T>(value) * RawOne); }
	static constexpr fixed Float(float value) { return fixed(static_cast<T>(value * RawOne)); }
	static constexpr fixed Float(double value) { return fixed(static_cast<T>(value * RawOne)); }

	// Numerical Type-Casting static_cast<float>(fixed), etc...
	explicit constexpr operator float() const { return static_cast<float>(rawValue) / RawOne; }
	explicit constexpr operator double() const { return static_cast<double>(rawValue) / RawOne; }
	explicit constexpr operator int() const { return static_cast<int>(rawValue >> FractionShift); }
	explicit constexpr operator size_t() const { return static_cast<size_t>(rawValue >> FractionShift); }

	// Basic arithmetic functionality
	static constexpr fixed SafeAdd(fixed x, fixed y);
	static constexpr fixed SafeSub(fixed x, fixed y);
	static constexpr fixed SafeMul(fixed x, fixed y);
	static constexpr fixed SafeDiv(fixed x, fixed y);
	static constexpr fixed SafeMod(fixed x, fixed y);
	static constexpr fixed FastAdd(fixed x, fixed y);
	static constexpr fixed FastSub(fixed x, fixed y);
	static constexpr fixed FastMul(fixed x, fixed y);
	static constexpr fixed FastDiv(fixed x, fixed y);
	static constexpr fixed FastMod(fixed x, fixed y);

	static constexpr bool UseFastMathForOperatorOverloads = true;

	// Compound-assignment operators
	Fixed<T, F>& operator+=(fixed other)
	{
		if constexpr (UseFastMathForOperatorOverloads)
		{
			*this = FastAdd(*this, other);
		}
		else
		{
			*this = SafeAdd(*this, other);
		}
		return *this;
	}

	Fixed<T, F>& operator-=(fixed other)
	{
		if constexpr (UseFastMathForOperatorOverloads)
		{
			*this = FastSub(*this, other);
		}
		else
		{
			*this = SafeSub(*this, other);
		}
		return *this;
	}

	Fixed<T, F>& operator*=(fixed other)
	{
		if constexpr (UseFastMathForOperatorOverloads)
		{
			*this = FastMul(*this, other);
		}
		else
		{
			*this = SafeMul(*this, other);
		}
		return *this;
	}

	Fixed<T, F>& operator/=(fixed other)
	{
		if constexpr (UseFastMathForOperatorOverloads)
		{
			*this = FastDiv(*this, other);
		}
		else
		{
			*this = SafeDiv(*this, other);
		}
		return *this;
	}

	Fixed<T, F>& operator%=(fixed other)
	{
		if constexpr (UseFastMathForOperatorOverloads)
		{
			*this = FastMod(*this, other);
		}
		else
		{
			*this = SafeMod(*this, other);
		}
		return *this;
	}

	Fixed<T, F>& operator&=(fixed other) { this->rawValue &= other.rawValue; return *this; }
	Fixed<T, F>& operator|=(fixed other) { this->rawValue |= other.rawValue; return *this; }
	Fixed<T, F>& operator^=(fixed other) { this->rawValue ^= other.rawValue; return *this; }
	Fixed<T, F>& operator<<=(int shift) { this->rawValue <<= shift; return *this; }
	Fixed<T, F>& operator>>=(int shift) { this->rawValue >>= shift; return *this; }

	static constexpr uraw GetUnsigned(raw r) { return static_cast<uraw>(r); }
	static constexpr raw GetSigned(uraw r) { return static_cast<raw>(r); }
	static constexpr fixed GetLutInterval() { return LutSize / PiOver2; }

private:
	static fixed MakeSinLutEntry(int i, size_t n)
	{
		double angle = (i * std::numbers::pi_v<double> *0.5) / (n - 1);
		double s = std::sin(angle);
		return fixed::Float(s);
	}

	static fixed MakeTanLutEntry(int i, size_t n)
	{
		const double doubleMax = static_cast<double>(MaxValue);
		double angle = (i * std::numbers::pi_v<double> *0.5) / (n - 1);
		double t = std::tan(angle);
		if (t > doubleMax || t < 0.0)
		{
			return MaxValue;
		}
		return fixed::Float(t);
	}

	template <fixed(*Proj)(int, size_t)>
	constexpr static auto MakeInternalLookupTable()
	{
		return MakeLookupTable<fixed, Proj, TrigLookupTableSize>();
	}
};

// Static Constant Fixed Value Definitions
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::Zero(static_cast<T>(0));
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::One(RawOne);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::Half(RawHalf);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::NegOne(-RawOne);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::MinValue(RawMinValue);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::MaxValue(RawMaxValue);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::OneOverMaxValue(One / MaxValue);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::Log2Max(RawLog2Max);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::Log2Min(RawLog2Min);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::Pi = fixed::Float(std::numbers::pi_v<double>);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::TwoPi = fixed::Float(std::numbers::pi_v<double> *2);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::PiOver2 = fixed::Float(std::numbers::pi_v<double> / 2);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::OneOverPi = fixed::Float(1.0 / std::numbers::pi_v<double>);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::OneOverTwoPi = fixed::Float(1.0 / (2 * std::numbers::pi_v<double>));
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::LargePi = fixed::Float(std::numbers::pi_v<double> *LargePiMulti);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::Ln2 = fixed::Float(std::numbers::ln2_v<double>);
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::LutSize = fixed::Int(static_cast<int>(TrigLookupTableSize - 1));
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::Deg2Rad = fixed::Float(1.0 / 180.0) * TwoPi;
template <typename T, int F> const Fixed<T, F> Fixed<T, F>::Rad2Deg = OneOverTwoPi * fixed::Int(360);

// Lookup Tables
template <typename T, int F>
const std::array<Fixed<T, F>, Fixed<T, F>::TrigLookupTableSize> Fixed<T, F>::SinLookupTable = MakeInternalLookupTable<&MakeSinLutEntry>();

template <typename T, int F>
const std::array<Fixed<T, F>, Fixed<T, F>::TrigLookupTableSize> Fixed<T, F>::TanLookupTable = MakeInternalLookupTable<&MakeTanLutEntry>();

// Mathematical functions
template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::SafeAdd(fixed x, fixed y)
{
	raw xr = x.rawValue;
	raw yr = y.rawValue;
	raw sum = xr + yr;

	if (((~(xr ^ yr) & (xr ^ sum)) & RawMinValue) != 0)
	{
		return (xr > 0) ? MaxValue : MinValue;
	}
	return fixed(sum);
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::SafeSub(fixed x, fixed y)
{
	raw xr = x.rawValue;
	raw yr = y.rawValue;
	raw diff = xr - yr;

	if ((((xr ^ yr) & (xr ^ diff)) & RawMinValue) != 0)
	{
		diff = xr < 0 ? RawMinValue : RawMaxValue;
	}

	return fixed(diff);
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::SafeMul(fixed x, fixed y)
{
	raw xr = x.rawValue;
	raw yr = y.rawValue;

	uraw xlo = GetUnsigned(xr & FractionMask);
	raw xhi = xr >> FractionShift;
	uraw ylo = GetUnsigned(yr & FractionMask);
	raw yhi = yr >> FractionShift;

	uraw lolo = xlo * ylo;
	raw lohi = GetSigned(xlo) * yhi;
	raw hilo = xhi * GetSigned(ylo);
	raw hihi = xhi * yhi;

	uraw loRet = lolo >> FractionShift;
	raw mid1 = lohi;
	raw mid2 = hilo;
	raw hiRet = hihi << FractionShift;

	bool overflow = false;
	auto add_overflower_helper = [&overflow](raw x, raw y)
	{
		raw sum = x + y;
		overflow |= ((x ^ y ^ sum) & RawMinValue) != 0;
		return sum;
	};

	raw sum = add_overflower_helper(fixed::GetSigned(loRet), mid1);
	sum = add_overflower_helper(sum, mid2);
	sum = add_overflower_helper(sum, hiRet);

	bool opSignsEqual = ((xr ^ yr) & RawMinValue) == 0;

	if (opSignsEqual)
	{
		if (sum < 0 || (overflow && xr > 0))
		{
			return MaxValue;
		}
	}
	else
	{
		if (sum > 0)
		{
			return MinValue;
		}
	}

	auto topCarry = hihi >> FractionShift;
	if (topCarry != 0 && topCarry != -1)
	{
		return opSignsEqual ? MaxValue : MinValue;
	}

	if (!opSignsEqual)
	{
		T posOp = xr, negOp = yr;
		if (xr <= yr)
		{
			std::swap(posOp, negOp);
		}
		if (sum > negOp && negOp < -RawOne && posOp > RawOne)
		{
			return MinValue;
		}
	}

	return fixed(sum);
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::SafeDiv(fixed x, fixed y)
{
	raw xr = x.rawValue;
	raw yr = y.rawValue;

	FXMATH_ASSERT(yr != 0 && "Divide by zero");

	uraw remainder = GetUnsigned(xr >= 0 ? xr : -xr);
	uraw divider = GetUnsigned(yr >= 0 ? yr : -yr);
	uraw quotient = static_cast<uraw>(0);
	int bitPosition = NumBits / 2 + 1;

	while ((divider & 0xF) == 0 && bitPosition >= 4)
	{
		divider >>= 4;
		bitPosition -= 4;
	}

	while (remainder != 0 && bitPosition >= 0)
	{
		int shift = std::countl_zero(remainder);
		if (shift > bitPosition)
		{
			shift = bitPosition;
		}
		remainder <<= shift;
		bitPosition -= shift;

		raw div = remainder / divider;
		remainder %= divider;
		quotient += div << bitPosition;

		// Overflow detection
		if ((div & ~(AllMask >> bitPosition)) != 0)
		{
			return ((xr ^ yr) & RawMinValue) == 0 ? MaxValue : MinValue;
		}

		remainder <<= 1;
		--bitPosition;
	}

	// rounding
	++quotient;
	raw result = GetSigned(quotient >> 1);
	if (((xr ^ yr) & RawMinValue) != 0)
	{
		result = -result;
	}

	return fixed(result);
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::SafeMod(fixed x, fixed y)
{
	if (x.rawValue == RawMinValue && y.rawValue == -1)
	{
		return Zero;
	}
	else
	{
		raw rawResult = x.rawValue % y.rawValue;
		return fixed(rawResult);
	}
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::FastAdd(fixed x, fixed y)
{
	return fixed(x.rawValue + y.rawValue);
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::FastSub(fixed x, fixed y)
{
	return fixed(x.rawValue - y.rawValue);
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::FastMul(fixed x, fixed y)
{
	raw xr = x.rawValue;
	raw yr = y.rawValue;

	uraw xlo = GetUnsigned(xr & FractionMask);
	raw xhi = xr >> FractionShift;
	uraw ylo = GetUnsigned(yr & FractionMask);
	raw yhi = yr >> FractionShift;

	uraw lolo = xlo * ylo;
	raw lohi = GetSigned(xlo) * yhi;
	raw hilo = xhi * GetSigned(ylo);
	raw hihi = xhi * yhi;

	raw loRet = GetSigned(lolo >> FractionShift);
	raw mid1 = lohi;
	raw mid2 = hilo;
	raw hiRet = hihi << FractionShift;

	raw sum = loRet + mid1 + mid2 + hiRet;
	return fixed(sum);
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::FastDiv(fixed x, fixed y)
{
	// TODO: Identify potential FastDiv implementation
	return SafeDiv(x, y);
}

template <typename T, int F>
constexpr Fixed<T, F> Fixed<T, F>::FastMod(fixed x, fixed y)
{
	return fixed(x.rawValue % y.rawValue);
}

// Inline Operator Overloads

constexpr bool kUseFastMathInOperators = true;

// Arithmatic operators
template <typename T, int F> constexpr Fixed<T, F> operator+(Fixed<T, F> x, Fixed<T, F> y) { return x += y; }
template <typename T, int F> constexpr Fixed<T, F> operator-(Fixed<T, F> x, Fixed<T, F> y) { return x -= y; }
template <typename T, int F> constexpr Fixed<T, F> operator*(Fixed<T, F> x, Fixed<T, F> y) { return x *= y; }
template <typename T, int F> constexpr Fixed<T, F> operator/(Fixed<T, F> x, Fixed<T, F> y) { return x /= y; }
template <typename T, int F> constexpr Fixed<T, F> operator%(Fixed<T, F> x, Fixed<T, F> y) { return x %= y; }

// Unary Negation
template <typename T, int F> constexpr Fixed<T, F> operator-(Fixed<T, F> x) { return (x == Fixed<T, F>::MinValue) ? Fixed<T, F>::MaxValue : Fixed<T, F>(-x.rawValue); }

// Bitwise operators
template <typename T, int F> constexpr Fixed<T, F> operator&(Fixed<T, F> x, Fixed<T, F> y) { return x &= y; }
template <typename T, int F> constexpr Fixed<T, F> operator|(Fixed<T, F> x, Fixed<T, F> y) { return x |= y; }
template <typename T, int F> constexpr Fixed<T, F> operator^(Fixed<T, F> x, Fixed<T, F> y) { return x ^= y; }
template <typename T, int F> constexpr Fixed<T, F> operator<<(const Fixed<T, F>& x, int shift) { return x <<= shift; }
template <typename T, int F> constexpr Fixed<T, F> operator>>(const Fixed<T, F>& x, int shift) { return x >>= shift; }
template <typename T, int F> constexpr Fixed<T, F> operator~(const Fixed<T, F>& x) { return Fixed<T, F>(~x.rawValue); }

// Comparison operators
template <typename T, int F> constexpr bool operator==(Fixed<T, F> x, Fixed<T, F> y) { return x.rawValue == y.rawValue; }
template <typename T, int F> constexpr bool operator!=(Fixed<T, F> x, Fixed<T, F> y) { return x.rawValue != y.rawValue; }
template <typename T, int F> constexpr bool operator<(Fixed<T, F> x, Fixed<T, F> y) { return x.rawValue < y.rawValue; }
template <typename T, int F> constexpr bool operator>(Fixed<T, F> x, Fixed<T, F> y) { return x.rawValue > y.rawValue; }
template <typename T, int F> constexpr bool operator<=(Fixed<T, F> x, Fixed<T, F> y) { return x.rawValue <= y.rawValue; }
template <typename T, int F> constexpr bool operator>=(Fixed<T, F> x, Fixed<T, F> y) { return x.rawValue >= y.rawValue; }


// Simple type aliases
using fixed32 = Fixed<int32_t, 16>;
using fixed64 = Fixed<int64_t, 32>;

using fixed = fixed64;

static_assert(sizeof(fixed32) == 4);
static_assert(sizeof(fixed64) == 8);
static_assert(sizeof(fixed) == sizeof(fixed::raw));

// User defined literals
inline fixed32 operator"" _fx32(long double v) { return fixed32::Float(static_cast<double>(v)); }
inline fixed32 operator"" _fx32(unsigned long long int v) { return fixed32::Int(static_cast<int>(v)); }
inline fixed64 operator"" _fx64(long double v) { return fixed64::Float(static_cast<double>(v)); }
inline fixed64 operator"" _fx64(unsigned long long int v) { return fixed64::Int(static_cast<int>(v)); }
inline fixed operator"" _fx(long double v) { return fixed::Float(static_cast<double>(v)); }
inline fixed operator"" _fx(unsigned long long int v) { return fixed::Int(static_cast<int>(v)); }
