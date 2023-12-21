// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#include "Workflow/irepository.h"

namespace Philips { namespace Workflow {

	IRepository::Model::Model(std::string key) :_key(std::move(key))
	{
	}

	IRepository::Model::operator std::string() const
	{
		return _key;
	}

	IRepository::Model::Model(Model&& model) noexcept: _key(std::move(model._key))
	{
	}

    bool IRepository::Model::operator==(const Model& other) const
    {
        return !(*this != other);
    }

    bool IRepository::Model::operator!=(const Model& other) const
    {
        return _key != other._key;
    }

	void IRepository::setKey(Model& model, const std::string& key)
	{
		model._key = key;
	}
}}