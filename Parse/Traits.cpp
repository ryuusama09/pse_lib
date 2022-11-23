#pragma once

namespace pse
{
	namespace Traits
	{
		template<typename T>
		struct Is_Type
		{
			static constexpr auto value = true;
		};

		template<typename A, typename B>
		struct Is_Same
		{
			static constexpr auto value = false;
		};

		template<typename T>
		struct Add_Const
		{
			using value = const T;
		};

		template<typename T>
		struct Add_Const<T const>
		{
			using value = T;
		};

		template<typename T>
		struct Add_Ref
		{
			using value = T&;
		};

		template<typename T>
		struct Add_Ref<T&>
		{
			using value = T;
		};

		template<typename T>
		struct Add_Pointer
		{
			using value = T*;
		};

		template<typename T>
		struct Add_Pointer<T*>
		{
			using value = T;
		};

		template<typename T>
		struct Add_Pointer_Pointer
		{
			using value = T**;
		};

		template<typename T>
		struct Add_Pointer_Pointer<T*>
		{
			using value = T*;
		};

		template<typename T>
		struct Add_Pointer_Pointer<T**>
		{
			using value = T;
		};

		template<typename T>
		struct Add_Rvalue_Ref
		{
			using value = T&&;
		};

		template<typename T>
		struct Add_Rvalue_Ref<T&&>
		{
			using value = T;
		};

		template<typename T>
		struct Is_Same<T, T>
		{
			static constexpr auto value = true;
		};

		template<typename T>
		struct Is_Pointer
		{
			static constexpr bool value = false;
		};

		template<typename T>
		struct Is_Pointer<T*>
		{
			static constexpr bool value = true;
		};

		template<typename A, typename B>
		concept same_as = requires(A a, B b)
		{
			Is_Same<A, B>::value;
		};

		template<typename T>
		concept is_pointer = requires(T t)
		{
			Is_Pointer<T>::value == true;
		};

        template<typename T>
		concept is_ref = requires(T t)
		{
			Is_Same < T, Add_Ref<T>::value>::value == true;
		};

		template<typename T>
		concept is_rvalue_ref = requires(T t)
		{
			Is_Same<T, Add_Rvalue_Ref<T>::value>::value == true;
		};

	} // namespace Traits
} // namespace pse