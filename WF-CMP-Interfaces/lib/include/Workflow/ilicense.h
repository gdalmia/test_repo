// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include <string>
namespace Philips { namespace Workflow {

	class ILicense
	{
	public:
		ILicense()							 = default;
		virtual ~ILicense()					 = default;
		
		//! Gets feature available status
		virtual bool available(const std::string& featureName) const = 0;
	};
}}