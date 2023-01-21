#pragma once
#ifndef RND_H
#define RND_H

//#include <random>
//
//using u32 = uint_least32_t;

class RND
{
public:
	//static void Seed(const u32 uSeed) noexcept { m_Engine.seed(uSeed); bIsSeeded = true; }
	//static void Seed() noexcept { m_Engine.seed(m_Device()); bIsSeeded = true; }

	//static u32 Get(const u32 uMin, const u32 uMax) noexcept { if (!bIsSeeded) Seed(); return std::uniform_int_distribution<u32>(uMin, uMax)(m_Engine); }
	//static u32 Get(const u32 uMax) noexcept { if (!bIsSeeded) Seed(); return std::uniform_int_distribution<u32>(0, uMax)(m_Engine); }
	//static u32 Get() noexcept { if (!bIsSeeded) Seed(); return std::uniform_int_distribution<u32>(0, std::numeric_limits<u32>::max())(m_Engine); }

	//static float GetF(const float fMin, const float fMax) noexcept { if (!bIsSeeded) Seed(); return std::uniform_real_distribution<float>(fMin, fMax)(m_Engine); }
	//static float GetF(const float fMax) noexcept { if (!bIsSeeded) Seed(); return std::uniform_real_distribution<float>(0.0f, fMax)(m_Engine); }
	//static float GetF() noexcept { if (!bIsSeeded) Seed(); return std::uniform_real_distribution<float>(0.0f, 1.0f)(m_Engine); }

	static int Get(int iMin, int iMax) noexcept { return iMin + std::rand() / ((RAND_MAX + iMin) / iMax); }
	static int Get(int iMax) noexcept { return std::rand() / (RAND_MAX / iMax); }
	static int Get() noexcept { return std::rand(); }
	
private:
	RND() = default;
	~RND() = default;

	//static std::random_device m_Device;
	//static std::mt19937 m_Engine;
	//static bool bIsSeeded;
};

#endif // !RND
