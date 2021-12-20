#include <Container/Internal/Type.h>
#include <Private/ContainerImpl.h>

namespace container
{
	namespace internal
	{
		void* GetOrCreate(ContainerImpl& containerImpl, Type::Id id, bool bIsParamConst)
		{
			return containerImpl.getOrCreateConstSafe(id, bIsParamConst);
		}
	}
}