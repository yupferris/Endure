#ifndef __ENDURE_LIST_H__
#define __ENDURE_LIST_H__

#include <memory>

namespace Endure
{
	template <typename T> class _List;
	template <typename T> class _List
	{
	public:
		const T Head;
		const std::shared_ptr<_List<T>> Tail;
		const int Count;

		_List(T head)
			: Head(head), Tail(nullptr), Count(1)
		{
		}

		_List(T head, std::shared_ptr<_List<T>> tail)
			: Head(head), Tail(tail), Count(tail.get()->Count + 1)
		{
		}
	};

	template <typename T> using List = std::shared_ptr<_List<T>>;

	template <typename T> List<T> Cons(T head)
	{
		return std::make_shared<_List<T>>(_List<T>(head));
	}

	template <typename T> List<T> Cons(T head, List<T> tail)
	{
		return std::make_shared<_List<T>>(_List<T>(head, tail));
	}
}

#endif
