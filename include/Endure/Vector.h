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
			: Count(count), Elements(count > 0 ? new T[count] : nullptr)
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

		std::shared_ptr<_Vector<T>> Pop()
		{
			if (!count)
				throw std::logic_error("Cannot pop empty vector");

			if (count == 1)
				return std::shared_ptr<_Vector<T>>(new _Vector<T>());

			if (count - TailOffset() > 1)
			{
				auto tailNode = (_Node<T> *)tail.get();
				int tailCount = tailNode->Count - 1;
				auto elements = new T[tailCount];
				for (int i = 0; i < tailCount; i++)
					elements[i] = tailNode->Elements[i];
				return std::shared_ptr<_Vector<T>>(new _Vector(count - 1, shift, root, std::shared_ptr<void>(new _Node<T>(tailCount, elements))));
			}

			auto newTail = ArrayFor(count - 2);
			auto newRoot = PopTail(shift, root);
			int newShift = shift;
			if (!newRoot.get())
				newRoot = std::shared_ptr<void>(new _Node<std::shared_ptr<void>>(32));
			auto newRootValue = (_Node<std::shared_ptr<void>> *)newRoot.get();
			if (shift > 5 && !newRootValue->Elements[1])
			{
				newRoot = newRootValue->Elements[0];
				newShift -= 5;
			}
			return std::shared_ptr<_Vector<T>>(new _Vector(count - 1, newShift, newRoot, newTail));
		}

		T Get(int i) const
		{
			auto node = (_Node<T> *)ArrayFor(i).get();
			return node->Elements[i & 0x1f];
		}

	private:
		_Vector(int count, int shift, std::shared_ptr<void> root, std::shared_ptr<void> tail)
			: count(count), shift(shift), root(root), tail(tail)
		{
		}

		std::shared_ptr<void> ArrayFor(int i) const
		{
			if (i < 0 || i >= count)
				throw std::out_of_range("Index was out of bounds");

			if (i >= TailOffset())
				return tail;

			auto node = root;
			for (int level = shift; level > 0; level -= 5)
				node = ((_Node<std::shared_ptr<void>> *)node.get())->Elements[(i >> level) & 0x1f];
			return node;
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

		std::shared_ptr<void> PopTail(int level, std::shared_ptr<void> node)
		{
			int localIndex = ((count - 2) >> level) & 0x1f;
			if (level > 5)
			{
				auto newChild = PopTail(level - 5, ((_Node<std::shared_ptr<void>> *)node.get())->Elements[localIndex]);
				if (!newChild.get() && localIndex == 0)
					return std::shared_ptr<void>();

				auto internalNode = (_Node<std::shared_ptr<void>> *)node.get();
				int internalNodeCount = internalNode->Count;
				auto elements = new std::shared_ptr<void>[internalNodeCount];
				for (int i = 0; i < internalNodeCount; i++)
					elements[i] = internalNode->Elements[i];
				elements[localIndex] = newChild;
				return std::shared_ptr<void>(new _Node<std::shared_ptr<void>>(internalNodeCount, elements));
			}

			if (localIndex == 0)
				return std::shared_ptr<void>();

			auto internalNode = (_Node<std::shared_ptr<void>> *)node.get();
			int internalNodeCount = internalNode->Count;
			auto elements = new std::shared_ptr<void>[internalNodeCount];
			for (int i = 0; i < internalNodeCount; i++)
				elements[i] = internalNode->Elements[i];
			elements[localIndex] = nullptr;
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

	template <typename T> Vector<T> Pop(Vector<T> vector)
	{
		return vector->Pop();
	}

	template <typename T> T Get(Vector<T> vector, int key)
	{
		return vector->Get(key);
	}
}

#endif
