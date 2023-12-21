// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include <DLS.h>

namespace Philips { namespace Workflow { namespace Control { namespace Ui {
	using namespace Sense;
	using namespace Sense::DLS;

	class VirtualKeyboard : public Panel
	{
	public:

		explicit VirtualKeyboard(Control &parent)
		:
			Panel(parent)
		#pragma region Sense Composer (do not modify)
	,	row0             (*this)
	,	keyTab           (row0)
	,	key00            (row0)
	,	key01            (row0)
	,	key02            (row0)
	,	key03            (row0)
	,	key04            (row0)
	,	key05            (row0)
	,	key06            (row0)
	,	key07            (row0)
	,	key08            (row0)
	,	key09            (row0)
	,	keyBack          (row0)
	,	row1             (*this)
	,	keyCaps          (row1)
	,	key10            (row1)
	,	key11            (row1)
	,	key12            (row1)
	,	key13            (row1)
	,	key14            (row1)
	,	key15            (row1)
	,	key16            (row1)
	,	key17            (row1)
	,	key18            (row1)
	,	keyReturn        (row1)
	,	row2             (*this)
	,	control2         (row2)
	,	keyLShift        (row2)
	,	key20            (row2)
	,	key21            (row2)
	,	key22            (row2)
	,	key23            (row2)
	,	key24            (row2)
	,	key25            (row2)
	,	key26            (row2)
	,	key27            (row2)
	,	key28            (row2)
	,	keyRShift        (row2)
	,	control          (*this)
	,	control3         ((Control&)control)
	,	keyLNum          (control)
	,	keySpace         (control)
	,	keyRNum          (control)
	,	hide             (control)
		#pragma endregion
		{
		#pragma region Sense Composer (do not modify)
		layoutUpdateBegin();
		//composerReference              = ..\..\doc\Screenshots\image2017-7-31_10-43-35.png;
		//composerImageNamespace         = SdkResources;
		//composerImageFolder            = ..\Resources;
		name                             = L"keyboard";
		size                             = Size(1280, 342);
		font                             = Font(L"CentraleSansBook", 24, false, false, false, false, Font::Antialias::High);
		noActivate                       = true;
		layoutInnerMargin                = Margin(16, 16, 16, 16);
		layoutSpacing                    = 16;
		color                            = Palette::Gray60;
		row0.name                        = L"row0";
		row0.position                    = Point(16, 16);
		row0.size                        = Size(1248, 63);
		row0.noActivate                  = true;
		row0.layout                      = Layout::Fixed1d;
		row0.layoutSpacing               = 13;
		row0.layoutOrientation           = Orientation::Horizontal;
		keyTab.name                      = L"virtualKeyboardKey";
		keyTab.size                      = Size(123, 63);
		keyTab.font                      = Font(L"CentraleSansBook", 20, false, false, false, false, Font::Antialias::High);
		keyTab.tabStop                   = false;
		keyTab.noActivate                = true;
		keyTab.layoutHideInvisible       = false;
		keyTab.layout                    = Layout::Fixed1d;
		keyTab.repeatClicked             = true;
		keyTab.text                      = L"TAB";
		keyTab.textMargin                = Margin(2, 4, 4, 4);
		key00.name                       = L"virtualKeyboardKey";
		key00.position                   = Point(136, 0);
		key00.size                       = Size(68, 63);
		key00.tabStop                    = false;
		key00.noActivate                 = true;
		key00.layoutHideInvisible        = false;
		key00.layout                     = Layout::Fixed1d;
		key00.repeatClicked              = true;
		key00.text                       = L"q";
		key00.textMargin                 = Margin(2, 4, 4, 10);
		key01.name                       = L"virtualKeyboardKey";
		key01.position                   = Point(217, 0);
		key01.size                       = Size(68, 63);
		key01.tabStop                    = false;
		key01.noActivate                 = true;
		key01.layoutHideInvisible        = false;
		key01.layout                     = Layout::Fixed1d;
		key01.repeatClicked              = true;
		key01.text                       = L"w";
		key01.textMargin                 = Margin(2, 4, 4, 10);
		key02.name                       = L"virtualKeyboardKey";
		key02.position                   = Point(298, 0);
		key02.size                       = Size(68, 63);
		key02.tabStop                    = false;
		key02.noActivate                 = true;
		key02.layoutHideInvisible        = false;
		key02.layout                     = Layout::Fixed1d;
		key02.repeatClicked              = true;
		key02.text                       = L"e";
		key02.textMargin                 = Margin(2, 4, 4, 10);
		key03.name                       = L"virtualKeyboardKey";
		key03.position                   = Point(379, 0);
		key03.size                       = Size(68, 63);
		key03.tabStop                    = false;
		key03.noActivate                 = true;
		key03.layoutHideInvisible        = false;
		key03.layout                     = Layout::Fixed1d;
		key03.repeatClicked              = true;
		key03.text                       = L"r";
		key03.textMargin                 = Margin(2, 4, 4, 10);
		key04.name                       = L"virtualKeyboardKey";
		key04.position                   = Point(460, 0);
		key04.size                       = Size(68, 63);
		key04.tabStop                    = false;
		key04.noActivate                 = true;
		key04.layoutHideInvisible        = false;
		key04.layout                     = Layout::Fixed1d;
		key04.repeatClicked              = true;
		key04.text                       = L"t";
		key04.textMargin                 = Margin(2, 4, 4, 10);
		key05.name                       = L"virtualKeyboardKey";
		key05.position                   = Point(541, 0);
		key05.size                       = Size(68, 63);
		key05.tabStop                    = false;
		key05.noActivate                 = true;
		key05.layoutHideInvisible        = false;
		key05.layout                     = Layout::Fixed1d;
		key05.repeatClicked              = true;
		key05.text                       = L"y";
		key05.textMargin                 = Margin(2, 4, 4, 10);
		key06.name                       = L"virtualKeyboardKey";
		key06.position                   = Point(622, 0);
		key06.size                       = Size(68, 63);
		key06.tabStop                    = false;
		key06.noActivate                 = true;
		key06.layoutHideInvisible        = false;
		key06.layout                     = Layout::Fixed1d;
		key06.repeatClicked              = true;
		key06.text                       = L"u";
		key06.textMargin                 = Margin(2, 4, 4, 10);
		key07.name                       = L"virtualKeyboardKey";
		key07.position                   = Point(703, 0);
		key07.size                       = Size(68, 63);
		key07.tabStop                    = false;
		key07.noActivate                 = true;
		key07.layoutHideInvisible        = false;
		key07.layout                     = Layout::Fixed1d;
		key07.repeatClicked              = true;
		key07.text                       = L"i";
		key07.textMargin                 = Margin(2, 4, 4, 10);
		key08.name                       = L"virtualKeyboardKey";
		key08.position                   = Point(784, 0);
		key08.size                       = Size(68, 63);
		key08.tabStop                    = false;
		key08.noActivate                 = true;
		key08.layoutHideInvisible        = false;
		key08.layout                     = Layout::Fixed1d;
		key08.repeatClicked              = true;
		key08.text                       = L"o";
		key08.textMargin                 = Margin(2, 4, 4, 10);
		key09.name                       = L"virtualKeyboardKey";
		key09.position                   = Point(865, 0);
		key09.size                       = Size(68, 63);
		key09.tabStop                    = false;
		key09.noActivate                 = true;
		key09.layoutHideInvisible        = false;
		key09.layout                     = Layout::Fixed1d;
		key09.repeatClicked              = true;
		key09.text                       = L"p";
		key09.textMargin                 = Margin(2, 4, 4, 10);
		keyBack.name                     = L"virtualKeyboardKey";
		keyBack.position                 = Point(946, 0);
		keyBack.size                     = Size(301, 63);
		keyBack.tabStop                  = false;
		keyBack.noActivate               = true;
		keyBack.layoutHideInvisible      = false;
		keyBack.layout                   = Layout::Fixed1d;
		keyBack.repeatClicked            = true;
		keyBack.text                     = L"BackSpace";
		keyBack.textMargin               = Margin(2, 4, 4, 4);
		keyBack.imageSize                = Size(37, 24);
		row1.name                        = L"row1";
		row1.position                    = Point(16, 95);
		row1.size                        = Size(1248, 63);
		row1.noActivate                  = true;
		row1.layout                      = Layout::Fixed1d;
		row1.layoutSpacing               = 13;
		row1.layoutOrientation           = Orientation::Horizontal;
		keyCaps.name                     = L"virtualKeyboardKey";
		keyCaps.size                     = Size(136, 63);
		keyCaps.font                     = Font(L"CentraleSansBook", 20, false, false, false, false, Font::Antialias::High);
		keyCaps.tabStop                  = false;
		keyCaps.noActivate               = true;
		keyCaps.layoutHideInvisible      = false;
		keyCaps.layout                   = Layout::Fixed1d;
		keyCaps.repeatClicked            = true;
		keyCaps.text                     = L"CAPSLOCK";
		keyCaps.textMargin               = Margin(2, 4, 4, 4);
		key10.name                       = L"virtualKeyboardKey";
		key10.position                   = Point(149, 0);
		key10.size                       = Size(68, 63);
		key10.tabStop                    = false;
		key10.noActivate                 = true;
		key10.layoutHideInvisible        = false;
		key10.layout                     = Layout::Fixed1d;
		key10.repeatClicked              = true;
		key10.text                       = L"a";
		key10.textMargin                 = Margin(2, 4, 4, 10);
		key11.name                       = L"virtualKeyboardKey";
		key11.position                   = Point(230, 0);
		key11.size                       = Size(68, 63);
		key11.tabStop                    = false;
		key11.noActivate                 = true;
		key11.layoutHideInvisible        = false;
		key11.layout                     = Layout::Fixed1d;
		key11.repeatClicked              = true;
		key11.text                       = L"s";
		key11.textMargin                 = Margin(2, 4, 4, 10);
		key12.name                       = L"virtualKeyboardKey";
		key12.position                   = Point(311, 0);
		key12.size                       = Size(68, 63);
		key12.tabStop                    = false;
		key12.noActivate                 = true;
		key12.layoutHideInvisible        = false;
		key12.layout                     = Layout::Fixed1d;
		key12.repeatClicked              = true;
		key12.text                       = L"d";
		key12.textMargin                 = Margin(2, 4, 4, 10);
		key13.name                       = L"virtualKeyboardKey";
		key13.position                   = Point(392, 0);
		key13.size                       = Size(68, 63);
		key13.tabStop                    = false;
		key13.noActivate                 = true;
		key13.layoutHideInvisible        = false;
		key13.layout                     = Layout::Fixed1d;
		key13.repeatClicked              = true;
		key13.text                       = L"f";
		key13.textMargin                 = Margin(2, 4, 4, 10);
		key14.name                       = L"virtualKeyboardKey";
		key14.position                   = Point(473, 0);
		key14.size                       = Size(68, 63);
		key14.tabStop                    = false;
		key14.noActivate                 = true;
		key14.layoutHideInvisible        = false;
		key14.layout                     = Layout::Fixed1d;
		key14.repeatClicked              = true;
		key14.text                       = L"g";
		key14.textMargin                 = Margin(2, 4, 4, 10);
		key15.name                       = L"virtualKeyboardKey";
		key15.position                   = Point(554, 0);
		key15.size                       = Size(68, 63);
		key15.tabStop                    = false;
		key15.noActivate                 = true;
		key15.layoutHideInvisible        = false;
		key15.layout                     = Layout::Fixed1d;
		key15.repeatClicked              = true;
		key15.text                       = L"h";
		key15.textMargin                 = Margin(2, 4, 4, 10);
		key16.name                       = L"virtualKeyboardKey";
		key16.position                   = Point(635, 0);
		key16.size                       = Size(68, 63);
		key16.tabStop                    = false;
		key16.noActivate                 = true;
		key16.layoutHideInvisible        = false;
		key16.layout                     = Layout::Fixed1d;
		key16.repeatClicked              = true;
		key16.text                       = L"j";
		key16.textMargin                 = Margin(2, 4, 4, 10);
		key17.name                       = L"virtualKeyboardKey";
		key17.position                   = Point(716, 0);
		key17.size                       = Size(68, 63);
		key17.tabStop                    = false;
		key17.noActivate                 = true;
		key17.layoutHideInvisible        = false;
		key17.layout                     = Layout::Fixed1d;
		key17.repeatClicked              = true;
		key17.text                       = L"k";
		key17.textMargin                 = Margin(2, 4, 4, 10);
		key18.name                       = L"virtualKeyboardKey";
		key18.position                   = Point(797, 0);
		key18.size                       = Size(68, 63);
		key18.tabStop                    = false;
		key18.noActivate                 = true;
		key18.layoutHideInvisible        = false;
		key18.layout                     = Layout::Fixed1d;
		key18.repeatClicked              = true;
		key18.text                       = L"l";
		key18.textMargin                 = Margin(2, 4, 4, 10);
		keyReturn.name                   = L"virtualKeyboardKey";
		keyReturn.position               = Point(878, 0);
		keyReturn.size                   = Size(301, 63);
		keyReturn.font                   = Font(L"CentraleSansBook", 20, false, false, false, false, Font::Antialias::High);
		keyReturn.tabStop                = false;
		keyReturn.noActivate             = true;
		keyReturn.layoutHideInvisible    = false;
		keyReturn.layout                 = Layout::Fixed1d;
		keyReturn.repeatClicked          = true;
		keyReturn.text                   = L"ENTER";
		keyReturn.textMargin             = Margin(2, 4, 4, 4);
		row2.name                        = L"row2";
		row2.position                    = Point(16, 174);
		row2.size                        = Size(1248, 63);
		row2.noActivate                  = true;
		row2.layout                      = Layout::Fixed1d;
		row2.layoutSpacing               = 13;
		row2.layoutOrientation           = Orientation::Horizontal;
		control2.name                    = L"virtualKeyboardKey";
		control2.size                    = Size(28, 63);
		control2.layoutHideInvisible     = false;
		control2.layout                  = Layout::Fixed1d;
		keyLShift.name                   = L"virtualKeyboardKey";
		keyLShift.position               = Point(41, 0);
		keyLShift.size                   = Size(120, 63);
		keyLShift.tabStop                = false;
		keyLShift.noActivate             = true;
		keyLShift.layoutHideInvisible    = false;
		keyLShift.layout                 = Layout::Fixed1d;
		keyLShift.text                   = L"Shift";
		keyLShift.textMargin             = Margin(2, 4, 4, 4);
		keyLShift.imageSize              = Size(23, 22);
		key20.name                       = L"virtualKeyboardKey";
		key20.position                   = Point(174, 0);
		key20.size                       = Size(68, 63);
		key20.tabStop                    = false;
		key20.noActivate                 = true;
		key20.layoutHideInvisible        = false;
		key20.layout                     = Layout::Fixed1d;
		key20.repeatClicked              = true;
		key20.text                       = L"z";
		key20.textMargin                 = Margin(2, 4, 4, 10);
		key21.name                       = L"virtualKeyboardKey";
		key21.position                   = Point(255, 0);
		key21.size                       = Size(68, 63);
		key21.tabStop                    = false;
		key21.noActivate                 = true;
		key21.layoutHideInvisible        = false;
		key21.layout                     = Layout::Fixed1d;
		key21.repeatClicked              = true;
		key21.text                       = L"x";
		key21.textMargin                 = Margin(2, 4, 4, 10);
		key22.name                       = L"virtualKeyboardKey";
		key22.position                   = Point(336, 0);
		key22.size                       = Size(68, 63);
		key22.tabStop                    = false;
		key22.noActivate                 = true;
		key22.layoutHideInvisible        = false;
		key22.layout                     = Layout::Fixed1d;
		key22.repeatClicked              = true;
		key22.text                       = L"c";
		key22.textMargin                 = Margin(2, 4, 4, 10);
		key23.name                       = L"virtualKeyboardKey";
		key23.position                   = Point(417, 0);
		key23.size                       = Size(68, 63);
		key23.tabStop                    = false;
		key23.noActivate                 = true;
		key23.layoutHideInvisible        = false;
		key23.layout                     = Layout::Fixed1d;
		key23.repeatClicked              = true;
		key23.text                       = L"v";
		key23.textMargin                 = Margin(2, 4, 4, 10);
		key24.name                       = L"virtualKeyboardKey";
		key24.position                   = Point(498, 0);
		key24.size                       = Size(68, 63);
		key24.tabStop                    = false;
		key24.noActivate                 = true;
		key24.layoutHideInvisible        = false;
		key24.layout                     = Layout::Fixed1d;
		key24.repeatClicked              = true;
		key24.text                       = L"b";
		key24.textMargin                 = Margin(2, 4, 4, 10);
		key25.name                       = L"virtualKeyboardKey";
		key25.position                   = Point(579, 0);
		key25.size                       = Size(68, 63);
		key25.tabStop                    = false;
		key25.noActivate                 = true;
		key25.layoutHideInvisible        = false;
		key25.layout                     = Layout::Fixed1d;
		key25.repeatClicked              = true;
		key25.text                       = L"n";
		key25.textMargin                 = Margin(2, 4, 4, 10);
		key26.name                       = L"virtualKeyboardKey";
		key26.position                   = Point(660, 0);
		key26.size                       = Size(68, 63);
		key26.tabStop                    = false;
		key26.noActivate                 = true;
		key26.layoutHideInvisible        = false;
		key26.layout                     = Layout::Fixed1d;
		key26.repeatClicked              = true;
		key26.text                       = L"m";
		key26.textMargin                 = Margin(2, 4, 4, 10);
		key27.name                       = L"virtualKeyboardKey";
		key27.position                   = Point(741, 0);
		key27.size                       = Size(68, 63);
		key27.tabStop                    = false;
		key27.noActivate                 = true;
		key27.layoutHideInvisible        = false;
		key27.layout                     = Layout::Fixed1d;
		key27.repeatClicked              = true;
		key27.text                       = L",";
		key27.textMargin                 = Margin(2, 4, 4, 10);
		key28.name                       = L"virtualKeyboardKey";
		key28.position                   = Point(822, 0);
		key28.size                       = Size(68, 63);
		key28.tabStop                    = false;
		key28.noActivate                 = true;
		key28.layoutHideInvisible        = false;
		key28.layout                     = Layout::Fixed1d;
		key28.repeatClicked              = true;
		key28.text                       = L".";
		key28.textMargin                 = Margin(2, 4, 4, 10);
		keyRShift.name                   = L"virtualKeyboardKey";
		keyRShift.position               = Point(903, 0);
		keyRShift.size                   = Size(276, 63);
		keyRShift.tabStop                = false;
		keyRShift.noActivate             = true;
		keyRShift.layoutHideInvisible    = false;
		keyRShift.layout                 = Layout::Fixed1d;
		keyRShift.text                   = L"Shift";
		keyRShift.textMargin             = Margin(2, 4, 4, 4);
		keyRShift.imageSize              = Size(23, 22);
		control.position                 = Point(16, 253);
		control.size                     = Size(1248, 63);
		control.layout                   = Layout::Fixed1d;
		control.layoutSpacing            = 13;
		control.layoutOrientation        = Orientation::Horizontal;
		control3.name                    = L"virtualKeyboardKey";
		control3.size                    = Size(34, 63);
		control3.layoutHideInvisible     = false;
		control3.layout                  = Layout::Fixed1d;
		keyLNum.name                     = L"virtualKeyboardKey";
		keyLNum.position                 = Point(47, 0);
		keyLNum.size                     = Size(128, 63);
		keyLNum.font                     = Font(L"CentraleSansBook", 20, false, false, false, false, Font::Antialias::High);
		keyLNum.tabStop                  = false;
		keyLNum.noActivate               = true;
		keyLNum.layoutHideInvisible      = false;
		keyLNum.layout                   = Layout::Fixed1d;
		keyLNum.text                     = L".123";
		keyLNum.textMargin               = Margin(2, 4, 4, 4);
		keySpace.name                    = L"virtualKeyboardKey";
		keySpace.position                = Point(188, 0);
		keySpace.size                    = Size(744, 63);
		keySpace.tabStop                 = false;
		keySpace.noActivate              = true;
		keySpace.layoutHideInvisible     = false;
		keySpace.layout                  = Layout::Fixed1d;
		keySpace.repeatClicked           = true;
		keySpace.text                    = L"";
		keySpace.textMargin              = Margin(2, 4, 4, 10);
		keyRNum.name                     = L"virtualKeyboardKey";
		keyRNum.position                 = Point(945, 0);
		keyRNum.size                     = Size(123, 63);
		keyRNum.font                     = Font(L"CentraleSansBook", 20, false, false, false, false, Font::Antialias::High);
		keyRNum.tabStop                  = false;
		keyRNum.noActivate               = true;
		keyRNum.layoutHideInvisible      = false;
		keyRNum.layout                   = Layout::Fixed1d;
		keyRNum.text                     = L".123";
		keyRNum.textMargin               = Margin(2, 4, 4, 4);
		hide.name                        = L"hide";
		hide.position                    = Point(1081, 0);
		hide.size                        = Size(136, 63);
		hide.tabStop                     = false;
		hide.noActivate                  = true;
		hide.layoutHideInvisible         = false;
		hide.layout                      = Layout::Fixed1d;
		hide.text                        = L"Hide";
		hide.textMargin                  = Margin(2, 8, 4, 4);
		hide.imageSize                   = Size(60, 36);
		layoutUpdateEnd();
		#pragma endregion
		}


		class Button : public DLS::Button
		{
		public:
			Button(Control &parent) : DLS::Button(parent)
			{
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//										Up       			UpCheck				Down     			DownCheck			Hover    			HoverCheck 			Disabled			DisabledCheck	
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				static const Color defaultButton [] = { Palette::Gray70,	Palette::Gray45,	Palette::Gray50,	Palette::Gray55,	Palette::Gray65,	Palette::Gray50,	Palette::Gray65,	Palette::Gray40 };
				static const Color defaultFont   [] = { Palette::White,		Palette::White,		Palette::White,		Palette::White,		Palette::White,		Palette::White,		Palette::Gray60,	Palette::White  };
				//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				colorButton = &defaultButton;
				colorFont   = &defaultFont;
			}
		};

	#pragma region Sense Composer (do not modify)
	public:    Spacer            row0;
	public:    Button            keyTab;
	public:    Button            key00;
	public:    Button            key01;
	public:    Button            key02;
	public:    Button            key03;
	public:    Button            key04;
	public:    Button            key05;
	public:    Button            key06;
	public:    Button            key07;
	public:    Button            key08;
	public:    Button            key09;
	public:    Button            keyBack;
	public:    Spacer            row1;
	public:    Button            keyCaps;
	public:    Button            key10;
	public:    Button            key11;
	public:    Button            key12;
	public:    Button            key13;
	public:    Button            key14;
	public:    Button            key15;
	public:    Button            key16;
	public:    Button            key17;
	public:    Button            key18;
	public:    Button            keyReturn;
	public:    Spacer            row2;
	private:   Control           control2;
	public:    Button            keyLShift;
	public:    Button            key20;
	public:    Button            key21;
	public:    Button            key22;
	public:    Button            key23;
	public:    Button            key24;
	public:    Button            key25;
	public:    Button            key26;
	public:    Button            key27;
	public:    Button            key28;
	public:    Button            keyRShift;
	private:   Control           control;
	private:   Control           control3;
	public:    Button            keyLNum;
	public:    Button            keySpace;
	public:    Button            keyRNum;
	public:    Button            hide;
	#pragma endregion
	private:

		VirtualKeyboard(const VirtualKeyboard&);
		VirtualKeyboard& operator=(const VirtualKeyboard&);
	};
}}}}
