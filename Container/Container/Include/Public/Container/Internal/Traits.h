#pragma once
#include <type_traits>
#include <Container/Internal/Type.h>

namespace container
{
	class ContainerImpl;

	namespace internal
	{
		void* GetOrCreate(ContainerImpl& containerImpl, Type::Id id, bool bIsParamConst);

		template <class Parent>
		class any_type
		{
		private:
			ContainerImpl& m_containerImpl;

			template<class T>
			using base_type = std::remove_cv_t<std::remove_reference_t<T>>;

			template<class Child>
			using is_copy_constructor = std::is_same<Parent, base_type<Child>>;

			template<class Child>
			using no_copy_constructor = std::enable_if_t <is_copy_constructor<Child>::value == false>;

			template <class T>
			T* getOrCreate() const
			{
				Type::Id id = Type::id<std::remove_const_t<T>>();
				bool bIsParamConstant = std::is_const_v<T>;

				void* child = GetOrCreate(m_containerImpl, id, bIsParamConstant);

				return reinterpret_cast<T*>(child);
			}

		public:
			any_type(ContainerImpl& containerImpl) : m_containerImpl(containerImpl) {}

			template <class Child, class = no_copy_constructor<Child>>
			operator Child& () const
			{
				return *getOrCreate<Child>();
			}

			template <class Child, class = no_copy_constructor<Child>>
			operator Child* () const
			{
				return getOrCreate<Child>();
			}
		};

		template <class...>
		struct type_list {};

		template <class Class, int ... ArgsNum>
		using param_array = any_type<Class>;

		template <class Class, int ... ArgsNum>
		using can_construct = std::is_constructible<Class, param_array<Class, ArgsNum>...>;

		template <class Class, int ... ArgsNum>
		using can_construct_true = std::enable_if_t<can_construct<Class, ArgsNum...>::value == true>;

		template <class Class, int ... ArgsNum>
		using can_construct_false = std::enable_if_t<can_construct<Class, ArgsNum...>::value == false>;

		template <class Class, class, class = void>
		struct constructor {};

		template <class Class, int ... ArgsNum>
		struct constructor<Class, std::index_sequence<ArgsNum...>, can_construct_true<Class, ArgsNum...>>
		{
			using args = type_list<param_array<Class, ArgsNum>...>;
		};

		template <class Class, int ... ArgsNum>
		struct constructor<Class, std::index_sequence<ArgsNum...>, can_construct_false<Class, ArgsNum...>>
		{
			using next = constructor<Class, std::make_index_sequence<sizeof...(ArgsNum) - 1>>;
			using args = typename next::args;
		};

		constexpr static int MaxArgs = 16;

		template<class Class>
		using args_list = typename constructor<Class, std::make_index_sequence<MaxArgs>>::args;

		template <class T, class>
		struct ConstructorFactoryImpl;

		template <class T, class... Args>
		struct ConstructorFactoryImpl<T, type_list<Args...>>
		{
			static void* Create(ContainerImpl& containerImpl, void* addr)
			{
				return (void*) new (addr) T(Args{ containerImpl }...);
			}
		};

		template<class Class>
		using ConstructorFactory = ConstructorFactoryImpl<Class, args_list<Class>>;

		template <class T>
		struct DestructorFactory
		{
			static void Destroy(void* instance)
			{
				reinterpret_cast<T*>(instance)->~T();
			}
		};
	}
}
