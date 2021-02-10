#include <Inputs.h>
#include <Context.h>
#include <iostream>
#include <Window.h>
#include <CommCtrl.h>
#include <wchar.h>

void WinGui::Input<std::string>::on_text_entered(LPARAM l_param) {
	int buffer_size = GetWindowTextLength((HWND)l_param);
	std::wstring buffer;
	buffer.resize(buffer_size);
	GetWindowText((HWND)l_param, (LPWSTR)buffer.c_str(), buffer_size + 1);
	text_ = wchar_to_string(buffer.c_str(), buffer_size);
}

WinGui::Input<std::string>::Input(const Window &parent,
                                  const std::string &name, const int &x, const int &y, const int &width,
                                  const int &height, const bool &v_scroll, const bool &h_scroll,
                                  const bool &is_password) :
	Control(parent, x, y, name), width_(width), height_(height) {

	/*if ((x < 0) || (y < 0) || (width <= 0) || (height <= 0))
		exc;*/

	/*if (!name.empty()) {
		add_label(name, x, y);
		height -= FONT_HEIGHT;
		y += FONT_HEIGHT;
	}*/

	DWORD dw_style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;

	if (v_scroll)
		dw_style |= WS_VSCROLL;

	if (h_scroll)
		dw_style |= WS_HSCROLL;

	if (is_password) { dw_style = WS_CHILD | WS_VISIBLE | ES_PASSWORD; }

	id_ = RICH_TEXT_INPUT | Context::get_rt_buffer_size();

	handle_ = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		nullptr,
		dw_style,
		x_,
		y_,
		width_,
		height_,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));

	Context::register_gui_object(this);
}

std::string WinGui::Input<std::string>::get_text() const { return text_; }


void WinGui::Input<float>::on_value_entered(LPARAM l_param) {
	int buffer_size = GetWindowTextLength((HWND)l_param);
	std::wstring buffer;
	std::string buffer_2;
	buffer.resize(buffer_size);
	GetWindowText((HWND)l_param, (LPWSTR)buffer.c_str(), buffer_size + 1);
	buffer_2 = wchar_to_string(buffer.c_str(), buffer_size);
	value_ = std::stof(buffer_2);
}

const wchar_t *WinGui::Input<float>::format_float_input(const wchar_t *source, int length) {

	wchar_t *padding_buffer;
	wchar_t *result;
	int no_minus_str_len = 0;
	int itr_1 = 0;
	int itr_2 = 0;
	bool was_separator = false;
	bool is_negative = false;
	bool only_zeros = true;
	std::wstring buffer;
	// 1. String preprocessing
	// 1.1 Return default string if source is empty

	// 1.1.2 Throw out minuses and raise a flag if there are any of them

	for (int i = 0; i < length; i++) {
		if (source[i] == L'-') {
			is_negative = true;
			continue;
		}

		if ((source[i] != L'0') && (source[i] != L'.') && (source[i] != L'\0'))
			only_zeros = false;

		buffer += source[i];
		no_minus_str_len++;
	}

	const wchar_t *no_minus_str = buffer.c_str();

	if (no_minus_str_len == 1 || only_zeros) {
		result = new wchar_t[4];

		result[0] = L'0';
		result[1] = L'.';
		result[2] = L'0';
		result[3] = L'\0';
		return result;
	}

	

	// 1.2. Filter redundant decimal separators - only first will be relevant

	wchar_t *no_mul_ds_str = new wchar_t[no_minus_str_len];
	int no_mul_ds_str_len = no_minus_str_len;

	while (no_minus_str[itr_1] != L'\0') {
		if (was_separator && no_minus_str[itr_1] == L'.') {
			itr_1++;
			no_mul_ds_str_len--;
			continue;
		}

		no_mul_ds_str[itr_2] = no_minus_str[itr_1];

		if (no_minus_str[itr_1] == L'.')
			was_separator = true;

		itr_1++;
		itr_2++;
	}

	no_mul_ds_str[itr_2] = L'\0';

	if (!was_separator) {
		result = new wchar_t[no_minus_str_len + 2];

		for (int i = 0; i < no_minus_str_len; i++)
			result[i] = no_minus_str[i];

		result[no_minus_str_len + 1] = L'\0';
		result[no_minus_str_len] = L'0';
		result[no_minus_str_len - 1] = L'.';

		delete [] no_mul_ds_str;
		return result;
	}

	// filtered_str contains string which is filtered out of redundant decimal separators
	// Amount of array elements is still the same like origin length, but \0 is placed earlier which trims the string
	// Real length (pointed by \0) is stored in filtered_str_len

	// 2. Padding front with 0 when the first char is "."

	result = no_mul_ds_str;

	if (no_mul_ds_str[0] == L'.' && no_mul_ds_str_len >= 2) {
		padding_buffer = new wchar_t[no_mul_ds_str_len + (int)1];
		padding_buffer[0] = L'0';
		padding_buffer[1] = L'.';

		for (itr_1 = 1, itr_2 = 2; itr_1 < no_mul_ds_str_len; itr_1++, itr_2++)
			padding_buffer[itr_2] = no_mul_ds_str[itr_1];

		delete [] no_mul_ds_str;
		no_mul_ds_str = padding_buffer;
		no_mul_ds_str_len++;
		result = no_mul_ds_str;
	}

	// 3. Padding back with 0 when the last char before \0 is "."

	if (no_mul_ds_str[no_mul_ds_str_len - 2] == L'.' && no_mul_ds_str_len >= 2) {
		padding_buffer = new wchar_t[no_mul_ds_str_len + (int)1];
		padding_buffer[no_mul_ds_str_len] = L'\0';
		padding_buffer[no_mul_ds_str_len - 1] = L'0';

		for (itr_1 = 0, itr_2 = 0; itr_1 < no_mul_ds_str_len - 1; itr_1++, itr_2++)
			padding_buffer[itr_2] = no_mul_ds_str[itr_1];

		delete [] no_mul_ds_str;
		result = padding_buffer;
	}
	
	if (is_negative && wcscmp(result, L"0.0")) {
		int n_length = wcslen(result) + 2;
		wchar_t *n_result = new wchar_t[n_length];

		n_result[0] = L'-';
		for (int i = 1; i < n_length; i++)
			n_result[i] = result[i - 1];

		delete [] result;
		return n_result;
	}

	return result;
}

LRESULT CALLBACK WinGui::Input<float>::input_proc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
	int buffer_length;
	wchar_t *buffer;
	const wchar_t *format_buffer;
	switch (message) {
	case WM_CHAR:
		if (((w_param < '0') || (w_param > '9')) && (w_param != '.') && (w_param != 8) && (w_param != '-'))
			return 0;
		break;
	case WM_KILLFOCUS:
		buffer_length = GetWindowTextLength(window_handle);
		buffer = new wchar_t[buffer_length + 1];
		GetWindowText(window_handle, (LPWSTR)buffer, buffer_length + 1);

		format_buffer = format_float_input(buffer, buffer_length + 1);

		if (format_buffer != nullptr)
			SendMessage(window_handle,WM_SETTEXT, 0, (LPARAM)format_buffer);

		delete [] buffer;
		delete [] format_buffer;
		break;
	default:
		break;
	}

	return CallWindowProc(std_edit_proc_, window_handle, message, w_param, l_param);

}

WNDPROC WinGui::Input<float>::std_edit_proc_ = nullptr;

WinGui::Input<float>::Input(const Window &parent, const std::string &name, const int &x, const int &y, const int &width)
	: Control(parent, x, y, name), width_(width), value_(0.0f) {

	id_ = FLOAT_INPUT | Context::get_fp_in_buffer_size();

	handle_ = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		nullptr,
		WS_CHILD | WS_VISIBLE | ES_LEFT,
		x_,
		y_,
		width_,
		19,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	SendMessage(handle_,WM_SETTEXT, 0, (LPARAM)L"0.0");

	if (std_edit_proc_ == nullptr)
		std_edit_proc_ = (WNDPROC)SetWindowLongPtr(handle_,GWLP_WNDPROC, (LONG_PTR)input_proc);

	Context::register_gui_object(this);
}

float WinGui::Input<float>::get_value() const { return value_; }


WinGui::Input<int>::Input(const Window &parent, const std::string &name, const int &x, const int &y, const int &width)
	: Control(parent, x, y, name), width_(width), value_(0) {

	id_ = INT_INPUT | Context::get_int_in_buffer_size();

	handle_ = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		nullptr,
		WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_LEFT,
		x_,
		y_,
		width_,
		19,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));

	HWND ud_control_handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		UPDOWN_CLASS,
		nullptr,
		WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY | UDS_SETBUDDYINT | UDS_ALIGNRIGHT,
		0,
		0,
		0,
		0,
		parent_handle_,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	SendMessage(ud_control_handle,UDM_SETRANGE, 0,MAKELPARAM(100, -100));


	Context::register_gui_object(this);
}

float WinGui::Input<int>::get_value() const { return value_; }

void WinGui::Input<int>::on_value_entered(LPARAM l_param) { }
