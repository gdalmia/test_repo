// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <vector>

namespace Philips { namespace Workflow {
	
	//! Represents a repository in workflow
	class IRepository
	{
	public:
		//! Represents a repository model in workflow
		class Model
		{
			friend class IRepository;
			
		public:
			Model() = default;
			explicit Model(std::string key);
			virtual ~Model() = default;

			explicit operator std::string() const;

			Model(const Model&) = default;
			Model(Model&& model) noexcept;
			Model& operator=(const Model&) = default;
			Model& operator=(Model&& source) = default;

            bool operator ==(const Model& other) const;
            bool operator !=(const Model& other) const;
			
		private:
			std::string _key;
		};
		
		IRepository() = default;
		virtual ~IRepository() = default;

		IRepository(const IRepository&) = delete;
		IRepository(const IRepository&&) = delete;
		IRepository& operator=(const IRepository&) = delete;
		IRepository& operator=(IRepository&& source) = delete;
		
	protected:
		static void setKey(Model& model, const std::string& key);
	};

}}
