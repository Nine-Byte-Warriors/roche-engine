#pragma once
#ifndef RND_H
#define RND_H

#include <random>

using u32 = uint_least32_t;
using engine = std::mt19937;

class RND
{
public:
	static void Seed(const u32 uSeed) noexcept { m_Engine.seed(uSeed); }
	static void Seed() noexcept { m_Engine.seed(std::random_device()()); }

	static u32 Get(const u32 uMin, const u32 uMax) noexcept { return std::uniform_int_distribution<u32>(uMin, uMax)(m_Engine); }
	static u32 Get(const u32 uMax) noexcept { return std::uniform_int_distribution<u32>(0, uMax)(m_Engine); }
	static u32 Get() noexcept { return std::uniform_int_distribution<u32>(0, std::numeric_limits<u32>::max())(m_Engine); }

	static float GetF(const float fMin, const float fMax) noexcept { return std::uniform_real_distribution<float>(fMin, fMax)(m_Engine); }
	static float GetF(const float fMax) noexcept { return std::uniform_real_distribution<float>(0.0f, fMax)(m_Engine); }
	static float GetF() noexcept { return std::uniform_real_distribution<float>(0.0f, 1.0f)(m_Engine); }
	
private:
	RND() = default;
	~RND() = default;

	static engine m_Engine;
};

#endif // !RND
