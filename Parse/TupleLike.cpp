#pragma once

// #include<iostream> // std::cout for print

namespace pse
{

	template<typename T, typename... Ts>
	struct TupleLike;

	template<typename T>
	struct TupleLike<T>;

	template<typename T, typename B, typename A, typename...Bs> // Bs need to be here because of greedy matching
	auto make_TupleLike(T t, TupleLike<A, Bs...> tuple, B b)
	{
		//return Tuple<T, A, Bs..., B>(t, tuple/*Find a way of getting data out of the tuple*/, b)
		return make_TupleLike(t, tuple.first, b);
		/*
		* FIXME: This does not do what it is supposed to do:
		* It is supposed to use all elements of the TupleLike type
		*
		* Needs to pass in both tuple.first and tuple.seconds.
		*/
	}

	template<typename T, typename S, typename B, typename A>
	auto make_TupleLike(T t, S s, TupleLike<A> tuple, B b)
	{
		return TupleLike<T, S, A, B>();
	}

	template<auto T, auto C, auto A, auto... B>
	auto make_tuplelike()
	{
		/*
		* Was thinking of just passing in A and B...
		*
		* But could pass them in as NTTP
		*/

		return TupleLike<decltype(T), decltype(A), decltype(B)..., decltype(C)>(T, A, B..., C);
	}

	struct NoType {};

	// we need to make s varaidic because of the increasing template parameters, could just do
	// it like in c++98 where you have to manually do it.
	template<typename T, typename S, typename B, typename A, typename... Bs>
	auto make_TupleLike(T t, S s, TupleLike<A, Bs...> tuple, B b)
	{
		make_TupleLike(t, s, tuple.first, tuple.seconds, b);
		// need to make this now variadic becuae the amount of .first's and seconds' goes up linearly.
	}

	/*
	* auto make_TupleLike(T t, TupleLike)
	*/

	inline namespace global_elements_tupleLike
	{
		static int count = 0;
	}

	template<typename T, typename... Ts>
	auto GetParameterPackIndex(TupleLike<T, Ts...> tuple, auto index)
	{
		if (global_elements_tupleLike::count == index)
		{
			return tuple.first;
		}
		else
		{
			// we have to increment global_elements_tupleLike like this:
			global_elements_tupleLike::count += 1;
			return GetParameterPackIndex(tuple.seconds, index);
		}
	}

	/*
This function is going to be used to call get_tuple_elements n number of times on an element.
*/

// Do_Times that takes a callable that accepts no args.
	constexpr auto Do_Times(auto& Callable, int NumTimes)
	{
		for (int i = 0; i < NumTimes; i++)
		{
			return Callable();
		}
	}

	/*
	* A problem with Do_Times is that, we need it to be able to be done inside a parameter pack but this just returns something once,
	as soon as the loops starts the loop will and and the rest of the code will not excecute. We need to have something lik:

	get_tuple_elements(TupleThatCanBeExpanded)...;
	*/

	// overload that takes a callable with some args. Constexpr so can appear in a parameter pack sequence
	template<typename T, typename... Ts>
	auto Do_Times(auto& Callable, int NumTimes, T t, Ts&... ts)
	{
		for (int i = 0; i < NumTimes; i++)
		{
			return Callable(t, ts...);
		}
	}

	// notice taking them all by reference. Needs reference because of things like get_tuple_elements relies on it not being a copy
	template<typename T>
	constexpr auto Do_Times(auto& Callable, int NumTimes, T& t)
	{
		for (int i = 0; i < NumTimes; i++)
		{
			return Callable(t);
		}
	}

	// cosntexpred becuase operation can happen at compile time and should happen at compile time.
	template<typename T>
	constexpr auto GetParameterPackIndex(TupleLike<T> tuple, auto index)
	{
		return tuple.first; // no exception hanling currently, if wrong input from the user it is the user's fault.
	}

	template<typename T, typename... Ts>
	auto get_tuple_elements(TupleLike<T, Ts...>& tuple)
	{
		if (tuple.index == 0)
		{
			return T;
		}
		else
		{
			/*
			Index is to show that becuase we are going to use this function like this:
			get_tuple_elements(tuple)...
			We are going to need to know how deep we need to go in the TupleLike object. For instance:

			tuple.first;
			tuple.seconds;
			tuple.seconds.first;
			tuple.seconds.seconds.first;

			Extra so we need a count to track to get all of the elements in the TupleLike object.
			*/
			tuple.index++;
			return GetParameterPackIndex(tuple, tuple.index);
		}
	}

	// pointless so i have depracated it. Shouldnt really use atall.
	template<typename T, typename... Ts>
	[[deprecated]] auto GetType(T t, Ts... ts)
	{
		return 0;//(t + ts)...;
	}

	//  This one works. So this one does not need fixing
	template<typename T, typename B, typename A>
	auto make_TupleLike(T t, TupleLike<A> tuple, B b)
	{
		return Tuple<T, A, B>(t, tuple.first, b);
	}

	// TODO: Come back to this becuase i have no more ideas for using the parameter pack.

	/*
	Have hit a conumdrum with make_TupleLike becuase i can't define it like this:

	template<typename T, typename B, typename... Ts, typename... Tstwo>
	auto make_TupleLike(T t, Ts... ts, TupleLike<Tstwo...> tuple, B b)
	{
		return make_TupleLike(t, ts..., tuple.first, tuple.seconds, b);
	}

	and then have the base case as:

	template<typename T, typename... Ts, typename Ttwo>
	auto make_TupleLike(T t, Ts... ts, TupleLike<Ttwo> tuple, B b)
	{
		return TupleLike<T, Ts..., decltype(tuple.first), B>(t, ts..., tuple.first, b);
	}

	but is impossible becuase i am not allowed two parameter packs together because
	which one they would go in would be ambiguous.

	so i am not sure what to do.

	I could have one parameter pack. Yes that is what i will do. No i cannot do that because i have got to have that tuple variadic args.
	*/

	// cant acually implement this because i would need two parameter packs which is not possible.
	inline auto AssignElements()
	{
		// Do not implement
	}
	// was the cause of a error before because of a link error.

	template<typename T, typename B, typename... Ts>
	auto make_tuple_one_more(TupleLike<T, Ts...>& tuple, B b)
	{
		// so now we have a tuple with one more element and we just need to assign the elements over.
		auto NewTuple = TupleLike<T, Ts..., b>{};
		// AssignElements(tuple, NewTuple);

		// we can assign the first elements.
		NewTuple.first = tuple.first;

		int i = 0; // was going to be for the depth of the tuple
		// ie. if a tuple had a depth of 3 the following would be possible:
		// tuple.second.first // second: 1 + first: 1 + original tuple.first: 1 = 3.

/*
Tuple<Ts...> tuple{};
tuple.depth(); // need to implement depth function of TupleLike
*/
	}

	template<typename T, typename... Ts>
	struct TupleLike
	{
		template<typename B>
		auto push_back(B b) // need a different type because it is a tuple structure
		{
			auto curr_tuple = make_TupleLike(first, seconds, b);
			return curr_tuple; // curr_tuple is of type Tuple<decltype(first), decltype(b), decltype(seconds.get_data())
			// still to be implemented seconds of type TupleLike .get_data()
		}
		
		auto operator=(TupleLike<T, Ts...>& tup)
		{
			first = tup.first;
			seconds = tup.seconds;
		}

		// TupleLike(TupleLike<T, Ts...> tuple);

		T first;
		TupleLike<Ts...> seconds;
		int index = 0;
		static constexpr int value = 1;

		auto SizeDepth() -> int; // returns amount of parameter packs inside Ts... + 1.

		template<int i> // needed for varying return types.
		auto Depth(); // takes an integer and returns the depth element.

		auto Depth(int i);

		auto print(); // print the elements in the tuple
		auto print(bool b);

		auto GetArr(int val); // dont implement can't do because elements would be of different type.

		template<typename B>
		auto GetTupleUninitializedAdd();

		// only works if all types in the pse::TupleLike are of the same type
		auto operator[](int i) -> T
		{
			if (i == 0)
			{
				return first;
			}
			else
			{
				return seconds[i - 1];
			}
		}
	};

	template<typename T, typename... Ts>
	template<typename B>
	auto TupleLike<T, Ts...>::GetTupleUninitializedAdd()
	{
		return TupleLike<T, Ts..., B>{};
	}

	/*
	* TupleLike<T, Ts... B> tuple{};
	*/

	template<typename T, typename... Ts>
	auto TupleLike<T, Ts...>::print()
	{
		//std::cout << "\n";
		auto x = SizeDepth();
		//std::cout << first << ", ";
		seconds.print();

		//std::cout << "\n";
	}

	template<typename T, typename... Ts>
	auto TupleLike<T, Ts...>::print(bool b)
	{
		//std::cout << "\n";
		if (b != false)
		{
			//std::cout << "[";
		}
		//std::cout << "\n";
		auto x = SizeDepth();
		//std::cout << first << ", ";
		seconds.print(false);

		if (b != false)
		{
			//std::cout << "]\n";
		}

		//std::cout << "\n";
	}

	inline namespace parameter_pack_size
	{
		static int sizeParameter{};
	}

	namespace parameter_pack
	{
		struct Yes {};
		struct No {};
	};

	namespace parameter
	{
		struct Variadic {};
		struct Single {};
	};

	// doesnt need to take any arguaments.
	template<typename T, typename... Ts> // overloaded on parameter pack.
	auto FindSizeOfParameterPack(parameter_pack::Yes& y, parameter::Variadic& v) -> int // parameter not used
	{
		parameter_pack_size::sizeParameter += 1;
		return FindSizeOfParameterPack<Ts...>(y, v); // temporarys so that you can overload fuction calls
	}

	// this overload is when we are passed a parameter_pack::Yes but we need to change it when calling at end of function.
	template<typename T, typename Ttwo>
	auto FindSizeOfParameterPack(parameter_pack::Yes& y, parameter::Variadic& v)
	{ // would still think when passing it looks like a parameter pack
		parameter_pack_size::sizeParameter += 1;
		return FindSizeOfParameterPack<Ttwo>(parameter_pack::No{}, parameter::Single{});
	}

	/*
	* Now we need to also pass in parameter::variadic or parameter::typename because again can't overload just on template
	*/

	// doesnt need to take any arguements.
	template<typename T>  // overloaded in paramter pack
	auto FindSizeOfParameterPack(parameter_pack::No& n, parameter::Single& s) -> int
	{
		parameter_pack_size::sizeParameter += 1;
		auto x = parameter_pack_size::sizeParameter;
		parameter_pack_size::sizeParameter = 0; // reset it for next operation.
		return x;
	}

	//template<>  // overloaded in paramter pack
	//auto FindSizeOfParameterPack(parameter_pack::No& n, parameter::Single& s) -> int
	//{
	//	auto x = parameter_pack_size::sizeParameter;
	//	parameter_pack_size::sizeParameter = 0; // reset it for next operation.
	//	return x;
	//}

	/*
	* Because we don't know the size of variadic parameter pack, it could be empty or a single so overloads just in case.
	*
	* Commented the following selection out becuase was giving me the C2912 Visual Studio Microsoft Compiler Error.
	*/

	//template<>  // overloaded in paramter pack
	//auto FindSizeOfParameterPack(parameter_pack::No& n, parameter::Variadic& s) -> int
	//{
	//	auto x = parameter_pack_size::sizeParameter;
	//	parameter_pack_size::sizeParameter = 0; // reset it for next operation.
	//	return x;
	//}

	//template<>  // overloaded in paramter pack
	//auto FindSizeOfParameterPack(parameter_pack::Yes& y, parameter::Single& s) -> int
	//{
	//	auto x = parameter_pack_size::sizeParameter;
	//	parameter_pack_size::sizeParameter = 0; // reset it for next operation.
	//	return x;
	//}

	//template<>  // overloaded in paramter pack
	//auto FindSizeOfParameterPack(parameter_pack::Yes& y, parameter::Variadic& s) -> int
	//{
	//	auto x = parameter_pack_size::sizeParameter;
	//	parameter_pack_size::sizeParameter = 0; // reset it for next operation.
	//	return x;
	//}

	template<typename T, typename... Ts>
	auto TupleLike<T, Ts...>::SizeDepth() -> int // returns the depth of the tuple
	{
		//return 1 + FindSizeOfParameterPack<Ts...>(parameter_pack::Yes{}, parameter::Variadic{}); // just returns the total depth.
		return value + seconds.value;
	}



	template<typename T, typename... Ts>
	template<int i>
	auto TupleLike<T, Ts...>::Depth(/*int i*/) // don't know the return type yet. 
	{
		if constexpr (i == 1)
		{
			return first;
		}
		else
		{
			return seconds.Depth(i - 1);
		}
	}

	// like the base class of TupleLike<T, Ts...>
	template<typename T>
	struct TupleLike<T>
	{
		T first;
		int index = 0;
		auto SizeDepth() { return 1; }
		auto Depth() { return first; }
		auto print() { /*std::cout << first;*/ }
		auto print(bool b)
		{
			//std::cout << first;
		}
		
		auto operator=(TupleLike<T>& tup)
		{
		    first = tup.first;
        }
		static constexpr int value = 1;

		// a better way than using get<0>() and so on.
		auto operator[](int i) -> T
		{
			if (i == 0)
			{
				return first;
			}
			else
			{
				return {};
			}
		}
	};
} // namespace pse
