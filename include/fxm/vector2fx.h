#pragma once

#include "fixedtype.h"
#include "fixedmath.h"

struct Vector2fx
{
	using fixed = fixed64;

	fixed x = fixed::Zero;
	fixed y = fixed::Zero;

	static_assert(std::is_trivially_copyable_v<fixed>);

	static const Vector2fx Zero;
	static const Vector2fx One;
	static const Vector2fx Right;
	static const Vector2fx Left;
	static const Vector2fx Up;
	static const Vector2fx Down;

	// constructors
	constexpr Vector2fx() : x(fixed::Zero), y(fixed::Zero) {}
	constexpr Vector2fx(fixed x, fixed y) : x(x), y(y) {}
	constexpr Vector2fx(const Vector2fx& other) = default;
	constexpr Vector2fx(Vector2fx&& other) noexcept = default;
	~Vector2fx() = default;

	Vector2fx& operator=(const Vector2fx& other) = default;
	Vector2fx& operator=(Vector2fx&& other) noexcept = default;

	// compound-assignment operators
	Vector2fx& operator+=(const Vector2fx& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Vector2fx& operator-=(const Vector2fx& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Vector2fx& operator*=(const Vector2fx& other)
	{
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}

	Vector2fx& operator*=(fixed other)
	{
		this->x *= other;
		this->y *= other;
		return *this;
	}

	Vector2fx& operator/=(fixed other)
	{
		this->x /= other;
		this->y /= other;
		return *this;
	}

	fixed& operator[](int index)
	{
		return (index & 1) ? y : x;
	}

	fixed operator[](int index) const
	{
		return (index & 1) ? y : x;
	}

	// static methods
	static fixed Dot(const Vector2fx& a, const Vector2fx& b);
	static fixed Cross(const Vector2fx& a, const Vector2fx& b);
	static Vector2fx Project(const Vector2fx& a, const Vector2fx& bNormalized);
	static Vector2fx ReverseProject(const Vector2fx& a, const Vector2fx& b);
	static Vector2fx Normalize(const Vector2fx& vec);
	static fixed Distance(const Vector2fx& a, const Vector2fx& b);
	static fixed DistanceSquared(const Vector2fx& a, const Vector2fx& b);
	static fixed Slope(const Vector2fx& a, const Vector2fx& b);
	static Vector2fx Rotate90Clockwise(const Vector2fx& vec);
	static Vector2fx Rotate90CounterClockwise(const Vector2fx& vec);
	static fixed Angle(const Vector2fx& from, const Vector2fx& to);
	static fixed SignedAngle(const Vector2fx& from, const Vector2fx& to);
	static Vector2fx Rotate(const Vector2fx& vec, fixed degrees);
	static Vector2fx RotateAroundAxis(const Vector2fx& vec, fixed degrees, const Vector2fx& axis);
	static Vector2fx RotateByRadians(const Vector2fx& vec, fixed radians);
	static Vector2fx RotateByRadiansAroundAxis(const Vector2fx& vec, fixed radians, const Vector2fx& axis);
	static Vector2fx Reflect(const Vector2fx& vec, const Vector2fx& normal);
	static bool ApproxEqual(const Vector2fx& a, const Vector2fx& b, int ignoreBits = fixed::EpsilonBits);

	// instance methods
	fixed Magnitude() const
	{
		// Note: real big vectors might be a problem here, consider using slower Sqrt function or offering slower pathway for code that
		// needs to be safer with vectors that could square values to above 1000000000
		return Mathfx::FastSqrt(x * x + y * y);
	}

	fixed SqrMagnitude() const
	{
		return x * x + y * y;
	}

	Vector2fx Normalized() const
	{
		return Normalize(*this);
	}
};

constexpr Vector2fx Vector2fx::Zero(fixed::Zero, fixed::Zero);
constexpr Vector2fx Vector2fx::One(fixed::One, fixed::One);
constexpr Vector2fx Vector2fx::Right(fixed::One, fixed::Zero);
constexpr Vector2fx Vector2fx::Left(fixed::NegOne, fixed::Zero);
constexpr Vector2fx Vector2fx::Up(fixed::Zero, fixed::One);
constexpr Vector2fx Vector2fx::Down(fixed::Zero, fixed::NegOne);

inline Vector2fx operator+(Vector2fx a, const Vector2fx& b) { return a += b; }
inline Vector2fx operator-(Vector2fx a, const Vector2fx& b) { return a -= b; }
inline Vector2fx operator*(Vector2fx a, const Vector2fx& b) { return a *= b; }
inline Vector2fx operator*(Vector2fx a, fixed b) { return a *= b; }
inline Vector2fx operator*(fixed a, Vector2fx b) { return b *= a; }
inline Vector2fx operator/(Vector2fx a, fixed b) { return a /= b; }
inline bool operator==(const Vector2fx& a, const Vector2fx& b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(const Vector2fx& a, const Vector2fx& b) { return a.x != b.x || a.y != b.y; }

fixed Vector2fx::Dot(const Vector2fx& a, const Vector2fx& b)
{
	return a.x * b.x + a.y * b.y;
}

fixed Vector2fx::Cross(const Vector2fx& a, const Vector2fx& b)
{
	return a.x * b.y - a.y * b.x;
}

Vector2fx Vector2fx::Project(const Vector2fx& a, const Vector2fx& bNormalized)
{
	return Dot(a, bNormalized) * bNormalized;
}

Vector2fx Vector2fx::ReverseProject(const Vector2fx& a, const Vector2fx& b)
{
	fixed prod = Dot(a, b);
	if (prod == fixed::Zero)
	{
		return Zero;
	}
	return b * (a.SqrMagnitude() / prod);
}

Vector2fx Vector2fx::Normalize(const Vector2fx& vec)
{
	fixed length = vec.Magnitude();

	if (Mathfx::ApproxZero(length))
	{
		return Zero;
	}
	return vec / length;
}

fixed Vector2fx::Distance(const Vector2fx& a, const Vector2fx& b)
{
	return (a - b).Magnitude();
}

fixed Vector2fx::DistanceSquared(const Vector2fx& a, const Vector2fx& b)
{
	return (a - b).SqrMagnitude();
}

fixed Vector2fx::Slope(const Vector2fx& a, const Vector2fx& b)
{
	fixed dy = a.y - b.y;
	fixed dx = b.x - a.x;
	if (Mathfx::ApproxZero(dx))
	{
		return fixed::MaxValue;
	}
	return dy / dx;
}

Vector2fx Vector2fx::Rotate90Clockwise(const Vector2fx& vec)
{
	return Vector2fx { vec.x, -vec.y };
}

Vector2fx Vector2fx::Rotate90CounterClockwise(const Vector2fx& vec)
{
	return Vector2fx { -vec.x, vec.y };
}

fixed Vector2fx::Angle(const Vector2fx& from, const Vector2fx& to)
{
	// sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
	fixed denominator = Mathfx::Sqrt(from.SqrMagnitude() * to.SqrMagnitude());
	if (Mathfx::ApproxZero(denominator))
	{
		return fixed::Zero;
	}

	fixed dot = Mathfx::Clamp(Dot(from, to) / denominator, fixed::Int(-1), fixed::Int(1));
	return Mathfx::Acos(dot) * fixed::Rad2Deg;
}

fixed Vector2fx::SignedAngle(const Vector2fx& from, const Vector2fx& to)
{
	fixed unsignedAngle = Angle(from, to);
	fixed sign = Mathfx::Sign(from.x * to.y - from.y * to.x);
	return unsignedAngle * sign;
}

Vector2fx Vector2fx::Rotate(const Vector2fx& vec, fixed degrees)
{
	return RotateByRadians(vec, degrees * fixed::Deg2Rad);
}

Vector2fx Vector2fx::RotateAroundAxis(const Vector2fx& vec, fixed degrees, const Vector2fx& axis)
{
	return RotateByRadiansAroundAxis(vec, degrees * fixed::Deg2Rad, axis);
}

Vector2fx Vector2fx::RotateByRadians(const Vector2fx& vec, fixed radians)
{
	if (radians == fixed::Zero)
	{
		return vec;
	}

	fixed sin = Mathfx::Sin(radians);
	fixed cos = Mathfx::Cos(radians);

	Vector2fx result(vec.x * cos - vec.y * sin, vec.x * sin + vec.y * cos);
	return result;
}

Vector2fx Vector2fx::RotateByRadiansAroundAxis(const Vector2fx& vec, fixed radians, const Vector2fx& axis)
{
	return RotateByRadians(vec - axis, radians) + axis;
}

Vector2fx Vector2fx::Reflect(const Vector2fx& vec, const Vector2fx& normal)
{
	fixed multiplier = 2_fx * Dot(vec, normal);
	Vector2fx result(vec.x - (multiplier * normal.x), vec.y - (multiplier * normal.y));
	return result;
}

bool Vector2fx::ApproxEqual(const Vector2fx& a, const Vector2fx& b, int ignoreBits)
{
	return Mathfx::ApproxEqual(a.x, b.x, ignoreBits)
		&& Mathfx::ApproxEqual(a.y, b.y, ignoreBits);
}
