// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "src/control/filebrowserviewmodel.h"
#include <gtest/gtest.h>
#include "Workflow/Media/usbmonitor.h"
#include "Mock/usbMonitor.mock.h"
#include <fstream>

using namespace Philips::Workflow::Control;

class FileBrowserViewModelTest : public testing::Test
{
	public:

	void SetUp() override
	{
		std::filesystem::path testDirectory = std::filesystem::temp_directory_path() / L"FileBrowserTest";
		std::filesystem::create_directory(testDirectory);
		std::filesystem::create_directories(testDirectory / L"dir1" / L"dir11");
		std::filesystem::create_directories(testDirectory / L"dir2" / L"dir21");
		std::filesystem::create_directories(testDirectory / L"dir3" / L"dir31");
		testPath = std::filesystem::temp_directory_path() / L"FileBrowserTest";
		viewModel.setPath(testPath,"Test");	
	}

	void TearDown() override
	{
		std::filesystem::remove_all(std::filesystem::temp_directory_path() / L"FileBrowserTest");
	}

	public:
		FileBrowserViewModel viewModel;
		std::filesystem::path testPath;
};

TEST_F(FileBrowserViewModelTest, WhenPathIsSetThenDirectoryListIsUpdate)
{
    std::vector<std::filesystem::path> dirList;
	dirList.emplace_back(testPath / L"dir1");
	dirList.emplace_back(testPath / L"dir2");
	dirList.emplace_back(testPath / L"dir3");

	std::vector<std::filesystem::path> viewModelDir = viewModel.directoryList();
	ASSERT_EQ(viewModelDir, dirList);
}

TEST_F(FileBrowserViewModelTest, WhenFolderPathIsGivenThenFolderIsSelected)
{
	viewModel.setPath(testPath, "");
	const std::filesystem::path folderPath = testPath / L"dir3";	
	int index = 0;
	for (const auto& dir : viewModel.directoryList())
	{
		if (folderPath == dir)
		{
			viewModel.updateIndex(index);
			break;
		}
		index++;
	}
	const std::filesystem::path selectedPath = viewModel.selectedPath();
	ASSERT_EQ(folderPath, selectedPath);
	ASSERT_EQ(index, viewModel.selectedIndex());
}

TEST_F(FileBrowserViewModelTest, WhenValidNewFolderNameIsGivenThenNewFolderIsCreated)
{
	viewModel.addFolder();
	FileBrowserViewModel::View currentView = FileBrowserViewModel::View::AddFolder;
	ASSERT_EQ(viewModel.view(), currentView);

	viewModel.setNewFolderName(L"dir4");
	ASSERT_EQ(viewModel.newFolderName(), L"dir4");
	viewModel.createNewFolder();
	std::vector<std::filesystem::path> dirList;
	dirList.emplace_back(testPath / L"dir1");
	dirList.emplace_back(testPath / L"dir2");
	dirList.emplace_back(testPath / L"dir3");
	dirList.emplace_back(testPath / L"dir4");

	std::vector<std::filesystem::path> viewModelDir = viewModel.directoryList();
	ASSERT_EQ(viewModelDir, dirList);
}

TEST_F(FileBrowserViewModelTest, WhenInvalidNewFolderNameIsGivenThenFolderNameStatusUpdates)
{
	viewModel.addFolder();
	viewModel.setNewFolderName(L"COM4");
	viewModel.createNewFolder();
	FileBrowserViewModel::FolderName folderNameStatus = FileBrowserViewModel::FolderName::Invalid;
	ASSERT_EQ(folderNameStatus, viewModel.folderNameState());
}

TEST_F(FileBrowserViewModelTest, WhenFolderPathLengthLimitIsReachedThenFolderNameStatusUpdates)
{
	viewModel.addFolder();
	viewModel.setNewFolderName(L"2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222");
	viewModel.createNewFolder();
	FileBrowserViewModel::FolderName folderNameStatus = FileBrowserViewModel::FolderName::invalidFolderPathLength;
	ASSERT_EQ(folderNameStatus, viewModel.folderNameState());
}

TEST_F(FileBrowserViewModelTest, WhenUnsupportedCharacterFolderNameIsGivenThenFolderNameStatusUpdates)
{
	viewModel.addFolder();
	viewModel.setNewFolderName(L"dir?4");
	viewModel.createNewFolder();
	FileBrowserViewModel::FolderName folderNameStatus = FileBrowserViewModel::FolderName::UnsupportedCharacters;
	ASSERT_EQ(folderNameStatus, viewModel.folderNameState());
}

TEST_F(FileBrowserViewModelTest, WhenDuplicateFolderNameIsGivenThenFolderNameStatusUpdates)
{
	viewModel.addFolder();
	viewModel.setNewFolderName(L"dir2 ");
	viewModel.createNewFolder();
	FileBrowserViewModel::FolderName folderNameStatus = FileBrowserViewModel::FolderName::Duplicate;
	ASSERT_EQ(folderNameStatus, viewModel.folderNameState());
}

TEST_F(FileBrowserViewModelTest, WhenSelectIsClickedThenSelectedPathIsReturned)
{
	viewModel.setPath(testPath, "");
	const std::filesystem::path folderPath = testPath / L"dir3";
	int index = 0;
	for (const auto& dir : viewModel.directoryList())
	{
		if (folderPath == dir)
		{
			viewModel.updateIndex(index);
			break;
		}
		index++;
	}

	viewModel.select();
	const std::filesystem::path selectedPath = viewModel.selectedPath();
	ASSERT_EQ(folderPath, selectedPath);
}

TEST_F(FileBrowserViewModelTest, Given_NoVolumeName_When_SelectedPathIsCalled_Then_PathWithoutVolumeNameIsReturned)
{
	const auto volumeName = "";
	viewModel.setPath(testPath,volumeName);

	ASSERT_EQ(testPath, std::filesystem::path(viewModel.selectedPath()));
}
TEST_F(FileBrowserViewModelTest, Given_VolumeName_When_SelectedPathIsCalled_Then_RootDirectoryNameIsReplacedWithVolumeName)
{
	const auto volumeName = "USB";
	const std::string path = "C:\\TEMP";
	viewModel.setPath(path, volumeName);

	ASSERT_EQ(L"USB:/TEMP",viewModel.selectedPath());
}

TEST_F(FileBrowserViewModelTest, Given_InvalidPathAndValidVolumeName_When_SelectedPathIsCalled_Then_InvalidPathIsReturned)
{
	const auto volumeName = "USB";
	const std::string path = "ABC";
	viewModel.setPath(path, volumeName);

	ASSERT_EQ(L"ABC", viewModel.selectedPath());
}


TEST_F(FileBrowserViewModelTest, WhenCancelIsClickedThenBrowserModelStateIsRetained)
{
	viewModel.cancel();
	ASSERT_EQ(viewModel.visibleState(), true);
}

TEST_F(FileBrowserViewModelTest, WhenOpenFolderThenDirectoryListIsUpdated)
{
	std::vector<std::filesystem::path> dirList;
	dirList.emplace_back(testPath / L"dir2" / L"dir21");
	viewModel.updateIndex(1);
	viewModel.openFolder();
	ASSERT_EQ(dirList, viewModel.directoryList());
}

TEST_F(FileBrowserViewModelTest, WhenFolderUpThenCurrentPathIsUpdated)
{
	viewModel.setPath(testPath, "");
	viewModel.updateIndex(1);
	viewModel.openFolder();
	viewModel.folderUp();
	const std::filesystem::path selectedPath = viewModel.selectedPath();
	ASSERT_EQ(selectedPath, testPath);
	ASSERT_EQ(viewModel.folderUpState(), 0);
}

TEST_F(FileBrowserViewModelTest, WhenBackClickedThenDirectoryViewAppears)
{
	viewModel.addFolder();
	FileBrowserViewModel::View view = FileBrowserViewModel::View::AddFolder;
	ASSERT_EQ(viewModel.view(), view);
	viewModel.folderUp();
	view = FileBrowserViewModel::View::Directory;
	ASSERT_EQ(viewModel.view(), view);
}

class FileBrowserViewModelTestForUsb : public testing::Test
{
public:

	void SetUp() override
	{
		//Create Usb list 
		usbs = createUsbList();
		ON_CALL(_monitor, usbs()).WillByDefault(testing::ReturnRef(usbs));

		_viewModel.reset(new FileBrowserViewModel(_monitor));

		std::filesystem::path testDirectory  = std::filesystem::temp_directory_path() / L"Drive1";
		std::filesystem::path testDirectory2 = std::filesystem::temp_directory_path() / L"Drive2";
		std::filesystem::create_directory(testDirectory);
		std::filesystem::create_directory(testDirectory2);
		std::filesystem::create_directories(testDirectory / L"dir1" / L"dir11");
		std::filesystem::create_directories(testDirectory / L"dir2" / L"dir21");
		std::filesystem::create_directories(testDirectory2 / L"dir3" / L"dir31");
		testPath = std::filesystem::temp_directory_path();
	}

	void TearDown() override
	{
		std::filesystem::remove_all(std::filesystem::temp_directory_path() / L"Drive1");
		std::filesystem::remove_all(std::filesystem::temp_directory_path() / L"Drive2");
	}

	static std::vector<std::unique_ptr<IUsb>> createUsbList()
	{
		std::vector<std::unique_ptr<IUsb>> usbs;
		usbs.emplace_back(std::make_unique<Usb>("Drive1"));
		usbs.emplace_back(std::make_unique<Usb>("Drive2"));
		return usbs;
	}

	static std::vector<std::unique_ptr<IUsb>> createLockedUsbList()
	{
		std::vector<std::unique_ptr<IUsb>> usbs;
		usbs.emplace_back(std::make_unique<Usb>("Drive1", "Volume1", 0, 0, true));
		usbs.emplace_back(std::make_unique<Usb>("Drive2", "Volume2", 0, 0, true));
		return usbs;
	}

public:
	std::unique_ptr<FileBrowserViewModel> _viewModel;
	std::filesystem::path testPath;
	MockUsbMonitor									_monitor;
	std::vector<std::unique_ptr<IUsb>> usbs;
};

TEST_F(FileBrowserViewModelTestForUsb, WhenPathIsSetThenDirectoryListIsUpdate)
{
	std::vector<std::filesystem::path> dirList;
	dirList.emplace_back(testPath / _monitor.usbs()[0]->drive() / L"dir1");
	dirList.emplace_back(testPath / _monitor.usbs()[0]->drive() / L"dir2");

	_viewModel->setPath(testPath / _monitor.usbs()[0]->drive(), "This System");
	std::vector<std::filesystem::path> viewModelDir = _viewModel->directoryList();
	ASSERT_EQ(viewModelDir, dirList);
}

TEST_F(FileBrowserViewModelTestForUsb, WhenFolderPathIsGivenThenFolderIsSelected)
{
	_viewModel->setPath(testPath / _monitor.usbs()[1]->drive() /  L"dir3", "");

	const std::filesystem::path folderPath = testPath / _monitor.usbs()[1]->drive() / L"dir3";
	int index = 0;
	for (const auto& dir : _viewModel->directoryList())
	{
		if (folderPath == dir)
		{
			_viewModel->updateIndex(index);
			break;
		}
		index++;
	}
	const std::filesystem::path selectedPath = _viewModel->selectedPath();
	ASSERT_EQ(folderPath, selectedPath);
}

TEST_F(FileBrowserViewModelTestForUsb, WhenSelectIsClickedThenSelectedPathIsReturned)
{
	_viewModel->setPath(testPath / _monitor.usbs()[1]->drive() / L"dir3", "");

	const std::filesystem::path folderPath = testPath / _monitor.usbs()[1]->drive() / L"dir3";
	int index = 0;
	for (const auto& dir : _viewModel->directoryList())
	{
		if (folderPath == dir)
		{
			_viewModel->updateIndex(index);
			break;
		}
		index++;
	}

	_viewModel->select();
	const std::filesystem::path selectedPath = _viewModel->selectedPath();
	ASSERT_EQ(folderPath, selectedPath);
}

TEST_F(FileBrowserViewModelTestForUsb, Given_UsbsListed_When_UsbIsSelected_Then_OpenFolder)
{
	_viewModel->setPath(testPath / _monitor.usbs()[1]->drive(), "This System");

	_viewModel->updateIndex(0);
	ASSERT_EQ(FileBrowserViewModel::View::UsbList, _viewModel->view());

	_viewModel->openSelection();
	ASSERT_EQ(_monitor.usbs()[0]->drive(), std::filesystem::path(_viewModel->selectedPath()));

	std::vector<std::filesystem::path> dirList;
	_viewModel->setPath(testPath / _monitor.usbs()[0]->drive(), "This System");
	dirList.emplace_back(testPath / _monitor.usbs()[0]->drive() / L"dir2" / L"dir21");
	_viewModel->updateIndex(1);
	_viewModel->openFolder();
	ASSERT_EQ(dirList, _viewModel->directoryList());

	ASSERT_EQ(1, _viewModel->folderUpState());
	ASSERT_EQ(FileBrowserViewModel::View::Directory, _viewModel->view());
}

TEST_F(FileBrowserViewModelTestForUsb, Usb_Given_UsbsListed_When_UsbIsSelected_Then_PathWithoutVolumeNameIsReturned)
{
	//Add 1st usb
	_monitor.usbAdded(*usbs[0]);
	_monitor.usbAdded(*usbs[1]);

	_viewModel->setPath(testPath / _monitor.usbs()[1]->drive(), "This System");

	_viewModel->updateIndex(1);
	_viewModel->openSelection();
	ASSERT_EQ(_monitor.usbs()[1]->drive(), std::filesystem::path(_viewModel->selectedPath()));

	_viewModel->folderUp();
	ASSERT_EQ(0, _viewModel->folderUpState());
}

TEST_F(FileBrowserViewModelTestForUsb, Given_UsbsListed_When_UsbIsSelected_Then_ValidCertificatesDisplayed)
{
	//Add 1st usb
	_monitor.usbAdded(*usbs[0]);

	_viewModel->setPath(testPath / _monitor.usbs()[1]->drive(), "This System");

	const auto filePath = testPath / _monitor.usbs()[1]->drive() / "Certificate.p7b";
	std::ofstream nodeFile(filePath);

	std::vector <std::string> fileType;
	fileType.emplace_back(".p7b");
	_viewModel->setAllowedFileTypes(fileType);

	_viewModel->updateIndex(1);
	_viewModel->openSelection();
	_viewModel->setPath(testPath / _monitor.usbs()[1]->drive(), "");
	ASSERT_EQ(testPath / _monitor.usbs()[1]->drive(), std::filesystem::path(_viewModel->selectedPath()));

	_monitor.usbRemoved(*usbs[0]);

	ASSERT_EQ(0, _viewModel->folderUpState());
}

TEST_F(FileBrowserViewModelTestForUsb, Given_UsbIsSelected_When_FolderUp_Then_PathWithoutVolumeNameIsReturned)
{
	_viewModel->setPath(L"", "This System");

	_viewModel->updateIndex(0);
	_viewModel->openSelection();
	_viewModel->folderUp();

	ASSERT_EQ("Drive1", std::filesystem::path(_viewModel->selectedPath()));
}

TEST_F(FileBrowserViewModelTestForUsb, Given_LockedUsbIsOpened_When_EnteredInValidUsbPassword_Then_UsbUnlockstatu)
{
	//Create Usb list 
	usbs = createLockedUsbList();
	MockUsbMonitor	usbMonitor;
	ON_CALL(usbMonitor, usbs()).WillByDefault(testing::ReturnRef(usbs));
	usbMonitor.usbAdded(*usbs[0]);

	_viewModel.reset(new FileBrowserViewModel(usbMonitor));
	usbs[0]->unlock("Password");

	_viewModel->openSelection();
	
	ASSERT_EQ(FileBrowserViewModel::UsbUnlock::UnKnown, _viewModel->unlockStatus());
	ASSERT_EQ(0, _viewModel->folderUpState());
}
