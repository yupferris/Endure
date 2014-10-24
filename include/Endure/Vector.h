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

	template <typename T> class _Vector
	{
	public:
		_Vector()
			: root(nullptr), count(0)
		{
		}

		int Count() const
		{
			return count;
		}

		std::shared_ptr<_Vector<T>> Conj(T item)
		{
			auto n = new _Node<T>();
			for (int i = 0; i < count; i++)
				n->Elements[i] = root->Elements[i];
			n->Elements[count] = item;
			return std::shared_ptr<_Vector<T>>(new _Vector<T>(std::shared_ptr<_Node<T>>(n), count + 1));
		}

		T Get(int key)
		{
			return root->Elements[key];
		}

	private:
		_Vector(std::shared_ptr<_Node<T>> root)
			: root(root), count(1)
		{
		}

		_Vector(std::shared_ptr<_Node<T>> root, int count)
			: root(root), count(count)
		{
		}

		const std::shared_ptr<_Node<T>> root;
		const int count;
	};

	template <typename T> using Vector = std::shared_ptr<_Vector<T>>;

	template <typename T> Vector<T> CreateVector()
	{
		return std::shared_ptr<_Vector<T>>(new _Vector<T>());
	}

	template <typename T> Vector<T> Conj(Vector<T> vector, T item)
	{
		return vector->Conj(item);
	}

	template <typename T> T Get(Vector<T> vector, int key)
	{
		return vector->Get(key);
	}
}

#endif
