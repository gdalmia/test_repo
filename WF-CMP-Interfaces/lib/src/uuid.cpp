// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/uuid.h"
#include <Trace.h>
#include <Windows.h>

namespace Philips { namespace Workflow { 

	std::string Uuid::generate()
	{
		UUID uuid;
		if (UuidCreate(&uuid))
		{
			TraceError() << "Not able to create UUID.";
			throw std::runtime_error("Not able to create UUID.");
		}

		RPC_CSTR uuid_c_str{ nullptr };
		if (UuidToStringA(&uuid, static_cast<RPC_CSTR*>(&uuid_c_str)) != RPC_S_OK)
		{
			TraceError() << "Not able to convert UUID to string.";
			throw std::runtime_error("Not able to convert UUID to string.");
		}

		std::string uuidStr = reinterpret_cast<char*>(uuid_c_str);
		RpcStringFreeA(&uuid_c_str);

		return uuidStr;
	}

}}

