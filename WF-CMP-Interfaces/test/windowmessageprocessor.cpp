// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "windowmessageprocessor.h"
#include <Trace.h>

#include <thread>
#include <cassert>
#include <Windows.h>

namespace Philips { namespace Workflow   {

	namespace WindowMessageProcessor
	{
		void processMessage()
		{
			MSG msg = { 0 };
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					PostQuitMessage(int(msg.wParam));
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		bool processMessageWhile(std::function<bool()> condition, int timeoutInMs)
		{
			TraceScope();

			assert(condition != nullptr);
			auto count{ 0 };
			while (condition())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				processMessage();
				if (++count == timeoutInMs)
				{
					TraceWarning() << "Process message timed out after " << std::to_string(timeoutInMs) << " ms.";
					return false;
				}
			}
			return true;
		}
	}

}}
