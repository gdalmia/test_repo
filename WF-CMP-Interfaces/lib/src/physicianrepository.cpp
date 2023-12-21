// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/physicianrepository.h"
#include "physicianstore.h"
#include <Trace.h>
#include <Windows.h>

namespace Philips {namespace Workflow {
		
	Philips::Workflow::IPhysicianRepository::Physician::Physician(const std::string & key) : Model(key)
	{
	}

	bool Philips::Workflow::IPhysicianRepository::Physician::operator==(const Physician &other) const
	{
		return !(*this != other);
	}
	
	bool Philips::Workflow::IPhysicianRepository::Physician::operator!=(const Physician & other) const
	{
		
		return (this->name != other.name);
	}

	std::string PhysicianRepository::generateUniqueID() const
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
	
	PhysicianRepository::Physician PhysicianRepository::createPhysician(const Physician::Type & physicianType, const Workflow::PersonName& name) const
	{
		PhysicianRepository::Physician physician(generateUniqueID());
		physician.name = name;
		physician.type = physicianType;
		return physician;
	}
	
	bool PhysicianRepository::add(const Physician & physician)
	{
		Physician physicianToAdd = physician;
		std::vector <PhysicianRepository::Physician> Physicians;
		auto allPhysicians = physicians(physicianToAdd.type);
		auto newPhysicianLowerCase = Workflow::convert(physicianToAdd.name.toString());

		std::transform(newPhysicianLowerCase.begin(), newPhysicianLowerCase.end(), newPhysicianLowerCase.begin(), ::towlower);

		auto it = std::find_if(allPhysicians.begin(), allPhysicians.end(), [newPhysicianLowerCase](auto &physician) {
			auto physicianLowerCase = Workflow::convert(physician.name.toString());
			std::transform(physicianLowerCase.begin(), physicianLowerCase.end(), physicianLowerCase.begin(), ::towlower);
			return newPhysicianLowerCase == physicianLowerCase;
		});

		if (it != allPhysicians.end())
		{
			physicianToAdd = *it;
			physicianToAdd.name = physician.name;
		}

		Physicians.emplace_back(static_cast<std::string>(physicianToAdd).empty() ? createPhysician(physicianToAdd.type, physicianToAdd.name) : physicianToAdd);
		for (const auto singlePhysican : allPhysicians)
			if (static_cast<std::string>(singlePhysican) != static_cast<std::string>(physicianToAdd))
				Physicians.emplace_back(singlePhysican);

		const PhysicianStore store;
		if (Physicians.size() > 20)
			Physicians.erase(Physicians.begin() + 20);
		return store.write(Physicians, physicianToAdd.type);
	}
	
	std::vector<PhysicianRepository::Physician> PhysicianRepository::physicians(const Physician::Type & type)
	{
		PhysicianStore store;
		return store.read(type);
	}
}}