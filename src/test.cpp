#include <fxm/fxm.h>

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <random>
#include <chrono>
#include <limits>

struct
{
	std::mt19937_64 rng{};
} G;

fixed64 random_fixed(fixed64 max = fixed64::Int(1000))
{
	std::uniform_int_distribution dist(-max.rawValue, max.rawValue);
	return fixed64(dist(G.rng));
}

fixed64 random_pos_fixed(fixed64 max = fixed64::Int(1000))
{
	std::uniform_int_distribution dist(fixed64::One.rawValue, max.rawValue);
	return fixed64(dist(G.rng));
}

fixed64 random_norm_fixed()
{
	std::uniform_int_distribution dist(0ll, fixed64::One.rawValue);
	return fixed64(dist(G.rng));
}

fixed64 random_snorm_fixed()
{
	std::uniform_int_distribution dist(-fixed64::One.rawValue + 1, fixed64::One.rawValue);
	return fixed64(dist(G.rng));
}

float random_float(float max = 1000.0f)
{
	std::uniform_real_distribution dist(-max, max);
	return dist(G.rng);
}

float random_pos_float(float max = 1000.0f)
{
	std::uniform_real_distribution dist(std::numeric_limits<float>::epsilon(), max);
	return std::fabs(dist(G.rng));
}

float random_norm_float()
{
	std::uniform_real_distribution dist(0.0f, 1.0f);
	return dist(G.rng);
}

float random_snorm_float()
{
	std::uniform_real_distribution dist(-1.0f, 1.0f);
	return dist(G.rng);
}

#pragma warning(push)
#pragma warning(disable : 4146)
int64_t testCases[] = {
	// Very small fractions
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	-1, -2, -3, -4, -5, -6, -7, -8, -9, -10,

	// Whole integer numbers
	0x100000000, -0x100000000, 0x200000000, -0x200000000, 0x300000000, -0x300000000,
	0x400000000, -0x400000000, 0x500000000, -0x500000000, 0x600000000, -0x600000000,

	// Power of 2 fractions 1/2, 1/4, etc...
	0x80000000, -0x80000000, 0x40000000, -0x40000000, 0x20000000, -0x20000000,

	// Problematic carry
	0xFFFFFFFF, -0xFFFFFFFF, 0x1FFFFFFFF, -0x1FFFFFFFF, 0x3FFFFFFFF, -0x3FFFFFFFF,

	// Smallest and largest values
	std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min(),

	// Large random numbers
	 1457466697518801965, -6172936938307286322,  2349539390256613016,  2419513431596392674,
	 3976881727867200483, -1001748061126808233, -8060579896565875674,  3592268519976138234,
	-3865498568268036244, -1060468494051786140, -2522879792306034087,  7119918320609595945,
	-6092650318713774851,  6123177281325195208,  5770379232468740089,  1651530457224778615,
	 2683741531827883531,  5644423677118373590, -1787372587816818563, -3600924876557093681,
	 4495584553561360277,  4678236981017005539, -3077339067110002481,  8222739718542766952,
	-2907247268322498712,  8926190314813935612, -5104046291242085355,  5849626007272441578,
	-6507510225145691663, -3874984356333327166, -8253344911820207014, -5711617745786984050,
	-8221433455696316564, -1926286001397543443, -8511311688902925918, -7099515507006119066,
	-2725705860307620003,  6918155940072606121,  4326352916117827255,  2276184905549482753,
	 5743432580798190009, -7523393194003503823,  8727387978082023717,  8092917355764490961,
	-2580828170179890796,  2280831645894754823, -9010030057431833871, -2730192874550452137,
	-8012103923674207311,  3385561312960630726, -5610435353652685727, -6953792525569180133,
	 5545752943480091208,  1228564845127946047,  5260219702022504991, -2810910306307490861,
	-9045036432117350997,  3151838233605927780, -5221925152266852364,  5873917871718789580,
	-6034336645708932990,  7892537077331518698, -8547577030010717417,  6551217960709952345,
	-7749407440866431068,  6405560974122285892,  2390218503115029322,  4033462375253100686,
	-2354961758125968138,  5992388088083817281,  8166252613255662510,  1762064443801529907,
	 1974725860838100575,  4831455857331398884,  6618542144118242027, -3379717537564558762,
	 6385627628734540399,  9189472803314843945,  1208079644192151285, -4092284768346922419,
	-8435525971328366386, -1249820897464544966,  5047737633346470937, -9221647673598191856,
	 3595697263988685941, -8942111522671291535, -4524380036488401571,  3167767525679809934,
	-1833633529083714721, -8389416333351386570, -8760065268242315935,  2710484353173164553,
	 8511648282914657261,  1741675856117937697, -6371613665633556251, -1155668073229008388,

	// Small random numbers
	-9332762525,  5752940692, -8553289738, -8358515806,  2266925555,  5200200522, -8285944860,
	-3781497127,  5870195037, -1878618891, -5362652151, -6645433808, -8140990289, -1834739798,
	 2586325724, -3153809529,  5938789156, -9325482612, -3211865416,  9037491532, -4356195582,
	-1782463699, -6980377840, -7221718120,  5557181742, -7837354086,  7518154665,  2181340759,
	 9468009034,  3705127921, -6688099329, -3532798514,  3761433493, -9192172133, -4024709946,
	 6690672392,  1950460299,  5704311136, -1485644382,  6573619685, -4616160201,  1953824831,
	 2063116981, -2861095922, -2935923129, -1375929935, -1279125104, -7411993700, -1072173014,
	-4673443477,  5585427695,  8833319463, -9954304147,  5560306159,  4518929485,  3379634867,
	 4692052556, -4945265063,  1894421390,  2757139127, -1560333904,  9364054547,  6855929542,
	 1473520966, -7547563415,  8429134394,  1857085531, -5908535988, -6062731203, -6109028337,
	-6643643742, -9208999843, -7701227598, -2150723443,  6378394321,  4462909008, -5980818474,
	-3555941540,  6158150937,  7824866115, -4247031886, -2705123087,  9011062269,  7067644681,
	-3760064650, -9968158700,  6862016947, -7365185641,  9563693139, -2930269324, -6175356976,
	 6613090120, -7396830453,  5587249971,  9478070348, -8653401763,

	// Tiny random numbers
	 867, -639, -918, -247, -684,  754, -551,  130, -919,  376,  383,  922, -935, -894,  851, -595,  638,
	 923, -120,  612,  271, -784, -938,  837,  577, -376,  216,  175, -942, -471,  694,  971,  444, -260,
	 132,  141,  732,  923, -357, -252, -600,  649,  926,  897, -785,  535, -512, -911,  600, -168,  211,
	 797, -514,  826,  824, -817, -574,  953,  210,  475, -838,  296,  766,  991, -622, -531,  624, -152,
	-503, -711,  480, -759,  593,  255,  452,  105, -110, -686,  765,  137,  911,  766,  604, -908,  230,
	-331, -618, -305, -856, -857,  521,  169, -473,  792, -860,  870, -540, -737,  110,  351, -829,  802,
	 359, -749, -190, -835,  577, -364,  174,  133, -664,  362,  489, -279, -334, -378, -449,  944, -140,
	 426, -735, -361,  105,  507, -672, -665,  963,  131,  872,  381,  136, -346,  470, -484, -455, -897,
	 780,  140,  616,  478, -301, -274, -101,  398,
};
#pragma warning(pop)

#include <random>
#include <chrono>

TEST_CASE("Casting", "[fixedmath]")
{
	fixed64 f5literal = 5_fx64;
	fixed64 f5int = fixed64::Int(5);
	fixed64 f5float = fixed64::Float(5.0f);

	REQUIRE(f5literal == f5int);
	REQUIRE(f5int == f5float);

	fixed64 fracliteral = 0.25_fx64;
	fixed64 fracfloat = fixed64::Float(0.25f);

	REQUIRE(fracliteral == fracfloat);
}

TEST_CASE("Arithmetic", "[fixedmath]")
{
	SECTION("Comparison")
	{
		REQUIRE(0_fx64 == 0_fx64);
		REQUIRE(1_fx64 == 1_fx64);
		REQUIRE(-1_fx64 != 1_fx64);
		REQUIRE(-2_fx64 == -2_fx64);
		REQUIRE(5_fx64 != 5.0001_fx64);
		REQUIRE(Mathfx::ApproxEqual(fixed64(1llu), fixed64(3llu)));
		REQUIRE(Mathfx::ApproxZero(fixed64(1llu), 4));
		REQUIRE(Mathfx::ApproxZero(fixed64(3llu), 4));
		REQUIRE(Mathfx::ApproxZero(fixed64(7llu), 4));
		REQUIRE(Mathfx::ApproxZero(fixed64(0xFllu), 4));
		REQUIRE(!Mathfx::ApproxZero(fixed64(0x1Fllu), 4));

		REQUIRE(Mathfx::ApproxZero(fixed64(1llu), 2));
		REQUIRE(Mathfx::ApproxZero(fixed64(3llu), 2));
		REQUIRE(!Mathfx::ApproxZero(fixed64(7llu), 2));
		REQUIRE(!Mathfx::ApproxZero(fixed64(0xFllu), 2));
		REQUIRE(!Mathfx::ApproxZero(fixed64(0x1Fllu), 2));

		REQUIRE(!Mathfx::ApproxZero(fixed64(1llu), 0));
		REQUIRE(Mathfx::ApproxZero(fixed64(1llu), 1));

		REQUIRE(fixed64(2ll) != 2_fx64);

		REQUIRE(-2_fx64 < 2_fx64);
		REQUIRE(-1_fx64 < 0_fx64);
		REQUIRE(-2_fx64 < -1_fx64);
		REQUIRE(-2_fx64 <= -2_fx64);
		REQUIRE(!(-2_fx64 < -2_fx64));
		REQUIRE(4_fx64 <= 4_fx64);
		REQUIRE(0_fx64 <= 0_fx64);
		REQUIRE(4_fx64 > 2_fx64);
		REQUIRE(4_fx64 >= 2_fx64);
		REQUIRE(4_fx64 >= 4_fx64);
		REQUIRE(!(-3_fx64 > -3_fx64));
	}


	SECTION("Addition")
	{
		fixed64 a = fixed64::One;
		a <<= 1;
		REQUIRE(a == 2_fx64);


		REQUIRE(0_fx64 + 1_fx64 == 1_fx64);
		REQUIRE(25.5_fx64 + 3.4_fx64 == 28.9_fx64);

		for (auto rawX : testCases)
		{
			for (auto rawY : testCases)
			{
				double dx = static_cast<double>(fixed64(rawX));
				double dy = static_cast<double>(fixed64(rawY));
				double expected = dx + dy;
				expected = std::clamp(expected, static_cast<double>(fixed64::MinValue), static_cast<double>(fixed64::MaxValue));
				fixed64 x(rawX), y(rawY);
				fixed64 actualF = fixed64::SafeAdd(x, y);
				double actual = static_cast<double>(actualF);
				CHECK(actual == Approx(expected).margin(0.00000001));

				if (std::abs(dx) < 30000 && std::abs(dy) < 30000)
				{
					fixed64 fastF = fixed64::FastAdd(x, y);
					double fastActual = static_cast<double>(fastF);
					CHECK(fastActual == Approx(expected).margin(0.0000001));
				}
			}
		}
	}

	SECTION("Subtraction")
	{
		REQUIRE(4.3_fx64 - 2_fx64 == 2.3_fx64);
		REQUIRE(1.5_fx64 - 3_fx64 == -1.5_fx64);

		for (auto rawX : testCases)
		{
			for (auto rawY : testCases)
			{
				double dx = static_cast<double>(fixed64(rawX));
				double dy = static_cast<double>(fixed64(rawY));
				double expected = dx - dy;
				expected = std::clamp(expected, static_cast<double>(fixed64::MinValue), static_cast<double>(fixed64::MaxValue));
				fixed64 x(rawX), y(rawY);
				fixed64 actualF = fixed64::SafeSub(x, y);
				double actual = static_cast<double>(actualF);
				CHECK(actual == Approx(expected).margin(0.00000001));

				if (std::abs(dx) < 30000 && std::abs(dy) < 30000)
				{
					fixed64 fastF = fixed64::FastSub(x, y);
					double fastActual = static_cast<double>(fastF);
					CHECK(fastActual == Approx(expected).margin(0.0000001));
				}
			}
		}
	}

	SECTION("Unary Negation")
	{
		fixed64 f = -1_fx64;
		REQUIRE(-f == 1_fx64);
		REQUIRE(-0_fx64 == 0_fx64);
		REQUIRE(-fixed64::MinValue == fixed64::MaxValue);

		for (auto rawX : testCases)
		{
			double dx = static_cast<double>(fixed64(rawX));
			double expected = -dx;
			expected = std::clamp(expected, static_cast<double>(fixed64::MinValue), static_cast<double>(fixed64::MaxValue));
			fixed64 x(rawX);
			fixed64 actualF = -x;
			double actual = static_cast<double>(actualF);
			CHECK(actual == Approx(expected).margin(0.00000001));
		}
	}

	SECTION("Multiplication")
	{
		REQUIRE(2_fx64 * 3_fx64 == 6_fx64);
		REQUIRE(Mathfx::ApproxEqual(2.1_fx64 * -3_fx64, -6.3_fx64));
		REQUIRE(4_fx64 * 0.5_fx64 == 2_fx64);

		for (auto rawX : testCases)
		{
			for (auto rawY : testCases)
			{
				double dx = static_cast<double>(fixed64(rawX));
				double dy = static_cast<double>(fixed64(rawY));
				double expected = dx * dy;
				expected = std::clamp(expected, static_cast<double>(fixed64::MinValue), static_cast<double>(fixed64::MaxValue));
				fixed64 x(rawX), y(rawY);
				fixed64 actualF = fixed64::SafeMul(x, y);
				double actual = static_cast<double>(actualF);
				CHECK(actual == Approx(expected).margin(0.00000001));

				if (std::abs(dx) < 30000 && std::abs(dy) < 30000)
				{
					fixed64 fastF = fixed64::FastMul(x, y);
					double fastActual = static_cast<double>(fastF);
					CHECK(fastActual == Approx(expected).margin(0.0000001));
				}
			}
		}
	}

	SECTION("Modulo")
	{
		REQUIRE(3_fx64 % 4_fx64 == 3_fx64);
		REQUIRE(7_fx64 % 4_fx64 == 3_fx64);

		for (auto rawX : testCases)
		{
			for (auto rawY : testCases)
			{
				if (rawY == 0 || (fixed64(rawX) == fixed64::MinValue && rawY == -1))
					continue;
				fixed64 x(rawX), y(rawY);
				fixed64 actualF = fixed64::SafeMod(x, y);
				CHECK(actualF.rawValue == rawX % rawY);

				if (!(rawX == fixed64::RawMinValue && rawY == -1))
				{
					fixed64 fastF = fixed64::FastMod(x, y);
					CHECK(fastF.rawValue == rawX % rawY);
				}
			}
		}
	}

	SECTION("Division")
	{
		REQUIRE(10_fx64 / 2_fx64 == 5_fx64);
		REQUIRE(Mathfx::ApproxEqual(25.5_fx64 / 5_fx64, 5.1_fx64));

		for (auto rawX : testCases)
		{
			for (auto rawY : testCases)
			{
				if (rawY == 0) continue;

				double dx = static_cast<double>(fixed64(rawX));
				double dy = static_cast<double>(fixed64(rawY));
				double expected = dx / dy;
				expected = std::clamp(expected, static_cast<double>(fixed64::MinValue), static_cast<double>(fixed64::MaxValue));
				fixed64 x(rawX), y(rawY);
				fixed64 actualF = fixed64::SafeDiv(x, y);
				double actual = static_cast<double>(actualF);
				CHECK(actual == Approx(expected).margin(0.000001));

				if (std::abs(dx) < 30000 && std::abs(dy) < 30000)
				{
					fixed64 fastF = fixed64::FastDiv(x, y);
					double fastActual = static_cast<double>(fastF);
					CHECK(fastActual == Approx(expected).margin(0.0000001));
				}
			}
		}
	}
}

TEST_CASE("Math Functions", "[fixedmath]")
{
	SECTION("Sign")
	{
		REQUIRE(Mathfx::Sign(34.2_fx64) == 1_fx64);
		REQUIRE(Mathfx::SignToInt(34.2_fx64) == 1);

		REQUIRE(Mathfx::Sign(0_fx64) == 0_fx64);
		REQUIRE(Mathfx::SignToInt(0_fx64) == 0);

		REQUIRE(Mathfx::Sign(-17_fx64) == -1_fx64);
		REQUIRE(Mathfx::SignToInt(-17_fx64) == -1);

		for (auto raw : testCases)
		{
			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);

			double expected = (d == 0) ? 0 : (d < 0) ? -1 : 1;
			double actual = static_cast<double>(Mathfx::Sign(e));
			CHECK(actual == Approx(expected));
		}
	}

	SECTION("Abs")
	{
		REQUIRE(Mathfx::Abs(5_fx64) == 5_fx64);
		REQUIRE(Mathfx::Abs(-15.324_fx64) == 15.324_fx64);
		REQUIRE(Mathfx::Abs(fixed64::MinValue) == fixed64::MaxValue);

		for (auto raw : testCases)
		{
			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);

			double expected = std::fabs(d);
			double actual = static_cast<double>(Mathfx::Abs(e));
			CHECK(actual == Approx(expected));
		}
	}

	SECTION("Floor")
	{
		REQUIRE(Mathfx::Floor(3.2_fx64) == 3_fx64);
		REQUIRE(Mathfx::FloorToInt(3.2_fx64) == 3);

		REQUIRE(Mathfx::Floor(-73.2_fx64) == -74_fx64);
		REQUIRE(Mathfx::FloorToInt(-73.2_fx64) == -74);

		REQUIRE(Mathfx::Floor(1_fx64) == 1_fx64);
		REQUIRE(Mathfx::FloorToInt(1_fx64) == 1);

		for (auto raw : testCases)
		{
			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);

			double expected = std::floor(d);
			double actual = static_cast<double>(Mathfx::Floor(e));
			CHECK(actual == Approx(expected));
		}
	}

	SECTION("Ceiling")
	{
		REQUIRE(Mathfx::Ceiling(3.2_fx64) == 4_fx64);
		REQUIRE(Mathfx::CeilingToInt(3.2_fx64) == 4);

		REQUIRE(Mathfx::Ceiling(-73.2_fx64) == -73_fx64);
		REQUIRE(Mathfx::CeilingToInt(-73.2_fx64) == -73);

		REQUIRE(Mathfx::Ceiling(1_fx64) == 1_fx64);
		REQUIRE(Mathfx::CeilingToInt(1_fx64) == 1);

		for (auto raw : testCases)
		{
			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);

			double expected = std::ceil(d);
			fixed64 f = Mathfx::Ceiling(e);
			double actual = static_cast<double>(f);

			CAPTURE(expected, actual, e.rawValue, f.rawValue);

			CHECK(actual == Approx(expected));
		}
	}

	SECTION("Round")
	{
		REQUIRE(Mathfx::Round(3.2_fx64) == 3_fx64);
		REQUIRE(Mathfx::RoundToInt(3.2_fx64) == 3);

		REQUIRE(Mathfx::Round(4.8_fx64) == 5_fx64);
		REQUIRE(Mathfx::RoundToInt(4.8_fx64) == 5);

		REQUIRE(Mathfx::Round(6.5_fx64) == 6_fx64);
		REQUIRE(Mathfx::RoundToInt(6.5_fx64) == 6);

		REQUIRE(Mathfx::Round(7.5_fx64) == 8_fx64);
		REQUIRE(Mathfx::RoundToInt(7.5_fx64) == 8);

		REQUIRE(Mathfx::Round(-3.2_fx64) == -3_fx64);
		REQUIRE(Mathfx::RoundToInt(-3.2_fx64) == -3);

		REQUIRE(Mathfx::Round(-4.8_fx64) == -5_fx64);
		REQUIRE(Mathfx::RoundToInt(-4.8_fx64) == -5);

		REQUIRE(Mathfx::Round(-6.5_fx64) == -6_fx64);
		REQUIRE(Mathfx::RoundToInt(-6.5_fx64) == -6);

		REQUIRE(Mathfx::Round(-7.5_fx64) == -8_fx64);
		REQUIRE(Mathfx::RoundToInt(-7.5_fx64) == -8);

		for (auto raw : testCases)
		{
			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);

			double expected = std::round(d);
			fixed64 f = Mathfx::Round(e);
			double actual = static_cast<double>(f);

			CAPTURE(expected, actual, e.rawValue, f.rawValue);
			CHECK(actual == Approx(expected));
		}
	}

	SECTION("Pow2")
	{
		for (auto raw : testCases)
		{
			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);

			double expected = std::min(std::pow(2, d), static_cast<double>(fixed64::MaxValue));
			double actual = static_cast<double>(Mathfx::Pow2(e));
			CHECK(actual == Approx(expected).margin(0.000001));
		}
	}

	SECTION("Pow")
	{
		for (auto rawB : testCases)
		{
			fixed64 b = fixed64(rawB);
			for (auto rawE : testCases)
			{
				fixed64 e = fixed64(rawE);

				if (b == fixed64::Zero && e < fixed64::Zero)
				{
					continue;
				}
				else if (b < fixed64::Zero && e != fixed64::Zero)
				{
					continue;
				}

				double ed = static_cast<double>(e);
				double bd = static_cast<double>(b);

				double expected;
				if (e == fixed64::Zero)
				{
					expected = 1;
				}
				else
				{
					if (b == fixed64::Zero)
					{
						expected = 0;
					}
					else
					{
						expected = std::min(std::pow(bd, ed), static_cast<double>(fixed64::MaxValue));
					}
				}

				// Increase margin in cases where result or exponent is further from zero
				double margin = 0.000001;
				if (std::abs(ed) > 100000000)
				{
					margin = 0.5;
				}
				else if (expected > 100000000)
				{
					margin = 10;
				}
				else if (expected > 1000)
				{
					margin = 0.5;
				}

				if (b.rawValue == 0x3e1 && e.rawValue == -0x5b02bf3c5ab6551d)
				{
					fixed64 _ = Mathfx::Pow(b, e);
				}

				fixed64 actualF = Mathfx::Pow(b, e);
				double actual = static_cast<double>(actualF);

				if (actual != Approx(expected).margin(margin))
				{
					// When outside of the margin there are certain cases that we expect differences from std::pow
					REQUIRE(((actualF == fixed64::MaxValue && expected == 0.0)
						|| (actualF == fixed64::OneOverMaxValue && expected == Approx(static_cast<double>(fixed64::MaxValue)))));
				}
				else
				{
					CHECK(actual == Approx(expected).margin(margin));
				}
			}
		}
	}

	SECTION("Sqrt")
	{
		REQUIRE(Mathfx::Sqrt(0_fx64) == 0_fx64);
		REQUIRE(Mathfx::Sqrt(1_fx64) == 1_fx64);
		REQUIRE(Mathfx::Sqrt(4_fx64) == 2_fx64);
		REQUIRE(Mathfx::Sqrt(9_fx64) == 3_fx64);
		REQUIRE(Mathfx::Sqrt(16_fx64) == 4_fx64);
		REQUIRE(Mathfx::Sqrt(25_fx64) == 5_fx64);
		REQUIRE(Mathfx::Sqrt(36_fx64) == 6_fx64);
		REQUIRE(Mathfx::Sqrt(49_fx64) == 7_fx64);
		REQUIRE(Mathfx::Sqrt(64_fx64) == 8_fx64);
		REQUIRE(Mathfx::Sqrt(81_fx64) == 9_fx64);
		REQUIRE(Mathfx::Sqrt(100_fx64) == 10_fx64);

		constexpr double tolerance = 0.0000001;
		for (auto raw : testCases)
		{
			if (raw < 0)
			{
				continue;
			}

			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);
			CAPTURE(raw, e, d);
			CHECK(static_cast<double>(Mathfx::Sqrt(e)) == Approx(std::sqrt(d)).margin(tolerance));
		}

		REQUIRE(Mathfx::FastSqrt(0_fx64) == 0_fx64);
		REQUIRE(Mathfx::FastSqrt(1_fx64) == 1_fx64);
		REQUIRE(Mathfx::FastSqrt(4_fx64) == 2_fx64);
		REQUIRE(Mathfx::FastSqrt(9_fx64) == 3_fx64);
		REQUIRE(Mathfx::FastSqrt(16_fx64) == 4_fx64);
		REQUIRE(Mathfx::FastSqrt(25_fx64) == 5_fx64);
		REQUIRE(Mathfx::FastSqrt(36_fx64) == 6_fx64);
		REQUIRE(Mathfx::FastSqrt(49_fx64) == 7_fx64);
		REQUIRE(Mathfx::FastSqrt(64_fx64) == 8_fx64);
		REQUIRE(Mathfx::FastSqrt(81_fx64) == 9_fx64);
		REQUIRE(Mathfx::FastSqrt(100_fx64) == 10_fx64);

		for (auto raw : testCases)
		{
			if (raw < 0)
			{
				continue;
			}

			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);

			double margin = 0.0001;
			// For very large values the margin can get a little wacky, mostly within ~10 but can be off by as much as a few thousand
			if (d > 1e9)
			{
				margin = 10000;
			}

			fixed64 actualF = Mathfx::FastSqrt(e);
			double actual = static_cast<double>(actualF);
			double expected = std::sqrt(d);
			CAPTURE(d, actual, expected);
			CHECK(actual == Approx(expected).margin(margin));
		}
	}

	SECTION("Log2")
	{
		constexpr double tolerance = 0.0000001;
		for (auto raw : testCases)
		{
			if (raw <= 0)
			{
				continue;
			}

			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);
			CHECK(static_cast<double>(Mathfx::Log2(e)) == Approx(std::log2(d)).margin(tolerance));
		}
	}

	SECTION("Ln")
	{
		constexpr double tolerance = 0.0000001;
		for (auto raw : testCases)
		{
			if (raw <= 0)
			{
				continue;
			}

			fixed64 e = fixed64(raw);
			double d = static_cast<double>(e);
			CHECK(static_cast<double>(Mathfx::Ln(e)) == Approx(std::log(d)).margin(tolerance));
		}
	}
}

TEST_CASE("Trigonometry", "[fixedmath]")
{
	constexpr double kSafeTrigEpsilon = 0.000001;
	constexpr double kFastTrigEpsilon = 0.001;

	SECTION("Sin")
	{
		REQUIRE(Mathfx::Sin(fixed64::Zero) == fixed64::Zero);

		REQUIRE(Mathfx::Sin(fixed64::PiOver2) == fixed64::One);
		REQUIRE(Mathfx::Sin(fixed64::Pi) == fixed64::Zero);
		REQUIRE(Mathfx::Sin(fixed64::Pi + fixed64::PiOver2) == -fixed64::One);
		REQUIRE(Mathfx::Sin(fixed64::TwoPi) == fixed64::Zero);

		REQUIRE(Mathfx::Sin(-fixed64::PiOver2) == -fixed64::One);
		REQUIRE(Mathfx::Sin(-fixed64::Pi) == fixed64::Zero);
		REQUIRE(Mathfx::Sin(-fixed64::Pi - fixed64::PiOver2) == fixed64::One);
		REQUIRE(Mathfx::Sin(-fixed64::TwoPi) == fixed64::Zero);

		for (double angle = -2 * std::numbers::pi_v<double>; angle <= 2 * std::numbers::pi_v<double>; angle += 0.0001)
		{
			fixed64 f = fixed64::Float(angle);
			fixed64 actualF = Mathfx::Sin(f);
			double expected = std::sin(angle);
			double actual = static_cast<double>(actualF);
			CHECK(actual == Approx(expected).margin(kSafeTrigEpsilon));
		}

		REQUIRE(Mathfx::FastSin(fixed64::Zero) == fixed64::Zero);

		REQUIRE(Mathfx::FastSin(fixed64::PiOver2) == fixed64::One);
		REQUIRE(Mathfx::FastSin(fixed64::Pi) == fixed64::Zero);
		REQUIRE(Mathfx::FastSin(fixed64::Pi + fixed64::PiOver2) == -fixed64::One);
		REQUIRE(Mathfx::FastSin(fixed64::TwoPi) == fixed64::Zero);

		REQUIRE(Mathfx::FastSin(-fixed64::PiOver2) == -fixed64::One);
		REQUIRE(Mathfx::FastSin(-fixed64::Pi) == fixed64::Zero);
		REQUIRE(Mathfx::FastSin(-fixed64::Pi - fixed64::PiOver2) == fixed64::One);
		REQUIRE(Mathfx::FastSin(-fixed64::TwoPi) == fixed64::Zero);

		for (double angle = -2 * std::numbers::pi_v<double>; angle <= 2 * std::numbers::pi_v<double>; angle += 0.0001)
		{
			fixed64 f = fixed64::Float(angle);
			fixed64 actualF = Mathfx::FastSin(f);
			double expected = std::sin(angle);
			double actual = static_cast<double>(actualF);
			CHECK(actual == Approx(expected).margin(kFastTrigEpsilon));
		}
	}

	SECTION("Cos")
	{
		REQUIRE(Mathfx::Cos(fixed64::Zero) == fixed64::One);

		REQUIRE(Mathfx::Cos(fixed64::PiOver2) == fixed64::Zero);
		REQUIRE(Mathfx::Cos(fixed64::Pi) == -fixed64::One);
		REQUIRE(Mathfx::Cos(fixed64::Pi + fixed64::PiOver2) == fixed64::Zero);
		REQUIRE(Mathfx::Cos(fixed64::TwoPi) == fixed64::One);

		REQUIRE(Mathfx::Cos(-fixed64::PiOver2) == -fixed64::Zero);
		REQUIRE(Mathfx::Cos(-fixed64::Pi) == -fixed64::One);
		REQUIRE(Mathfx::Cos(-fixed64::Pi - fixed64::PiOver2) == fixed64::Zero);
		REQUIRE(Mathfx::Cos(-fixed64::TwoPi) == fixed64::One);

		for (double angle = -2 * std::numbers::pi_v<double>; angle <= 2 * std::numbers::pi_v<double>; angle += 0.0001)
		{
			fixed64 f = fixed64::Float(angle);
			fixed64 actualF = Mathfx::Cos(f);
			double expected = std::cos(angle);
			double actual = static_cast<double>(actualF);
			CHECK(actual == Approx(expected).margin(kSafeTrigEpsilon));
		}

		REQUIRE(Mathfx::FastCos(fixed64::Zero) == fixed64::One);

		REQUIRE(Mathfx::FastCos(fixed64::PiOver2) == fixed64::Zero);
		REQUIRE(Mathfx::FastCos(fixed64::Pi) == -fixed64::One);
		REQUIRE(Mathfx::FastCos(fixed64::Pi + fixed64::PiOver2) == fixed64::Zero);
		REQUIRE(Mathfx::FastCos(fixed64::TwoPi) == fixed64::One);

		REQUIRE(Mathfx::FastCos(-fixed64::PiOver2) == -fixed64::Zero);
		REQUIRE(Mathfx::FastCos(-fixed64::Pi) == -fixed64::One);
		REQUIRE(Mathfx::FastCos(-fixed64::Pi - fixed64::PiOver2) == fixed64::Zero);
		REQUIRE(Mathfx::FastCos(-fixed64::TwoPi) == fixed64::One);

		constexpr double fastCosTolerance = 0.0001;
		for (double angle = -2 * std::numbers::pi_v<double>; angle <= 2 * std::numbers::pi_v<double>; angle += 0.0001)
		{
			fixed64 f = fixed64::Float(angle);
			fixed64 actualF = Mathfx::FastCos(f);
			double expected = std::cos(angle);
			double actual = static_cast<double>(actualF);
			CHECK(actual == Approx(expected).margin(kFastTrigEpsilon));
		}
	}

	SECTION("Tan")
	{
		REQUIRE(Mathfx::Tan(fixed64::Zero) == fixed64::Zero);
		REQUIRE(Mathfx::Tan(fixed64::Pi) == fixed64::Zero);
		REQUIRE(Mathfx::Tan(-fixed64::Pi) == fixed64::Zero);

		REQUIRE(Mathfx::Tan(fixed64::PiOver2 - 0.001_fx64) > fixed64::Zero);
		REQUIRE(Mathfx::Tan(fixed64::PiOver2 + 0.001_fx64) < fixed64::Zero);
		REQUIRE(Mathfx::Tan(-fixed64::PiOver2 - 0.001_fx64) > fixed64::Zero);
		REQUIRE(Mathfx::Tan(-fixed64::PiOver2 + 0.001_fx64) < fixed64::Zero);

		constexpr double pi = std::numbers::pi_v<double>;
		constexpr double twopi = std::numbers::pi_v<double> *2;
		constexpr double piover2 = std::numbers::pi_v<double> / 2.0;

		for (double angle = -twopi; angle <= twopi; angle += 0.0001)
		{
			double a = angle;
			while (a < 0) a += twopi;
			while (a > twopi) a -= twopi;

			// Calculate a normalized distance from tangents (90 and 270 degrees)
			double t = (a <= pi) ?
				1 - (std::abs(piover2 - a) / piover2) :
				1 - (std::abs(3 * piover2 - a) / piover2);

			// If too close to the tangent just ignore since the result will get increasingly wrong
			if (t > 0.99)
			{
				continue;
			}

			// Dynamically adjust acceptable margin of error based on normalized distance to tangent
			double maxMargin = 0.01;
			double minMargin = 0.0001;
			double marginDelta = maxMargin - minMargin;

			// Interpolates exponentially
			double margin = minMargin + marginDelta * (t * t);

			fixed64 f = fixed64::Float(angle);
			fixed64 actualF = Mathfx::Tan(f);
			double expected = std::tan(angle);
			double actual = static_cast<double>(actualF);
			CHECK(actual == Approx(expected).margin(margin));
		}
	}

	SECTION("Acos")
	{
		REQUIRE(fixed64::Zero == Mathfx::Acos(fixed64::One));
		REQUIRE(fixed64::PiOver2 == Mathfx::Acos(fixed64::Zero));
		REQUIRE(fixed64::Pi == Mathfx::Acos(-fixed64::One));

		for (double x = -1.0; x < 1.0; x += 0.001)
		{
			fixed64 f = fixed64::Float(x);
			fixed64 actualF = Mathfx::Acos(f);
			double expected = std::acos(x);
			double actual = static_cast<double>(actualF);
			CAPTURE(x, actual, expected);
			CHECK(actual == Approx(expected));
		}

		for (auto raw : testCases)
		{
			if (raw < -fixed64::One.rawValue || raw > fixed64::One.rawValue)
			{
				continue;
			}

			fixed64 f = fixed64(raw);
			fixed64 actualF = Mathfx::Acos(f);
			double expected = std::acos(static_cast<double>(f));
			double actual = static_cast<double>(actualF);
			CHECK(actual == Approx(expected));
		}
	}

	SECTION("Atan")
	{
		REQUIRE(fixed64::Zero == Mathfx::Atan(fixed64::Zero));

		for (double x = -1.0; x < 1.0; x += 0.001)
		{
			fixed64 f = fixed64::Float(x);
			fixed64 actualF = Mathfx::Atan(f);
			double expected = std::atan(x);
			double actual = static_cast<double>(actualF);
			CAPTURE(x, actual, expected);
			CHECK(actual == Approx(expected).margin(0.0000001));
		}

		for (auto raw : testCases)
		{
			fixed64 f = fixed64(raw);
			fixed64 actualF = Mathfx::Atan(f);
			double expected = std::atan(static_cast<double>(f));
			double actual = static_cast<double>(actualF);
			CHECK(actual == Approx(expected));
		}
	}

	SECTION("Atan2")
	{
		REQUIRE(Mathfx::Atan2(fixed64::Zero, fixed64::NegOne) == fixed64::Pi);
		REQUIRE(Mathfx::Atan2(fixed64::Zero, fixed64::Zero) == fixed64::Zero);
		REQUIRE(Mathfx::Atan2(fixed64::Zero, fixed64::One) == fixed64::Zero);
		REQUIRE(Mathfx::Atan2(fixed64::One, fixed64::Zero) == fixed64::PiOver2);
		REQUIRE(Mathfx::Atan2(fixed64::NegOne, fixed64::Zero) == -fixed64::PiOver2);

		for (double y = -1.0; y < 1.0; y += 0.1)
		{
			for (double x = -1.0; x < 1.0; x += 0.1)
			{
				fixed64 xf = fixed64::Float(x);
				fixed64 yf = fixed64::Float(y);

				if (xf == fixed64::Zero && yf == fixed64::Zero)
				{
					continue;
				}

				// Special case, if y is so small that it comes out as 0 when converted to fixed very small values of y
				// might result in very different outputs of atan2 unless y is exactly 0
				if (yf.rawValue == 0 && y != 0.0)
				{
					y = 0.0;
				}

				fixed64 actualF = Mathfx::Atan2(yf, xf);
				double expected = std::atan2(y, x);
				double actual = static_cast<double>(actualF);
				CAPTURE(x, y, xf.rawValue, yf.rawValue, actual, expected);
				REQUIRE(actual == Approx(expected).margin(0.005));
			}
		}

		for (auto rawX : testCases)
		{
			for (auto rawY : testCases)
			{
				fixed64 xf = fixed64(rawX);
				fixed64 yf = fixed64(rawY);

				if (xf == fixed64::Zero && yf == fixed64::Zero)
				{
					continue;
				}

				fixed64 actualF = Mathfx::Atan2(yf, xf);
				double expected = std::atan2(static_cast<double>(yf), static_cast<double>(xf));
				double actual = static_cast<double>(actualF);
				CAPTURE(xf.rawValue, yf.rawValue, actual, expected);
				REQUIRE(actual == Approx(expected).margin(0.005));
			}
		}
	}
}

#ifdef CATCH_CONFIG_ENABLE_BENCHMARKING

TEST_CASE("Benchmarks", "[fixedmath]")
{
	SECTION("Arithmetic")
	{
		BENCHMARK_ADVANCED("SafeAdd")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return fixed64::SafeAdd(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("FastAdd")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return fixed64::FastAdd(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("float::operator+")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return v[i] + v[i]; });
		};

		BENCHMARK_ADVANCED("SafeSub")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return fixed64::SafeSub(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("FastSub")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return fixed64::FastSub(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("float::operator-")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return v[i] - v[i]; });
		};

		BENCHMARK_ADVANCED("SafeMul")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return fixed64::SafeMul(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("FastMul")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return fixed64::FastMul(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("float::operator*")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return v[i] * v[i]; });
		};

		BENCHMARK_ADVANCED("SafeMod")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_pos_fixed(); });
			meter.measure([&v](int i) { return fixed64::SafeMod(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("FastMod")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_pos_fixed(); });
			meter.measure([&v](int i) { return fixed64::FastMod(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("std::fmodf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_pos_float(); });
			meter.measure([&v](int i) { return std::fmodf(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("SafeDiv")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_pos_fixed(); });
			meter.measure([&v](int i) { return fixed64::SafeMod(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("FastDiv")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_pos_fixed(); });
			meter.measure([&v](int i) { return fixed64::FastMod(v[i], v[i]); });
		};

		BENCHMARK_ADVANCED("float::operator/")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_pos_float(); });
			meter.measure([&v](int i) { return v[i] / v[i]; });
		};
	}

	SECTION("Basic Math")
	{

		BENCHMARK_ADVANCED("Sign")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Sign(v[i]); });
		};

		BENCHMARK_ADVANCED("Abs")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Abs(v[i]); });
		};

		BENCHMARK_ADVANCED("std::fabsf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return std::fabsf(v[i]); });
		};

		BENCHMARK_ADVANCED("Floor")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Floor(v[i]); });
		};

		BENCHMARK_ADVANCED("std::floorf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return std::floorf(v[i]); });
		};

		BENCHMARK_ADVANCED("Ceiling")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Ceiling(v[i]); });
		};

		BENCHMARK_ADVANCED("std::ceilf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return std::ceilf(v[i]); });
		};

		BENCHMARK_ADVANCED("Round")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Round(v[i]); });
		};

		BENCHMARK_ADVANCED("std::roundf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return std::roundf(v[i]); });
		};

		BENCHMARK_ADVANCED("Sqrt")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_pos_fixed(256_fx64); });
			meter.measure([&v](int i) { return Mathfx::Sqrt(v[i]); });
		};
		BENCHMARK_ADVANCED("FastSqrt")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_pos_fixed(256_fx64); });
			meter.measure([&v](int i) { return Mathfx::FastSqrt(v[i]); });
		};

		BENCHMARK_ADVANCED("std::sqrtf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_pos_float(256.0f); });
			meter.measure([&v](int i) { return std::sqrtf(v[i]); });
		};

		BENCHMARK_ADVANCED("Pow2")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Pow2(v[i]); });
		};

		BENCHMARK_ADVANCED("std::powf(2,x)")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return std::powf(2, v[i]); });
		};

		BENCHMARK_ADVANCED("Pow")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> vBase(meter.runs());
			std::vector<fixed64> vExp(meter.runs());
			std::ranges::transform(vBase, vBase.begin(), [](const fixed64& x) { return random_pos_fixed(fixed64::Float(20.0f)); });
			std::ranges::transform(vExp, vExp.begin(), [](const fixed64& x) { return random_pos_fixed(fixed64::Float(50.0f)); });
			meter.measure([&vBase, &vExp](int i) { return Mathfx::Pow(vBase[i], vExp[i]); });
		};

		BENCHMARK_ADVANCED("std::powf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> vBase(meter.runs());
			std::vector<float> vExp(meter.runs());
			std::ranges::transform(vBase, vBase.begin(), [](const float& x) { return random_pos_float(20.0f); });
			std::ranges::transform(vExp, vExp.begin(), [](const float& x) { return random_pos_float(50.0f); });
			meter.measure([&vBase, &vExp](int i) { return std::powf(vBase[i], vExp[i]); });
		};

		BENCHMARK_ADVANCED("Log2")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_pos_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Log2(v[i]); });
		};

		BENCHMARK_ADVANCED("std::log2f")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_pos_float(); });
			meter.measure([&v](int i) { return std::log2f(v[i]); });
		};


		BENCHMARK_ADVANCED("Ln")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_pos_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Ln(v[i]); });
		};

		BENCHMARK_ADVANCED("std::logf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_pos_float(); });
			meter.measure([&v](int i) { return std::logf(v[i]); });
		};
	}

	SECTION("Trigonometry")
	{
		BENCHMARK_ADVANCED("Sin")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Sin(v[i]); });
		};

		BENCHMARK_ADVANCED("FastSin")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::FastSin(v[i]); });
		};

		BENCHMARK_ADVANCED("std::sinf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return std::sinf(v[i]); });
		};

		BENCHMARK_ADVANCED("Cos")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Cos(v[i]); });
		};

		BENCHMARK_ADVANCED("FastCos")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::FastCos(v[i]); });
		};

		BENCHMARK_ADVANCED("std::cosf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return std::cosf(v[i]); });
		};

		BENCHMARK_ADVANCED("Tan")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Tan(v[i]); });
		};

		BENCHMARK_ADVANCED("std::tanf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_float(); });
			meter.measure([&v](int i) { return std::tanf(v[i]); });
		};

		BENCHMARK_ADVANCED("Acos")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_snorm_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Acos(v[i]); });
		};

		BENCHMARK_ADVANCED("std::acosf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_snorm_float(); });
			meter.measure([&v](int i) { return std::acosf(v[i]); });
		};

		BENCHMARK_ADVANCED("Atan")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const fixed64& x) { return random_snorm_fixed(); });
			meter.measure([&v](int i) { return Mathfx::Atan(v[i]); });
		};

		BENCHMARK_ADVANCED("std::atanf")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> v(meter.runs());
			std::ranges::transform(v, v.begin(), [](const float& x) { return random_snorm_float(); });
			meter.measure([&v](int i) { return std::atanf(v[i]); });
		};

		BENCHMARK_ADVANCED("Atan2")(Catch::Benchmark::Chronometer meter) {
			std::vector<fixed64> vx(meter.runs());
			std::vector<fixed64> vy(meter.runs());
			std::ranges::transform(vx, vx.begin(), [](const fixed64& x) { return random_snorm_fixed(); });
			std::ranges::transform(vy, vy.begin(), [](const fixed64& x) { return random_snorm_fixed(); });
			meter.measure([&vx, &vy](int i) { return Mathfx::Atan2(vy[i], vx[i]); });
		};

		BENCHMARK_ADVANCED("std::atan2f")(Catch::Benchmark::Chronometer meter) {
			std::vector<float> vx(meter.runs());
			std::vector<float> vy(meter.runs());
			std::ranges::transform(vx, vx.begin(), [](const float& x) { return random_snorm_float(); });
			std::ranges::transform(vy, vy.begin(), [](const float& x) { return random_snorm_float(); });
			meter.measure([&vx, &vy](int i) { return std::atan2f(vy[i], vx[i]); });
		};
	}
}

#endif

TEST_CASE("Vector2fx", "[fixedmath]")
{
	Vector2fx v0 = Vector2fx::Zero;
	Vector2fx v1 = Vector2fx::Right;
	REQUIRE(Vector2fx::Zero + Vector2fx::Right == Vector2fx::Right);
	REQUIRE(Vector2fx::Zero + Vector2fx::Left == Vector2fx::Left);
	REQUIRE(Vector2fx::Zero + Vector2fx::Up == Vector2fx::Up);
	REQUIRE(Vector2fx::Zero + Vector2fx::Down == Vector2fx::Down);
	REQUIRE(Vector2fx::Zero + Vector2fx::One == Vector2fx::One);
}

int main(int argc, char* argv[])
{
	Catch::Session session; // There must be exactly one instance

	// writing to session.configData() here sets defaults
	// this is the preferred way to set them

	session.configData().benchmarkNoAnalysis = true;
	session.configData().shouldDebugBreak = false;
	session.configData().benchmarkSamples = 200;

	if (int returnCode = session.applyCommandLine(argc, argv); returnCode != 0)
	{
		return returnCode;
	}

	// writing to session.configData() or session.Config() here 
	// overrides command line args
	// only do this if you know you need to

	G.rng = std::mt19937_64(std::chrono::utc_clock::now().time_since_epoch().count());

	int numFailed = session.run();

	// numFailed is clamped to 255 as some unices only use the lower 8 bits.
	// This clamping has already been applied, so just return it here
	// You can also do any post run clean-up here
	return numFailed;
}

