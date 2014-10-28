#ifndef __ENDURE_VECTOR_H__
#define __ENDURE_VECTOR_H__

#include <exception>
#include <memory>

namespace Endure
{
	template <typename T> class _Node
	{
	public:
		const int Count;
		T *Elements;

		_Node(int count, T *elements)
			: Count(count), Elements(elements)
		{
		}

		_Node(int count)
			: Count(count), Elements(new T[count])
		{
		}

		_Node()
			: Count(0), Elements(nullptr)
		{
		}

		~_Node()
		{
			if (Elements)
				delete [] Elements;
		}
	};

	template <typename T> class _Vector
	{
	public:
		_Vector()
			: count(0), shift(5), root(new _Node<std::shared_ptr<void>>(32)), tail(new _Node<T>())
		{
		}

		int Count() const
		{
			return count;
		}

		std::shared_ptr<_Vector<T>> Conj(T item) const
		{
			if (count - TailOffset() < 32)
			{
				auto tailNode = (_Node<T> *)tail.get();
				int tailCount = tailNode->Count + 1;
				auto elements = new T[tailCount];
				for (int i = 0; i < tailNode->Count; i++)
					elements[i] = tailNode->Elements[i];
				elements[tailNode->Count] = item;
				return std::shared_ptr<_Vector<T>>(new _Vector(count + 1, shift, root, std::shared_ptr<void>(new _Node<T>(tailCount, elements))));
			}

			std::shared_ptr<void> newRoot;
			int newShift = shift;

			if ((count >> 5) > (1 << shift))
			{
				auto n = new _Node<std::shared_ptr<void>>(32);
				n->Elements[0] = root;
				n->Elements[1] = NewPath(shift, tail);
				newRoot = std::shared_ptr<void>(n);
				newShift += 5;
			}
			else
			{
				newRoot = PushTail(shift, (_Node<std::shared_ptr<void>> *)root.get(), tail);
			}

			auto newTailElements = new T[1];
			newTailElements[0] = item;
			return std::shared_ptr<_Vector<T>>(new _Vector(count + 1, newShift, newRoot, std::shared_ptr<void>(new _Node<T>(1, newTailElements))));
		}

		std::shared_ptr<_Vector<T>> Assoc(int i, T value)
		{
			if (i < 0 || i > count)
				throw std::out_of_range("Index was out of bounds");

			if (i == count)
				return Conj(value);

			if (i >= TailOffset())
			{
				auto tailNode = (_Node<T> *)tail.get();
				int tailCount = tailNode->Count;
				auto elements = new T[tailCount];
				for (int j = 0; j < tailCount; j++)
					elements[j] = tailNode->Elements[j];
				elements[i & 0x1f] = value;
				return std::shared_ptr<_Vector<T>>(new _Vector(count, shift, root, std::shared_ptr<void>(new _Node<T>(tailCount, elements))));
			}

			return std::shared_ptr<_Vector<T>>(new _Vector(count, shift, AssocAux(shift, root, i, value), tail));
		}

		T Get(int i) const
		{
			auto node = ArrayFor(i);
			return node[i & 0x1f];
		}

	private:
		_Vector(int count, int shift, std::shared_ptr<void> root, std::shared_ptr<void> tail)
			: count(count), shift(shift), root(root), tail(tail)
		{
		}

		const T *ArrayFor(int i) const
		{
			if (i < 0 || i >= count)
				throw std::out_of_range("Index was out of bounds");

			if (i >= TailOffset())
				return ((_Node<T> *)tail.get())->Elements;

			auto node = root.get();
			for (int level = shift; level > 0; level -= 5)
				node = ((_Node<std::shared_ptr<void>> *)node)->Elements[(i >> level) & 0x1f].get();
			return ((_Node<T> *)node)->Elements;
		}

		int TailOffset() const
		{
			if (count < 32)
				return 0;

			return ((count - 1) >> 5) << 5;
		}

		std::shared_ptr<void> PushTail(int level, const _Node<std::shared_ptr<void>> *parent, std::shared_ptr<void> tail) const
		{
			int i = ((count - 1) >> level) & 0x1f;
			auto elements = new std::shared_ptr<void>[parent->Count];
			for (int i = 0; i < parent->Count; i++)
				elements[i] = parent->Elements[i];
			auto ret = new _Node<std::shared_ptr<void>>(parent->Count, elements);
			std::shared_ptr<void> nodeToInsert;

			if (level == 5)
			{
				nodeToInsert = tail;
			}
			else
			{
				auto child = parent->Elements[i];
				nodeToInsert = child.get() ?
					PushTail(level - 5, (_Node<std::shared_ptr<void>> *)child.get(), tail) :
					NewPath(level - 5, tail);
			}

			ret->Elements[i] = nodeToInsert;
			return std::shared_ptr<void>(ret);
		}

		static std::shared_ptr<void> NewPath(int level, std::shared_ptr<void> node)
		{
			if (level == 0)
				return node;

			auto ret = new _Node<std::shared_ptr<void>>(32);
			ret->Elements[0] = NewPath(level - 5, node);
			return std::shared_ptr<void>(ret);
		}

		static std::shared_ptr<void> AssocAux(int level, std::shared_ptr<void> node, int i, T value)
		{
			if (level == 0)
			{
				auto leafNode = (_Node<T> *)node.get();
				int leafCount = leafNode->Count;
				auto elements = new T[leafCount];
				for (int j = 0; j < leafCount; j++)
					elements[j] = leafNode->Elements[j];
				elements[i & 0x1f] = value;
				return std::shared_ptr<void>(new _Node<T>(leafCount, elements));
			}

			auto internalNode = (_Node<std::shared_ptr<void>> *)node.get();
			int internalNodeCount = internalNode->Count;
			auto elements = new std::shared_ptr<void>[internalNodeCount];
			for (int j = 0; j < internalNodeCount; j++)
				elements[j] = internalNode->Elements[j];
			int localIndex = (i >> level) & 0x1f;
			elements[localIndex] =
				AssocAux(level - 5, elements[localIndex], i, value);
			return std::shared_ptr<void>(new _Node<std::shared_ptr<void>>(internalNodeCount, elements));
		}

		const int count;
		const int shift;
		const std::shared_ptr<void> root;
		const std::shared_ptr<void> tail;
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

	template <typename T> Vector<T> Assoc(Vector<T> vector, int i, T value)
	{
		return vector->Assoc(i, value);
	}

	template <typename T> T Get(Vector<T> vector, int key)
	{
		return vector->Get(key);
	}
}

#endif
