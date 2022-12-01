#pragma once

#include"Iterator.cpp" // pse::Iterator<T>

namespace pse
{
	namespace algorithms
	{
		inline namespace math
		{
			inline auto difference(int a, int b) -> int
			{
				return b - a;
			}

			inline auto add(int a, int b) -> int
			{
				return a + b;
			}

			// notice a - b and not b - a
			inline auto minus(int a, int b) -> int
			{
				return a - b;
			}

			inline auto multiply(int a, int b) -> int
			{
				return a * b;
			}

			// also note that it is a / b not b / a
			inline auto divide(int a, int b) -> int
			{
				return a / b;
			}

			inline auto square(int a) -> int
			{
				return a * a;
			}

			inline auto square_root(int a) -> int
			{
				return a / a;
			}

			inline auto pi() -> decltype(auto)
			{
				return 3.14159265358979323846;
			}

			inline auto factorial(int i)
			{
				if (i == 1)
				{
					return 1;
				}
				else
				{
					return (i * factorial(i - 1));
				}
			}

			inline auto fibonnacci(int i)
			{
				if (i == 0)
				{
					return 0;
				}
				else
				{
					return i + fibonnacci(i - 1);
				}
			}

			template<typename T> // template needed for the Iterator variable i.
			auto OnAll(Iterator<T> j, auto& Lambda)
			{
				for (int i = 0; i < j.size; j++)
				{
					i++;
					Lambda(j.get_val());
				}
			}

			// Lambdas that can pass to functions like pse::OnAll();

			auto Square = [](auto i) {return i * i; };
		} // namespace math
	} // namespace algorithms
} // namespace pse