// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once
#include "datetime.h"
#include <Sense.h>

//TICS -INT#002: Domain model needs data member in public

namespace Philips { namespace Workflow {

	//! Represents a pictorial in workflow
	class Pictorial
	{
	public:
		Pictorial();										//!< Creates a pictorial

		Sense::Image	image;								//!< The thumbnail image for the pictorial
		std::string		seriesNumber;						//!< Series number of the pictorial
		std::string		title;								//!< Title to be displayed for the pictorial
		DateTime		creationTime;						//!< Creation time of the series represented by the pictorial
		Sense::Image	icon;								//!< Icon for the pictorial
		std::string		attribute;							//!< Attribute for the pictorial
		std::string		tag;								//!< Tag for the pictorial
		bool			favorite;							//!< Denotes whether the pictorial is a favorite
	};
}}