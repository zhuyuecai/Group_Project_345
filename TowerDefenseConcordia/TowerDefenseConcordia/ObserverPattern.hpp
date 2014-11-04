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
		// Subscribe to message type and call lambda function when receive one
		template <typename T>
		void subcribeToMessage(std::function<void(IMessage *message)> function)
		{
			if (_callbacks.size() <= T::getId())
			{
				_callbacks.resize(T::getId() + 1);
			}
			_callbacks[T::getId()] = function;
		}

		// Unsubscribe to message type
		template <typename T>
		void unsubcribeToMessage()
		{
			if (_callbacks.size() <= T::getId())
			{
				return;
			}
			_callbacks[T::getId()] = nullptr;
		}

		// Handle struct so that publisher can keep a reference on Subscriber and verify if they still exists
		struct Handle
		{
			Subscriber *ptr;
			bool valid;
			Handle(Subscriber *_ptr, bool _valid);
		};

		Subscriber();
		virtual ~Subscriber();
		inline const std::weak_ptr<Handle> getHandle() const
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

		// Used to publish a message
		template <class T, typename... Args>
		void publish(Args ...args)
		{
			T tmp(args...);
			for (unsigned int i = 0; i < _subscribers.size(); ++i)
			{
				auto &e = _subscribers[i];
				if (e.expired() || e.lock()->ptr == nullptr || !e.lock()->valid)
				{
					std::swap(_subscribers[i], _subscribers.back());
					_subscribers.pop_back();
					continue;
				}
				e.lock()->ptr->receive(&tmp);
			}
		}

		// Add a subscriber
		void addSubscriber(std::weak_ptr<Subscriber::Handle> handle);
		// Remove a subscriber
		void removeSubscriber(std::weak_ptr<Subscriber::Handle> handle);
		// Remove invalid subscribers
		void removeEmptySubscribers();
	private:
		std::vector<std::weak_ptr<Subscriber::Handle>> _subscribers;
	};

	// Class where the object is publisher and subscriber at the same time
	class PubSub : public Publisher, public Subscriber
	{};
}