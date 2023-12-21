// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/Media/usbmonitor.h"
#include "devicewatcher.h"
#include "Workflow/workloop.h"
#include "Workflow/eventloop.h"
#include <condition_variable>
#include <Trace.h>
#include <Log.h>

namespace Philips { namespace Workflow { namespace Media { 

	const Log::Event USBDetected = Log::Event(39, "DHI", Log::Category::UtilizationUserMessage, L"USB detected");

	struct UsbMonitor::Private
	{
		Private();

		bool started;
		std::condition_variable signal;
		std::mutex mutex;

		std::vector <std::unique_ptr<IUsb>> usbs;
		std::unique_ptr<DeviceWatcher> watcher;
		std::unique_ptr<WorkLoop> worker;
		EventLoop eventLoop;
	};

	UsbMonitor::Private::Private() : started(false)
	{
	}

	UsbMonitor::UsbMonitor():_private(std::make_unique<Private>())
	{
		_private->worker = std::make_unique<WorkLoop>("media-monitor-thread");

			_private->usbs = DeviceWatcher::addExistingNodes();
			_private->worker->post([this]
			{
				_private->watcher = std::make_unique<DeviceWatcher>();
				subscribe();

				//start() runs the message loop. so signal started before start
				{
					std::lock_guard<std::mutex> lock(_private->mutex);
					_private->started = true;
				}
				_private->signal.notify_one();

				_private->watcher->start();
			});

			//wait for watcher to start
			std::unique_lock<std::mutex> lock(_private->mutex);
			_private->signal.wait(lock, [this] { return _private->started; });
	}

	UsbMonitor::~UsbMonitor()
	{
		if (_private->watcher)
		{
			_private->watcher->added = nullptr;
			_private->watcher->removed = nullptr;
			_private->watcher->stop();
		}
	}

	const std::vector<std::unique_ptr<IUsb>>& UsbMonitor::usbs() const
	{
		return _private->usbs;
	}

	void UsbMonitor::subscribe()
	{
		_private->watcher->added = [this](const Usb& usb)
		{
			_private->eventLoop.post([this, usb]
			{
				TraceMessage() << L"Usb added : " << usb.drive();
				LogMessage(USBDetected)<<"Name = ["<< usb.volumeName()<<"] Configuration = [USB configuration enabled]";
				_private->usbs.push_back(std::make_unique<Usb>(usb));
				IUsb& newUsb = *_private->usbs[_private->usbs.size() - 1];
				usbAdded(newUsb);
			});
		};

		_private->watcher->removed = [this](const std::string& drive)
		{
			_private->eventLoop.post([this, drive]
			{
				TraceMessage() << L"Usb removed : " << drive;
				const auto iterator = find_if(_private->usbs.begin(), _private->usbs.end(),
					[drive](const std::unique_ptr<IUsb>& usb) { return usb->drive() == drive; });

				if (iterator != _private->usbs.end())
				{
					const auto node = std::move(*iterator);
					_private->usbs.erase(iterator);
					usbRemoved(*node);
				}
			});
		};
	}
	
}}}