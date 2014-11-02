#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "Message.hpp"

namespace TDC
{
	class Publisher;

	class Subscriber
	{
	public:
		template <typename T>
		void subcribeToMessage(std::function<void(IMessage *message)> function)
		{
			if (_callbacks.size() <= T::getId())
			{
				_callbacks.resize(T::getId() + 1);
			}
			_callbacks[T::getId()] = function;
		}

		template <typename T>
		void unsubcribeToMessage()
		{
			if (_callbacks.size() <= T::getId())
			{
				return;
			}
			_callbacks[T::getId()] = nullptr;
		}

		struct Handle
		{
			Subscriber *ptr;
			bool valid;
			Handle(Subscriber *_ptr, bool _valid);
		};

		Subscriber();
		virtual ~Subscriber();
		inline const std::shared_ptr<Handle> getHandle() const
		{
			return _handle;
		}
	private:
		bool receive(IMessage *message);

		std::vector<std::function<void(IMessage *message)>> _callbacks;
		std::shared_ptr<Handle> _handle;

		friend class Publisher;
	};

	class Publisher
	{
	public:
		Publisher();
		virtual ~Publisher();

		template <class T, typename... Args>
		void publish(Args ...args)
		{
			T tmp(args...);
			for (unsigned int i = 0; i < _subscribers.size(); ++i)
			{
				auto &e = _subscribers[i];
				e->ptr->receive(&tmp);
			}
		}

		void addSubscriber(std::shared_ptr<Subscriber::Handle> handle);
		void removeSubscriber(std::shared_ptr<Subscriber::Handle> handle);
		void removeEmptySubscribers();
	private:
		std::vector<std::shared_ptr<Subscriber::Handle>> _subscribers;
	};

	class PubSub : public Publisher, public Subscriber
	{};
}