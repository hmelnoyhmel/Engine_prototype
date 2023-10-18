#pragma once
#include <vector>
#include <functional>

template <typename T>
class Event
{
public:
	static void Subscribe(std::function<void(T)> delegate)
	{
		delegates.push_back(delegate);
	}

	static void Raise(T message)
	{
		for (auto& delegate : delegates)
			delegate(message);
	}

private:
	inline static std::vector<std::function<void(T)>> delegates;
};

struct ScreenResizeMessage
{
	int newWidth;
	int newHeight;
};