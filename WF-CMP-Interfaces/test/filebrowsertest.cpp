// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
//TICS -INT#027 -OAL#011  

#include "Workflow/control/testpeer/filebrowserpeer.h"
#include "automateduitest.h"
#include "src/Localization.h"
#include "Workflow/Media/usbmonitor.h"
#include "Mock/usbMonitor.mock.h"
#include <fstream>

using namespace Philips::Workflow::Control;

class FileBrowserTest : public AutomatedUiTest , public testing::WithParamInterface<UiStyle>
{
public:
	FileBrowserTest() :AutomatedUiTest(Sense::Rect(Sense::Point(), Sense::Size(1920, 1080)))
		, style(GetParam())
	{ 
		browser.reset(new FileBrowser(canvas(), style));
		peer.reset(new TestPeer::FileBrowserPeer(*browser));
		testPath = std::filesystem::temp_directory_path() / L"FileBrowserTest";
		std::filesystem::create_directory(testPath);
		std::filesystem::create_directories(testPath / L"dir1" / L"dir11");
		std::filesystem::create_directories(testPath / L"dir_2" / L"dir21");
		std::filesystem::create_directories(testPath / L"dir__3" / L"dir31");
		browser->pathSelected += [this](const auto& newPath) { selectedPath = newPath; };
		this->browser->setPath(testPath,"Test");
		this->browser->visible = true;
	}

	void initializeSecondFileBrowser(std::unique_ptr<FileBrowser>& fileBrowser, bool visible)
	{
		fileBrowser = std::make_unique<FileBrowser>(canvas(), style);
		fileBrowser->visible = visible;
		fileBrowser->pathSelected += [this](auto) { triggered = true; };
	}

	~FileBrowserTest()
	{
		std::filesystem::remove_all(testPath);
	}

	UiStyle style;
	std::unique_ptr<FileBrowser> browser;
	std::unique_ptr<TestPeer::FileBrowserPeer> peer;
	std::experimental::filesystem::path testPath;
	std::experimental::filesystem::path selectedPath;
	MockUsbMonitor						_monitor;
	bool								triggered{false};
};


TEST_P(FileBrowserTest, When_PathIsSet_Then_FileBrowserIsLaunched)
{
	ASSERT_EQ(peer->visiblity(), true);
}


TEST_P(FileBrowserTest, When_FileBrowserIsCreatedWithDefaultParameter_Then_AddFolderButtonIsVisible)
{
	browser->setPath(testPath, "Test");
	ASSERT_EQ(peer->isAddFolderButtonVisible(), true);
}

TEST_P(FileBrowserTest, When_FileBrowserIsCreatedWithDefaultParameter_Then_BrowserTitleIsEmpty)
{
	browser->setPath(testPath, "Test");
	ASSERT_TRUE(peer->getBrowserTitleText().empty());
}

TEST_P(FileBrowserTest, Given_FileBrowserTitle_When_FileBrowserIsLaunched_Then_BrowserTitleIsSet)
{
	FileBrowser fileBrowser(canvas(), style, L"Select Destination");
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	fileBrowser.setPath(testPath, "Test");
	ASSERT_EQ(testPeer.getBrowserTitleText(), L"Select Destination");
}

TEST_P(FileBrowserTest, Given_ReadOnlyIsSetToTrue_When_FileBrowserIsLaunched_Then_AddFolderIsInvisible)
{
	FileBrowser fileBrowser(canvas(), style, L"Select Destination", true);
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	fileBrowser.setPath(testPath, "Test");
	ASSERT_EQ(testPeer.isAddFolderButtonVisible(), false);
}

TEST_P(FileBrowserTest, When_FileBrowserIsLaunched_Then_ItIsLaunchedAsModalDialog)
{
	FileBrowser fileBrowser(canvas(), style, L"Select Destination", true);
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;
	fileBrowser.setPath(testPath, "Test");
	ASSERT_TRUE(fileBrowser.modal);
}

TEST_P(FileBrowserTest, When_FileBrowserPositionIsSet_Then_TheDialogIsLaunchedInCorrectPosition)
{
	FileBrowser fileBrowser(canvas(), style, L"Select Destination", true);
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	const auto position = Sense::Point(100, 100);
	fileBrowser.setPosition(position);
	fileBrowser.visible = true;
	fileBrowser.setPath(testPath, "Test");
	ASSERT_EQ(testPeer.getPosition(), position);
}

TEST_P(FileBrowserTest, Given_RootFolder_When_VolumeNameIsEmpty_Then_SelectedPathIsRootFolder)
{
	FileBrowser fileBrowser(canvas(), style, L"Select Destination");
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	const std::wstring path = L"C:/";
	fileBrowser.setPath("C:\\", "");
	ASSERT_EQ(testPeer.getSelectedPathText(), path);
}

TEST_P(FileBrowserTest, When_VolumeNameIsSet_Then_RootFolderIsReplacedWithVolumeName)
{
	FileBrowser fileBrowser(canvas(), style, L"Select Destination");
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	fileBrowser.setPath("C:\\", "USB");
	ASSERT_EQ(testPeer.getSelectedPathText(), L"USB:/");
}

TEST_P(FileBrowserTest, When_AddFolderClicked_Then_BrowserTitleIsSetToNewFolder)
{
	FileBrowser fileBrowser(canvas(), style, L"Select Destination",false);
	TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.setPath("C:\\", "USB");
	fileBrowser.visible = true;

	testPeer.clickAddFolderButton();

	ASSERT_EQ(testPeer.getBrowserTitleText(),Philips::Workflow::Localization::newFolder);
}

TEST_P(FileBrowserTest, Given_AddFolderIsClicked_When_BackButtonClicked_Then_BrowserTitleIsSetToDefaultTitle)
{
	FileBrowser fileBrowser(canvas(), style, L"Select Destination");
	TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.setPath("C:\\", "USB");
	fileBrowser.visible = true;

	testPeer.clickAddFolderButton();
	testPeer.folderUp();

	ASSERT_EQ(testPeer.getBrowserTitleText(), L"Select Destination");
}

TEST_P(FileBrowserTest, When_FolderNameIsGiven_Then_SameFolderIsSelected)
{
	
	ASSERT_TRUE(this->peer->selectFolder(L"dir_2"));
	ASSERT_EQ(this->peer->selectedFolder(), 1);

}

TEST_P(FileBrowserTest, When_FolderNameIsGiven_ThenSameFolderIsOpened)
{
	
	ASSERT_TRUE(this->peer->expandFolder(L"dir_2"));
	ASSERT_EQ(this->peer->getFolderName(0, 2), L"dir21");
}

TEST_P(FileBrowserTest, When_AddFolder_Then_FolderAdded)
{

	ASSERT_TRUE(this->peer->addFolder(L"dir__4"));
	ASSERT_EQ(this->peer->getFolderName(3, 2), L"dir__4");
}

TEST_P(FileBrowserTest, When_InvalidFolderNameIsGiven_Then_WrongFileNameIsEnabled)
{
	
	ASSERT_FALSE(this->peer->addFolder(L"CON"));
	ASSERT_EQ(this->peer->wrongFileNameVisibility(), true);
	
}

TEST_P(FileBrowserTest, When_FolderPathLengthLimitExceeds_Then_WrongFileNameIsEnabled)
{

	ASSERT_FALSE(this->peer->addFolder(L"2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222"));
	ASSERT_EQ(this->peer->wrongFileNameVisibility(), true);
}

TEST_P(FileBrowserTest, When_UnSupportedCharactersIsGiven_Then_WrongFileNameIsEnabled)
{
	
	ASSERT_FALSE(this->peer->addFolder(L"dir?4"));
	ASSERT_EQ(this->peer->wrongFileNameVisibility(), true);
}

TEST_P(FileBrowserTest, When_DuplicateFolderNameIsGiven_Then_WrongFileNameIsEnabled)
{
	
	ASSERT_FALSE(this->peer->addFolder(L"dir_2"));
	ASSERT_EQ(this->peer->wrongFileNameVisibility(), true);
}
TEST_P(FileBrowserTest, When_FolderUp_Then_ViewIsUpdated)
{
	
	ASSERT_TRUE(this->peer->expandFolder(L"dir_2"));
	ASSERT_TRUE(this->peer->folderUp());
	ASSERT_EQ(this->peer->getFolderName(0, 2), L"dir1");
}

TEST_P(FileBrowserTest, When_SelectIsClicked_Then_BrowserCloses)
{
	
	ASSERT_TRUE(this->peer->selectFolder(L"dir_2"));
	ASSERT_TRUE(this->peer->select());
	ASSERT_EQ(this->browser->isVisible(), false);
}

TEST_P(FileBrowserTest, When_CancelIsClicked_Then_BrowserCloses)
{
	
	ASSERT_TRUE(this->peer->selectFolder(L"dir_2"));
	ASSERT_EQ(this->peer->cancel(), true);
	ASSERT_EQ(this->peer->visiblity(), false);
}

TEST_P(FileBrowserTest, Given_PathIsSetAndFoldersBrowsed_When_PathIsSetAgain_Then_FolderUpIsDisabled)
{
	ASSERT_TRUE(this->peer->expandFolder(L"dir_2"));
	ASSERT_TRUE(this->peer->selectFolder(L"dir21"));
	ASSERT_TRUE(this->peer->select());

	this->browser->setPath(testPath, "Test");
	ASSERT_FALSE(this->peer->isBackButtonEnabled());
}

TEST_P(FileBrowserTest, Given_FolderIsSelected_When_MouseIsDoubleClickedNotOnTheItem_Then_SelectedFolderIsNotExpanded)
{
	ASSERT_TRUE(this->peer->selectFolder(L"dir_2"));
	ASSERT_TRUE(this->peer->expandFolder(L"dir1"));

	this->browser->setPath(testPath, "Test");
	ASSERT_NE(this->peer->getFolderName(0, 2), L"dir21");
}

TEST_P(FileBrowserTest, Given_FileBrowser_When_ValidSubFolderIsBrowsedAndSelected_Then_SelectedPathIsValidSubFolder)
{
	ASSERT_TRUE(this->peer->browse(L"dir_2\\dir21"));
	ASSERT_TRUE(this->peer->select());
	
	ASSERT_EQ(selectedPath.filename().wstring(), L"dir21");
}

TEST_P(FileBrowserTest, Given_FileBrowser_When_InvalidSubFolderIsBrowsedAndSelected_Then_SelectedPathIsOldSelectedPath)
{
	ASSERT_FALSE(this->peer->browse(L"InvalidDir1\\InvalidDir2"));
	ASSERT_TRUE(this->peer->select());

	ASSERT_EQ(selectedPath, testPath);
}

TEST_P(FileBrowserTest, Given_FileBrowser_When_ValidFolderAndInvalidSubFolderIsBrowsedAndSelected_Then_SelectedPathValidFolderPath)
{
	ASSERT_FALSE(this->peer->browse(L"dir_2\\InvalidDir2"));
	ASSERT_TRUE(this->peer->select());

	ASSERT_EQ(selectedPath, std::filesystem::path(testPath / L"dir_2"));
}

TEST_P(FileBrowserTest, Given_FileBrowserWith20Folders_When_ValidFolderAndValidSubFolderIsBrowsedAndSelected_Then_FolderIsScrolledAndExpandedAndSubFolderSelected)
{
	for(int i = 0; i < 20; i++)
	{
		std::filesystem::create_directories(testPath / (L"dir0"+std::to_wstring(i)));
	}
	
	ASSERT_TRUE(this->peer->browse(L"dir_2\\dir21"));
	ASSERT_TRUE(this->peer->select());

	ASSERT_EQ(selectedPath.filename().wstring(), L"dir21");
}

TEST_P(FileBrowserTest, Given_FileBrowserWith20SubFolders_When_ValidFolderAndValidSubFolderIsBrowsedAndSelected_Then_FolderExpandedAndSubFolderIsScrolledAndSelected)
{
	for (int i = 0; i < 20; i++)
	{
		std::filesystem::create_directories(testPath / L"dir_2" / (L"dir0" + std::to_wstring(i)));
	}

	ASSERT_TRUE(this->peer->browse(L"dir_2\\dir21"));
	ASSERT_TRUE(this->peer->select());

	ASSERT_EQ(selectedPath.filename().wstring(), L"dir21");
}

TEST_P(FileBrowserTest, Given_FileBrowserTitle_When_FileBrowserIsLaunched_Then_DisplayedConnectedUSBsListed)
{
	std::vector<std::unique_ptr<IUsb>> usbs;
	usbs.emplace_back(std::make_unique<Usb>("D:\\", "Volume1", 0, 0));
	ON_CALL(_monitor, usbs()).WillByDefault(testing::ReturnRef(usbs));
	_monitor.usbAdded(*usbs[0]);

	FileBrowser fileBrowser(canvas(), style, _monitor, L"Select File");
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	std::vector <std::string> fileType;
	fileType.emplace_back(".pfx");
	fileType.emplace_back(".p12");
	fileType.emplace_back(".cer");
	fileType.emplace_back(".crt");
	fileType.emplace_back(".p7b");
	fileBrowser.setAllowedFileTypes(fileType);

	ASSERT_EQ(testPeer.getBrowserTitleText(), L"Select File");
	ASSERT_EQ(testPeer.isAddFolderButtonVisible(), false);
	ASSERT_EQ(testPeer.getSelectedPathText(), L"This System");
}

TEST_P(FileBrowserTest, Given_UsbSelected_When_CertificateSelected_Then_ImportEnabled)
{
	std::vector<std::unique_ptr<IUsb>> usbs;
	usbs.emplace_back(std::make_unique<Usb>(testPath.string(), "Volume1", 0, 0));
	ON_CALL(_monitor, usbs()).WillByDefault(testing::ReturnRef(usbs));
	_monitor.usbAdded(*usbs[0]);

	FileBrowser fileBrowser(canvas(), style, _monitor, L"Select File");
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	std::filesystem::create_directories(testPath / L"Drive1" );
	const auto filePath = testPath / "Drive1" / "Certificate.pfx";
	std::ofstream nodeFile(filePath);

	std::vector <std::string> fileType;
	fileType.emplace_back(".pfx");
	fileBrowser.setAllowedFileTypes(fileType);

	fileBrowser.setPath(testPath / L"Drive1", "");
	ASSERT_EQ(testPeer.selectFolder(L"Volume1"), 1);

	ASSERT_TRUE(testPeer.expandFolder(L"Volume1"));
	ASSERT_EQ(testPeer.selectFolder(L"Drive1"), 1);
	ASSERT_TRUE(testPeer.expandFolder(L"Drive1"));

	ASSERT_EQ(testPeer.selectFolder(L"Certificate.pfx"), 1);
	ASSERT_TRUE(testPeer.select());

	ASSERT_EQ(testPeer.getSelectedPathText(), L"Volume1:/TEMP/FileBrowserTest/Drive1/Certificate.pfx");
}

TEST_P(FileBrowserTest, Given_UsbsListed_When_UsbSelected_Then_OpenFolder)
{
	std::vector<std::unique_ptr<IUsb>> usbs;
	usbs.emplace_back(std::make_unique<Usb>(testPath.string(), "Volume1", 0, 0));
	ON_CALL(_monitor, usbs()).WillByDefault(testing::ReturnRef(usbs));
	_monitor.usbAdded(*usbs[0]);

	FileBrowser fileBrowser(canvas(), style, _monitor, L"Select File");
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	fileBrowser.setPath(testPath / L"Drive1", "");
	ASSERT_EQ(testPeer.selectFolder(L"Volume1"), 1);
	ASSERT_TRUE(testPeer.expandFolder(L"Volume1"));

	ASSERT_EQ(testPeer.selectFolder(L"dir1"), 1);
	testPeer.select();

	ASSERT_EQ(testPeer.getSelectedPathText(), L"Volume1:/TEMP/FileBrowserTest/dir1");
}

TEST_P(FileBrowserTest, Given_LockedUsbSelected_When_UsbUnclockes_CertificateSelected_Then_ImportEnabled)
{
	std::vector<std::unique_ptr<IUsb>> usbs;
	usbs.emplace_back(std::make_unique<Usb>(testPath.string(), "Volume1", 0, 0, true));
	ON_CALL(_monitor, usbs()).WillByDefault(testing::ReturnRef(usbs));
	_monitor.usbAdded(*usbs[0]);

	FileBrowser fileBrowser(canvas(), style, _monitor, L"Select File");
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	ASSERT_EQ(testPeer.selectFolder(L"Volume1"), 1);

	ASSERT_TRUE(testPeer.expandFolder(L"Volume1"));
	testPeer.unlockUsb(L"Volume1");

	ASSERT_EQ(testPeer.wrongFileNameVisibility(), true);
}

TEST_P(FileBrowserTest, Given_EditBox_When_PasswordEntered_Then_UnlocksUsb)
{
	std::vector<std::unique_ptr<IUsb>> usbs;
	usbs.emplace_back(std::make_unique<Usb>(testPath.string(), "Volume1", 0, 0, true));
	ON_CALL(_monitor, usbs()).WillByDefault(testing::ReturnRef(usbs));
	_monitor.usbAdded(*usbs[0]);

	FileBrowser fileBrowser(canvas(), style, _monitor, L"Select File");
	const TestPeer::FileBrowserPeer testPeer(fileBrowser);
	fileBrowser.visible = true;

	ASSERT_EQ(testPeer.selectFolder(L"Volume1"), 1);

	ASSERT_TRUE(testPeer.expandFolder(L"Volume1"));
	ASSERT_FALSE(testPeer.unlockUsb(L"password"));
	ASSERT_TRUE(testPeer.showHide());
	ASSERT_TRUE(testPeer.showHide());

	ASSERT_TRUE(testPeer.folderUp());
}

TEST_P(FileBrowserTest, Given_TwoFileBrowsers_When_OneBrowserOpenedAndBrowsed_Then_VerifyPathChangedNotCalledOnClosedBrowser)
{
	std::unique_ptr<FileBrowser> fileBrowser;
	initializeSecondFileBrowser(fileBrowser, false);

	this->peer->browse(L"dir_2\\dir21");
	this->peer->select();

	ASSERT_FALSE(selectedPath.empty());
	ASSERT_FALSE(triggered);
}

TEST_P(FileBrowserTest, Given_TwoFileBrowsers_When_BothBrowsersOpenedAndBrowsed_Then_VerifyPathChangedCalledOnBothBrowsers)
{
	std::unique_ptr<FileBrowser> fileBrowser;
	initializeSecondFileBrowser(fileBrowser, true);
	const TestPeer::FileBrowserPeer testPeer(*fileBrowser);

	testPeer.browse(L"dir_2\\dir21");
	testPeer.select();

	ASSERT_FALSE(selectedPath.empty());
	ASSERT_TRUE(triggered);
}

INSTANTIATE_TEST_CASE_P(test, FileBrowserTest, testing::Values(UiStyle::Integrated, UiStyle::Cart, UiStyle::Tsm));
