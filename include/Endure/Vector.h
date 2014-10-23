#ifndef __ENDURE_VECTOR_H__
#define __ENDURE_VECTOR_H__

#include <memory>

namespace Endure
{
	template <typename T> class _Node
	{
	public:
		T Elements[32];
	};

	template <typename T> class _Vector;
	template <typename T> class _Vector
	{
	public:
		_Vector()
			: root(nullptr), count(0)
		{
		}

		_Vector(std::shared_ptr<_Node<T>> root)
			: root(root), count(1)
		{
		}

		int Count() const
		{
			return count;
		}

		T Get(int key)
		{
			return root->Elements[key];
		}

	private:
		const std::shared_ptr<_Node<T>> root;
		const int count;
	};

	template <typename T> using Vector = std::shared_ptr<_Vector<T>>;

	template <typename T> Vector<T> CreateVector()
	{
		return std::make_shared<_Vector<T>>(_Vector<T>());
	}

	template <typename T> Vector<T> Conj(Vector<T> vector, T item)
	{
		auto n = std::shared_ptr<_Node<T>>(new _Node<T>());
		n->Elements[0] = item;
		return Vector<T>(new _Vector<T>(n));
	}

	template <typename T> T Get(Vector<T> vector, int key)
	{
		return vector->Get(key);
	}
}

#endif
