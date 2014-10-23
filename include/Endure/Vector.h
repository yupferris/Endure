#ifndef __ENDURE_VECTOR_H__
#define __ENDURE_VECTOR_H__

#include <memory>

namespace Endure
{
	template <typename T> class _Vector;
	template <typename T> class _Vector
	{
	public:
		_Vector()
			: count(0)
		{
		}

		int Count() const
		{
			return count;
		}

	private:
		const int count;
	};

	template <typename T> using Vector = std::shared_ptr<_Vector<T>>;

	template <typename T> Vector<T> CreateVector()
	{
		return std::make_shared<_Vector<T>>(_Vector<T>());
	}
}

#endif
