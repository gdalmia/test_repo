// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <workflow.h>

namespace Philips {namespace Workflow {

	//! Represents a Physician repository in workflow
	class IPhysicianRepository : protected IRepository
	{
	public:
		class Physician : public Model
		{
		public:
			enum class Type { Referring, Performing, Operating };

			Physician() = default;
			explicit Physician(const std::string & key);
			virtual ~Physician() = default;

			bool operator ==(const Physician& other) const;
			bool operator !=(const Physician& other) const;
			
			Workflow::PersonName name;
			Type type;
		};

		//! Add a new Physician to the repository
		virtual bool add(const Physician& physician) = 0;
		
		//! Reads Physicians from  repository
		virtual std::vector<Physician> physicians(const Physician::Type& type) = 0;
	};
}}