#pragma once

#include <random>
#include <vector>
#include <samchon/IterPair.hpp>

namespace samchon
{
	class Math
	{
	protected:
		static std::default_random_engine& randomGenerator()
		{
			static std::default_random_engine val;
			return val;
		};
		static std::uniform_real_distribution<double>& randomDistribution()
		{
			static std::uniform_real_distribution<double> val;
			return val;
		};

	public:
		inline static void initRandom()
		{
			/*
			�߻��Ǵ� ������ ������ ���Ϻ����� ������ �Ǵµ�
			�� ������ �ʱ�ȭ��
			*/
			randomDistribution() = std::uniform_real_distribution<double>(0.0, 1.0);
		};
		inline static double random()
		{
			return randomDistribution()(randomGenerator());
		};

		//�ּ� �ִ밪�� ���
		template <typename T> static auto calcMin(const std::vector<T> &vec) -> IterPair<T>
		{
			size_t size = vec.size();

			T val = vec.at(0);
			size_t position = 0;

			for (size_t i = 0; i < size; i++)
				if (vec.at(i) < val)
				{
					val = vec.at(i);
					position = i;
				}
			return{ val, position };
		}
		template <typename T> static auto calcMax(const std::vector<T> &vec) -> IterPair<T>
		{
			size_t size = vec.size();

			T val = vec.at(0);
			size_t position = 0;

			for (size_t i = 0; i < size; i++)
				if (vec.at(i) > val)
				{
					val = vec.at(i);
					position = i;
				}
			return{ val, position };
		}
	};
};