// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "Workflow/Media/usb.h"
#include <memory>
#include <functional>
#include <vector>

namespace Philips { namespace Workflow { namespace Media {  

	class DeviceWatcher
	{
	public:
		DeviceWatcher();
		~DeviceWatcher();
		
		void stop() const;
		void start() const;

		static std::vector<std::unique_ptr<IUsb>> addExistingNodes();

		std::function<void(const Usb&)> added;
		std::function<void(const std::string&)> removed;

	private:
		class Private;
		std::unique_ptr<Private> _private;
	};
}}}


