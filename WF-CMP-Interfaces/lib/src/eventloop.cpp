// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/eventloop.h"
#include <windows.h>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace
{
	enum Messages { WM_DCM_POST = WM_USER };

	const WNDCLASSEXW & registerWindowClass(WNDPROC wndProc)
	{
		static const struct WindowClass : WNDCLASSEXW
		{
			WindowClass(WNDPROC wndProc)
			{
				memset(static_cast<WNDCLASSEXW *>(this), 0, sizeof(WNDCLASSEXW));
				cbSize = sizeof(WNDCLASSEXW);
				lpfnWndProc = wndProc;
				hInstance = reinterpret_cast<HINSTANCE>(&__ImageBase);
				lpszClassName = L"Workflow::EventLoop";
				RegisterClassExW(this);
			}

			~WindowClass()
			{
				UnregisterClassW(
					lpszClassName,
					reinterpret_cast<HINSTANCE>(&__ImageBase));
			}
		} windowClass(wndProc);

		return windowClass;
	}
}

namespace Philips { namespace Workflow {

	class EventLoop::Singleton
	{
		friend class EventLoop;
	public:
		static std::shared_ptr<EventLoop::Singleton> instance()
		{
			static std::weak_ptr<EventLoop::Singleton> singleton;

			auto result = singleton.lock();
			if (result) return result;

			std::lock_guard<std::mutex> guard(mutex);

			while (constructed) //Wait for pending destructors on different threads
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
			result.reset(new EventLoop::Singleton());
			singleton = result;

			return result;
		}

		~Singleton()
		{
			std::lock_guard<std::mutex> guard(mutex);
			if (window != nullptr)
			{
				DestroyWindow(window);
				window = nullptr;
			}
			constructed = false;
		}

		void post(const std::function<void(void)> &callback)
		{
			std::lock_guard<std::mutex> guard(mutex);
			if (!stopped)
			{
				actions.emplace(callback);
				PostMessage(window, WM_DCM_POST, 0, 0);
			}
		}

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

	private:
		Singleton()
			: actions()
			, window(CreateWindowExW(
				0, registerWindowClass(&Singleton::WndProc).lpszClassName, NULL,
				0, 0, 0, 0, 0,
				HWND_MESSAGE, NULL, reinterpret_cast<HINSTANCE>(&__ImageBase),
				this))
			, stopped(false)
		{
			constructed = true;
		}

		void executeNextCallback()
		{
			std::function<void()> callback{ nullptr };
			{
				std::lock_guard<std::mutex> guard(mutex);
				if (!stopped && !actions.empty())
				{
					callback = actions.front();
					actions.pop();
				}
			}
			if (callback) callback();
		}

		static LRESULT WINAPI WndProc(HWND window, UINT msg, WPARAM wp, LPARAM lp)
		{
			if (msg != WM_CREATE)
			{
				switch (msg)
				{
				case WM_DCM_POST:
					if (const auto windowPtr = reinterpret_cast<Singleton*>(::GetWindowLongPtr(window, GWLP_USERDATA)))
					{
						windowPtr->executeNextCallback();
					}
					break;
				default:
					break;
				}
			}
			else { ::SetWindowLongPtr(window, GWLP_USERDATA, LONG_PTR(reinterpret_cast<LPCREATESTRUCT>(lp)->lpCreateParams)); }

			return ::DefWindowProc(window, msg, wp, lp);
		}

	private:
		std::queue<std::function<void()>>	actions;
		HWND								window;
		bool								stopped;
		static std::mutex					mutex;
		static bool							constructed;
	};

	bool		EventLoop::Singleton::constructed;
	std::mutex	EventLoop::Singleton::mutex;

	EventLoop::EventLoop()
		: instance(Singleton::instance())
	{
	}

	void EventLoop::post(const std::function<void(void)>& action)
	{
		instance->post(action);
	}

}}