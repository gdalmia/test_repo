// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "Workflow/physicianrepository.h"
#include "physician.h"

namespace Philips {namespace Workflow {

	class PhysicianStore
	{
	public:
		PhysicianStore();
		bool write(const std::vector<PhysicianRepository::Physician>& physicians , const PhysicianRepository::Physician::Type & type) const;
		std::vector <PhysicianRepository::Physician> read(const PhysicianRepository::Physician::Type& type) const;
		
	private:
		void createDirectory(const std::filesystem::path & path) const;

		Physician::PersonName  toPhysicianName(const IPhysicianRepository::Physician & physician) const;
		IPhysicianRepository::Physician toPhysician(const Physician::PersonName & personName, const IPhysicianRepository::Physician::Type& type) const;
		std::map<PhysicianRepository::Physician::Type, std::filesystem::path> _filePath;

		std::filesystem::path	_configDir;
	};

}}