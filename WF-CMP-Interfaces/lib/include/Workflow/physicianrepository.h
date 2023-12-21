// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "iphysicianrepository.h"

namespace Philips {namespace Workflow {

	class PhysicianRepository : public IPhysicianRepository
	{
	public:
		PhysicianRepository() = default;
		virtual ~PhysicianRepository() = default;
		
		bool add(const Physician& physician) override;
		std::vector<Physician> physicians(const Physician::Type& type) override;

	private:
		std::string generateUniqueID() const;
		PhysicianRepository::Physician createPhysician(const Physician::Type & physicianType , const Workflow::PersonName& name) const;
	};

}}