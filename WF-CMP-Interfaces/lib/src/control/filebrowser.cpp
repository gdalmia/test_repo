// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <Workflow/control/filebrowser.h>
#include <Workflow/control/toucheditbox.h>
#include "filebrowserview.h"

namespace Philips { namespace Workflow { namespace Control {
	using namespace Sense;
	using namespace Sense::DLS;

	struct FileBrowser::Private
	{
		public:
			Private(Sense::Control& parent) 
			{
				_keyboard = std::make_unique<VirtualKeyboard>(parent);
			};
			Private(Sense::Control& parent, const Media::IUsbMonitor& usbMonitor):_viewModel(usbMonitor)
			{
				_keyboard = std::make_unique<VirtualKeyboard>(parent);
				_keyboard->size = Size(_keyboard->size->width, _keyboard->size->height);
				_keyboard->scale = 0.81;
			};

			FileBrowserViewModel _viewModel;
			std::unique_ptr<VirtualKeyboard> _keyboard;
	};

	FileBrowser::FileBrowser(Sense::Control& parent, UiStyle style, const std::wstring& title, bool readOnly)
		: Control(parent)
		, _browserDialog(*this)
		, _style(style)
		, _pImpl(std::make_unique<Private>(*this))

	{
		this->size = parent.size;
		this->position = parent.position;
		updateVariables(style, title, readOnly);
	}

	FileBrowser::FileBrowser(Sense::Control& parent, UiStyle style, const Media::IUsbMonitor& usbMonitor, const std::wstring& title, bool readOnly)
		: Control(parent)
		, _browserDialog(*this)
		, _style(style)
		, _pImpl(std::make_unique<Private>(*this, usbMonitor))

	{
		this->size = parent.size;
		this->position = parent.position;
		updateVariables(style, title, readOnly);

		setPath(L"", "This System");
	}

	void FileBrowser::setPath(const std::filesystem::path &path, const std::string& volumeName) const
	{
		_pImpl->_viewModel.setPath(path,volumeName);
	}

	void FileBrowser::setPosition(const Sense::Point& position_)
	{
		_browserDialog.position = position_;
	}

	void FileBrowser::setAllowedFileTypes(const std::vector <std::string>& fileType)
	{
		_pImpl->_viewModel.setAllowedFileTypes(fileType);
	}

	void FileBrowser::subscribe()  
	{
		_pImpl->_viewModel.selected.subscribe([this](const std::filesystem::path &path)
		{
			if (this->visible)
			{
				pathSelected(path);
				this->visible = false;
			}
		});
	}

	void FileBrowser::updateVariables(UiStyle style, const std::wstring& title, bool readOnly)
	{
		updateUi(style, title);
		subscribe();

		this->visible = false;
		this->_browserDialog.headerSize = 0;

		_pImpl->_viewModel.readOnly = readOnly;

		this->modal = true;
	}

	void FileBrowser::updateUi(UiStyle style, const std::wstring& title)
	{
		if (style == UiStyle::Integrated)
		{
			_browser = std::make_unique<FileBrowserIntegrated>(_browserDialog, _pImpl->_viewModel, *(_pImpl->_keyboard), title);
			_browserDialog.size = Size(910, 866);
			_browserDialog.position = Point(200, 30);
		}
		else
		{
			_browser = std::make_unique<FileBrowserTouch>(_browserDialog, _pImpl->_viewModel, *(_pImpl->_keyboard), title);
			_browserDialog.size = Size(700, 660);
			_browserDialog.position = Point(290, 30);
		}
	}

	void FileBrowser::render(Sense::IRenderer &renderer)  const
	{
		const auto transparency = 0.8;
		const auto backgroundColor = Color::fromRgb(3, 3, 3) * transparency;
		//render a semi-transparent black background
		const auto space = getParentSpace();
		renderer.render(space, space.bounds, backgroundColor);

		Control::render(renderer);
	}

	FileBrowser::~FileBrowser() = default;
}}}