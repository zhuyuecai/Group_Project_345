#pragma once

#include <cstddef>

namespace TDC
{
	namespace __privateDetails
	{
		static std::size_t __getId()
		{
			static std::size_t id = 0;
			return ++id;
		}
	}

	struct IMessage
	{
		IMessage()
			: _id(-1)
		{
		}
		virtual ~IMessage(){}
		std::size_t getTypeId() const
		{
			return _id;
		}
	protected:
		std::size_t _id;
		static std::size_t __getid()
		{
			static std::size_t __id = 0;
			return __id++;
		}
	};

	template <class T>
	struct Message : public IMessage
	{
		Message()
		{
			_id = getId();
		}
		virtual ~Message(){}

		static std::size_t getId()
		{
			static std::size_t id = __getid();
			return id;
		}
	};

	struct MessageTest : public Message<MessageTest>
	{
		int value;
		MessageTest(int v = 42) : value(v) {}
		virtual ~MessageTest(){}
	};
}