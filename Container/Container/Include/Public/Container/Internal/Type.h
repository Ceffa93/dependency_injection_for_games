#pragma once
#include <typeinfo>

namespace container
{
	namespace internal
	{
		struct Type
		{
			using Id = const char*;
			Id typeId;

			template <class T>
			static Id id()
			{
				return typeid(T).name();
			}
		};
	}
}
