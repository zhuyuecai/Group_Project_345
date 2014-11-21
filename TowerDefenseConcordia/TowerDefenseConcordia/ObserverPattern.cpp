#include "ObserverPattern.hpp"

namespace TDC
{
	Subscriber::Handle::Handle(Subscriber *_ptr, bool _valid)
		: ptr(_ptr)
		, valid(_valid)
	{}

	Subscriber::Subscriber()
	{
		_handle = std::make_shared<Handle>(this, true);
	}

	Subscriber::~Subscriber()
	{
		_handle->valid = false;
	}

	bool Subscriber::receive(IMessage *message)
	{
		if (message->getTypeId() >= _callbacks.size())
			return false;
		if (!_callbacks[message->getTypeId()])
			return false;
		_callbacks[message->getTypeId()](message);
		return true;
	}

	Publisher::Publisher()
	{}

	Publisher::~Publisher()
	{}

	void Publisher::addSubscriber(std::weak_ptr<Subscriber::Handle> handle)
	{
		if (!handle.lock())
			return;
		removeEmptySubscribers();
		for (auto &e : _subscribers)
		{
			if (e.lock()->ptr == handle.lock()->ptr)
				return;
		}
		_subscribers.push_back(handle);
	}

	void Publisher::removeSubscriber(std::weak_ptr<Subscriber::Handle> handle)
	{
		for (std::size_t i = 0; i < _subscribers.size(); ++i)
		{
			if (_subscribers[i].lock()->ptr == handle.lock()->ptr)
			{
				std::swap(_subscribers[i], _subscribers.back());
				_subscribers.pop_back();
				return;
			}
		}
	}

	void Publisher::removeEmptySubscribers()
	{
		for (std::size_t i = 0; i < _subscribers.size(); ++i)
		{
			if (_subscribers[i].expired() || !_subscribers[i].lock() || !_subscribers[i].lock()->ptr || !_subscribers[i].lock()->valid)
			{
				std::swap(_subscribers[i], _subscribers.back());
				_subscribers.pop_back();
				return;
			}
		}
	}
}