// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "physicianstore.h"
#include <Trace.h>
#include <fstream>

namespace Philips {namespace Workflow {

	const std::filesystem::path configFolderPath	= "Physicians";
	const std::string performingFileName			= "performing.json";
	const std::string operatingFileName				= "operating.json";
	const std::string referringFileName				= "referring.json";
	
	PhysicianStore::PhysicianStore()
	{
		const Settings settings;
		_configDir = settings.path() / configFolderPath;
		createDirectory(_configDir);

		_filePath.insert(std::pair< PhysicianRepository::Physician::Type, std::filesystem::path>(PhysicianRepository::Physician::Type::Operating, _configDir / operatingFileName));
		_filePath.insert(std::pair< PhysicianRepository::Physician::Type, std::filesystem::path>(PhysicianRepository::Physician::Type::Performing, _configDir / performingFileName));
		_filePath.insert(std::pair< PhysicianRepository::Physician::Type, std::filesystem::path>(PhysicianRepository::Physician::Type::Referring, _configDir / referringFileName));
	}

	void PhysicianStore::createDirectory(const std::filesystem::path & path) const
	{
		if (!std::filesystem::exists(path))
		{
			std::error_code error;
			if (!std::filesystem::create_directories(path, error))
			{
				TraceError() << "Not able to create directory. Path: " << path << ". Error code: " << error.message();
				throw std::runtime_error("Failed to create directory." + error.message());
			}
		}
	}
	
	Physician::PersonName PhysicianStore::toPhysicianName(const IPhysicianRepository::Physician & physician) const
	{
		Physician::PersonName personName;
		if (!physician.name.components.empty()) {
			personName.alphabetic.familyName = physician.name.components.at(PersonName::Group::Alphabetic).familyName;
			personName.alphabetic.givenName = physician.name.components.at(PersonName::Group::Alphabetic).givenName;
			personName.alphabetic.middleName = physician.name.components.at(PersonName::Group::Alphabetic).middleName;
			personName.alphabetic.namePrefix = physician.name.components.at(PersonName::Group::Alphabetic).namePrefix;
			personName.alphabetic.nameSuffix = physician.name.components.at(PersonName::Group::Alphabetic).nameSuffix;
		}

		if (physician.name.components.find(PersonName::Group::Ideographic) != physician.name.components.end()) {
			personName.ideographic.familyName = physician.name.components.at(PersonName::Group::Ideographic).familyName;
			personName.ideographic.givenName = physician.name.components.at(PersonName::Group::Ideographic).givenName;
			personName.ideographic.middleName = physician.name.components.at(PersonName::Group::Ideographic).middleName;
			personName.ideographic.namePrefix = physician.name.components.at(PersonName::Group::Ideographic).namePrefix;
			personName.ideographic.nameSuffix = physician.name.components.at(PersonName::Group::Ideographic).nameSuffix;
		}

		if (physician.name.components.find(PersonName::Group::Phonetic) != physician.name.components.end()) {
			personName.phonetic.familyName = physician.name.components.at(PersonName::Group::Phonetic).familyName;
			personName.phonetic.givenName = physician.name.components.at(PersonName::Group::Phonetic).givenName;
			personName.phonetic.middleName = physician.name.components.at(PersonName::Group::Phonetic).middleName;
			personName.phonetic.namePrefix = physician.name.components.at(PersonName::Group::Phonetic).namePrefix;
			personName.phonetic.nameSuffix = physician.name.components.at(PersonName::Group::Phonetic).nameSuffix;
		}
		personName.id = static_cast<std::string>(physician);
		return personName;
	}

	IPhysicianRepository::Physician PhysicianStore::toPhysician(const Physician::PersonName & personName, const IPhysicianRepository::Physician::Type& type) const
	{
		IPhysicianRepository::Physician physician(personName.id);
		physician.type = type;
		physician.name.components.insert({ PersonName::Group::Alphabetic, PersonName::Component{personName.alphabetic.familyName,
		 personName.alphabetic.givenName,  personName.alphabetic.middleName,  personName.alphabetic.namePrefix,  personName.alphabetic.nameSuffix} });
		
		physician.name.components.insert({ PersonName::Group::Ideographic, PersonName::Component{personName.ideographic.familyName,
		 personName.ideographic.givenName,  personName.ideographic.middleName,  personName.ideographic.namePrefix,  personName.ideographic.nameSuffix} });
		
		physician.name.components.insert({ PersonName::Group::Phonetic, PersonName::Component{personName.phonetic.familyName,
		 personName.phonetic.givenName,  personName.phonetic.middleName,  personName.phonetic.namePrefix,  personName.phonetic.nameSuffix} });
		return physician;
	}
	
	bool PhysicianStore::write(const std::vector<PhysicianRepository::Physician>& physicians , const PhysicianRepository::Physician::Type & type) const
	{
		std::filesystem::path filePath = _filePath.at(type);;
		Physician localPhys;
		for (const auto physician : physicians)
		{
			localPhys.name.emplace_back(toPhysicianName(physician));
		}
		
		std::ofstream nodeFile(filePath, std::ios::binary);
		if (!nodeFile)
		{
			TraceError() << L"Failed to write file. File path : " << filePath;
			return false;
		}
		
		std::vector <Physician> seralizePhysicans;
		seralizePhysicans.emplace_back(localPhys);
		nodeFile << seralizePhysicans.at(seralizePhysicans.size() - 1);
		nodeFile.flush();
		nodeFile.close();
		return true;
	}

	std::vector <PhysicianRepository::Physician> PhysicianStore::read(const PhysicianRepository::Physician::Type & type) const
	{
		Physician physicians;
		auto filePath = _filePath.at(type);
		std::ifstream ifstr(filePath);
		if (!ifstr)
		{
			TraceError() << L"Failed to read file. File path : " << filePath;
			return std::vector<PhysicianRepository::Physician>();
		}
		ifstr >> physicians;
		
		std::vector<PhysicianRepository::Physician> repositoryPhysicians;
		for (const auto &physician : physicians.name)
		{
			repositoryPhysicians.emplace_back(toPhysician(physician, type));
		}
		return repositoryPhysicians;
	}

	
}}