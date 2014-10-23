#ifndef __ENDURE_LIST_H__
#define __ENDURE_LIST_H__

#include <exception>
#include <memory>

namespace Endure
{
	template <typename T> class _List;
	template <typename T> class _List
	{
	public:
		virtual ~_List()
		{
		}

		virtual T Head() const = 0;
		virtual std::shared_ptr<_List<T>> Tail() const = 0;
		virtual int Count() const = 0;
	};

	template <typename T> class _EmptyList : public _List<T>
	{
	public:
		virtual ~_EmptyList()
		{
		}

		virtual T Head() const
		{
			throw std::logic_error("Cannot get Head of an empty list");
		}

		virtual std::shared_ptr<_List<T>> Tail() const
		{
			throw std::logic_error("Cannot get Tail of an empty list");
		}

		virtual int Count() const
		{
			return 0;
		}
	};

	template <typename T> class _Cons : public _List<T>
	{
	public:
		_Cons(T head)
			: head(head), tail(std::shared_ptr<_List<T>>(new _EmptyList<T>())), count(1)
		{
		}

		_Cons(T head, std::shared_ptr<_List<T>> tail)
			: head(head), tail(tail), count(tail.get()->Count() + 1)
		{
		}

		virtual ~_Cons()
		{
		}

		virtual T Head() const
		{
			return head;
		}

		virtual std::shared_ptr<_List<T>> Tail() const
		{
			return tail;
		}

		virtual int Count() const
		{
			return count;
		}

	private:
		const T head;
		const std::shared_ptr<_List<T>> tail;
		const int count;
	};

	template <typename T> using List = std::shared_ptr<_List<T>>;

	template <typename T> List<T> CreateList()
	{
		return List<T>(new _EmptyList<T>());
	}

	template <typename T> List<T> CreateList(T head)
	{
		return List<T>(new _Cons<T>(head));
	}

	template <typename T> List<T> Cons(T head, List<T> tail)
	{
		return List<T>(new _Cons<T>(head, tail));
	}
}

#endif
