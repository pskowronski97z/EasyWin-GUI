#include <StaticControls.h>
#include <CommCtrl.h>
#include <Window.h>



WinGUI::Label::Label(const Window& parent, const int& x, const int& y, std::string name) : Control(parent, x, y, name) {

	std::wstring w_name = string_to_wstring(name_);
	id_= STATIC_CTRL;
	width_ = w_name.size() * 6;
	handle_ = CreateWindowExW(
		0,
		L"STATIC",
		w_name.c_str(),
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		x_,
		y_,
		width_,
		FONT_HEIGHT,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		0);

	SendMessage(handle_, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	
}

void WinGUI::Label::set_name(std::string name) {
	name_ = name;
	std::wstring w_name = string_to_wstring(name_);
	SetWindowText(handle_,w_name.c_str());
	width_ = w_name.size()*6;
}

int WinGUI::Label::get_width() const { return width_; }



WinGUI::ProgressBar::ProgressBar(const Window& parent, const int& x, const int& y, const int& width, const int& height, std::string name)
	: Control(parent,x,y,name),width_(width),height_(height),progress_(0.0f) {

	id_ = STATIC_CTRL;

	if(!name.empty()) {
		Label name_label(parent,x_,y_,name);
		y_ += FONT_HEIGHT;
	}
	
	handle_ = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		PROGRESS_CLASS,
		0,
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH | PBS_SMOOTHREVERSE, 
		x_,
		y_, 
		width_,
		height_,
		parent_handle_,
		(HMENU)id_, 
		parent.get_instance(), 
		0);
}

int WinGUI::ProgressBar::get_width() const { return width_; }

int WinGUI::ProgressBar::get_height() const { return height_; }

float WinGUI::ProgressBar::get_progress() const { return progress_; }

bool WinGUI::ProgressBar::set_progress(unsigned short progress) {

	if(progress<0 || progress>100)
		return false;
	progress_ = progress;
	SendMessage(handle_, PBM_SETPOS, (WPARAM)progress_,0 );

	return true;
}



WinGUI::GroupBox::GroupBox(const Window& parent, const int& x, const int& y, const int& width, const int& height, std::string name)
	: Control(parent, x, y, name), width_(width), height_(height) {

	std::wstring w_name = string_to_wstring(name_);
	id_ = STATIC_CTRL;
	handle_ = CreateWindowExW(
			0,
			L"BUTTON",
			w_name.c_str(),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
			x_,
			y_,
			width_,
			height_,
			parent_handle_,
			(HMENU)id_,
			parent.get_instance(),
			nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
}

int WinGUI::GroupBox::get_width() const { return width_; }

int WinGUI::GroupBox::get_height() const { return height_; }
