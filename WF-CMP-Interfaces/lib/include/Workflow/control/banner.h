// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include <Sense.h>
#include <workflow.h>

namespace Philips {namespace Workflow {namespace Control {
	enum Modes
	{
		
		Demo				= 0,
		XrayConnectionLost	= 1,
		Remote				= 2,
		Emergency			= 3,
		Service				= 4,
		Administrator		= 5,
		None 				= 6
	};

	namespace TestPeer { class BannerPeer; }
	
	class Banner: public Sense::Control
	{
		friend class TestPeer::BannerPeer;
	public:
		Banner(Sense::Control& parent, const UiStyle& style);
		~Banner();
		void setMode(const Modes& mode, const bool enable) const;

	private:
		Modes mode() const;
		std::unique_ptr<Sense::Control>		_bannerView;
		struct PrivateBanner;
		std::unique_ptr<PrivateBanner>		_pImpl;
	};

}}}