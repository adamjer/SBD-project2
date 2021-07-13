#pragma once

#include"MyRand.h"
#include<functional>
#include<random>


namespace Jereczek
{
	template<typename T>
	class MyRand
	{
	public:
		MyRand(const T &low, const T &high)
			: r(std::bind(std::uniform_real_distribution<>(low, high + 1), std::default_random_engine(std::random_device{}())))
		{
		}

		T operator()()
		{
			return this->r();
		}

	private:
		std::function<T()> r;
	};
}