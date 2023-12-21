// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "Workflow/control/virtualkeyboard.h"
#include <Automation.h>

//TICS -INT#002: Test peers needs data member in public

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer {
	class VirtualKeyboardPeer
	{
	public:
		explicit VirtualKeyboardPeer(VirtualKeyboard& keyboard)
			: control(keyboard)
			, keyTab(keyboard.keyboard.keyTab)
			, key00(keyboard.keyboard.key00)
			, key01(keyboard.keyboard.key01)
			, key02(keyboard.keyboard.key02)
			, key03(keyboard.keyboard.key03)
			, key04(keyboard.keyboard.key04)
			, key05(keyboard.keyboard.key05)
			, key06(keyboard.keyboard.key06)
			, key07(keyboard.keyboard.key07)
			, key08(keyboard.keyboard.key08)
			, key09(keyboard.keyboard.key09)
			, keyBack(keyboard.keyboard.keyBack)
			, keyCaps(keyboard.keyboard.keyCaps)
			, key10(keyboard.keyboard.key10)
			, key11(keyboard.keyboard.key11)
			, key12(keyboard.keyboard.key12)
			, key13(keyboard.keyboard.key13)
			, key14(keyboard.keyboard.key14)
			, key15(keyboard.keyboard.key15)
			, key16(keyboard.keyboard.key16)
			, key17(keyboard.keyboard.key17)
			, key18(keyboard.keyboard.key18)
			, keyReturn(keyboard.keyboard.keyReturn)
			, keyLShift(keyboard.keyboard.keyLShift)
			, key20(keyboard.keyboard.key20)
			, key21(keyboard.keyboard.key21)
			, key22(keyboard.keyboard.key22)
			, key23(keyboard.keyboard.key23)
			, key24(keyboard.keyboard.key24)
			, key25(keyboard.keyboard.key25)
			, key26(keyboard.keyboard.key26)
			, key27(keyboard.keyboard.key27)
			, key28(keyboard.keyboard.key28)
			, keyRShift(keyboard.keyboard.keyRShift)
			, keyLNum(keyboard.keyboard.keyLNum)
			, keySpace(keyboard.keyboard.keySpace)
			, keyRNum(keyboard.keyboard.keyRNum)
			, hide(keyboard.keyboard.hide)
		{
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'q', this->key00 ));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'w', this->key01));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'e', this->key02));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'r', this->key03));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L't', this->key04));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'y', this->key05));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'u', this->key06));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'i', this->key07));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'o', this->key08));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'p', this->key09));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'a', this->key10));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L's', this->key11));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'd', this->key12));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'f', this->key13));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'g', this->key14));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'h', this->key15));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'j', this->key16));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'k', this->key17));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'l', this->key18));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'z', this->key20));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'x', this->key21));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'c', this->key22));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'v', this->key23));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'b', this->key24));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'n', this->key25));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'm', this->key26));

			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'Q', this->key00));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'W', this->key01));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'E', this->key02));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'R', this->key03));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'T', this->key04));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'Y', this->key05));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'U', this->key06));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'I', this->key07));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'O', this->key08));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'P', this->key09));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'A', this->key10));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'S', this->key11));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'D', this->key12));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'F', this->key13));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'G', this->key14));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'H', this->key15));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'J', this->key16));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'K', this->key17));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'L', this->key18));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'Z', this->key20));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'X', this->key21));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'C', this->key22));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'V', this->key23));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'B', this->key24));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'N', this->key25));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'M', this->key26));

			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L',', this->key27));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'.', this->key28));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'0', this->key09));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'1', this->key00));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'2', this->key01));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'3', this->key02));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'4', this->key03));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'5', this->key04));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'6', this->key05));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'7', this->key06));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'8', this->key07));
			peerMap.insert(std::pair<wchar_t, Peer<Button>>(L'9', this->key08));
		}														  

		void enterText(const std::wstring& text)
		{
			for (auto wchar : text)
			{
				if (iswdigit(wchar))
				{
					if(!control.showNumeric) pressLeftNumeric();
				}
				else if (iswlower(wchar))
				{
					if (control.capsLock) pressCapsLock();
				}
				else if (iswupper(wchar))
				{
					if (!control.capsLock) pressCapsLock();
				}
				peerMap.at(wchar).mouseClick();
			}
		}

		void pressTab()
		{
			keyTab.mouseClick();
		}

		void pressBackSpace()
		{
			keyBack.mouseClick();
		}

		void pressCapsLock()
		{
			keyCaps.mouseClick();
		}

		void pressEnter()
		{
			keyReturn.mouseClick();
		}

		void pressLeftShift()
		{
			keyLShift.mouseClick();
		}

		void pressRightShift()
		{
			keyRShift.mouseClick();
		}

		void pressLeftNumeric()
		{
			keyLNum.mouseClick();
		}

		void pressRightNumeric()
		{
			keyRNum.mouseClick();
		}

		void pressSpace()
		{
			keySpace.mouseClick();
		}

		void pressHide()
		{
			hide.mouseClick();
		}

		bool isVisible() const
		{
			return control.isKeyBoardVisible();
		}

		void hideKeyboard() const
		{
			control.hide();
		}

		std::wstring firstKeyText() const
		{
			return key00.control.text;
		}

		VirtualKeyboard& control;
		private:
			Peer<Button>     keyTab;
			Peer<Button>     key00;
			Peer<Button>     key01;
			Peer<Button>     key02;
			Peer<Button>     key03;
			Peer<Button>     key04;
			Peer<Button>     key05;
			Peer<Button>     key06;
			Peer<Button>     key07;
			Peer<Button>     key08;
			Peer<Button>     key09;
			Peer<Button>     keyBack;
			Peer<Button>     keyCaps;
			Peer<Button>     key10;
			Peer<Button>     key11;
			Peer<Button>     key12;
			Peer<Button>     key13;
			Peer<Button>     key14;
			Peer<Button>     key15;
			Peer<Button>     key16;
			Peer<Button>     key17;
			Peer<Button>     key18;
			Peer<Button>     keyReturn;
			Peer<Button>     keyLShift;
			Peer<Button>     key20;
			Peer<Button>     key21;
			Peer<Button>     key22;
			Peer<Button>     key23;
			Peer<Button>     key24;
			Peer<Button>     key25;
			Peer<Button>     key26;
			Peer<Button>     key27;
			Peer<Button>     key28;
			Peer<Button>     keyRShift;
			Peer<Button>     keyLNum;
			Peer<Button>     keySpace;
			Peer<Button>     keyRNum;
			Peer<Button>     hide;
		std::map<wchar_t, Peer<Button>> peerMap;
	};

}}}}