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

	void Publisher::addSubscriber(std::shared_ptr<Subscriber::Handle> handle)
	{
		for (auto &e : _subscribers)
		{
			if (e->ptr == handle->ptr)
				return;
		}
		_subscribers.push_back(handle);
	}

	void Publisher::removeSubscriber(std::shared_ptr<Subscriber::Handle> handle)
	{
		for (std::size_t i = 0; i < _subscribers.size(); ++i)
		{
			if (_subscribers[i]->ptr == handle->ptr)
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
			if (!_subscribers[i]->valid)
			{
				std::swap(_subscribers[i], _subscribers.back());
				_subscribers.pop_back();
				return;
			}
		}
	}
}