// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "dialogboxviewmodel.h"

namespace Philips { namespace Workflow { namespace Control {

	DialogBoxViewModel::DialogBoxViewModel()
		: header("")
		, text("")
		, accent("")
		, primary("")
		, secondary("")
		, accentVisible(false)
		, primaryVisible(false)
		, secondaryVisible(false)
		, showDialog(false)
	{
	}

}}}