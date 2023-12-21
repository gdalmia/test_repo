// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "datetime.h"
#include <Sense.h>
#include <string>
#include <vector>

namespace Philips { namespace Workflow {

	//! Represents a preview run in workflow
	class IPreviewRun
	{
	public:
		//! Represents a preview meta data in workflow
		class Meta
		{
		public:

			//! Represents an attribute for preview
			struct Attribute
			{
				bool operator==(const Attribute& other) const
				{
					return !(*this != other);
				}
				
				bool operator!=(const Attribute& other) const
				{
					return !(this->key == other.key && this->value == other.value);
				}
				
				std::string key  ;			//!< The attribute key
				std::string value;			//!< The attribute value
			};

			//! Represents a bookmark in preview
			struct Bookmark
			{
				bool operator==(const Bookmark& other) const
				{
					return !(*this != other);
				}

				bool operator!=(const Bookmark& other) const
				{
					return !(this->frame == other.frame && this->type == other.type);
				}
				
				//! Represents a bookmark type in preview
				enum class Type
				{
					Bookmark    = 0,
					Measurement = 1
				};
				
				Type type ;			//!< The type of the bookmark
				int  frame;			//!< The frame number of the bookmark
			};
			
			Meta()          = default;
			virtual ~Meta() = default;

			DateTime                 creationTime     ;				//!< The creation time of the preview run
			std::string              prefix           ;				//!< The prefix for the preview run				
			std::string              description      ;				//!< The description of the preview run
			std::string		         tag              ;				//!< Tag for the preview run
			int                      numberOfFrames   ;				//!< The number of frames in the preview run
			double                   frameRate        ;				//!< The frame rate of the preview run
			int						 defaultFrameIndex;				//!< The default frame index of the preview run
			std::vector<Attribute>   attributes       ;				//!< The attributes for the preview run
			std::vector<Bookmark>    bookmarks        ;				//!< The bookmarks for the preview run
		};

		//! Returns the meta data of the preview run
		virtual Meta meta()                                    = 0;

		//! Returns the image for the given frame index
		virtual std::unique_ptr<Sense::Image> image(int index) = 0;

		//! Sets the description for the preview 
		virtual bool setDescription(const std::string& description)           = 0;
		
		IPreviewRun()          = default;
		virtual ~IPreviewRun() = default;

		IPreviewRun(const IPreviewRun&)              = delete;
		IPreviewRun(const IPreviewRun&&)             = delete;
		IPreviewRun& operator=(const IPreviewRun&)   = delete;
		IPreviewRun& operator=(IPreviewRun&& source) = delete;
	};
	
}}
