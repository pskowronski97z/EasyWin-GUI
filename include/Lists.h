#pragma once
#include <Control.h>
#include <vector>

namespace WinGUI {

	// Defined in Window.cpp
	std::wstring string_to_wstring(std::string source);
	
	class List : public Control {

	public:
		virtual bool add_item(std::string item) const = 0;
		virtual bool remove_item(int index) const = 0;
		virtual int get_selected_index() const = 0;
		virtual void clear() const = 0;
		virtual ~List() = default;
		List(const List &list) = default;
		List(List &&list) = delete;
		List &operator=(const List &) = delete;
		List &operator=(List &&) = delete;
		List(const Window &parent, const int &x, const int &y, std::string name);
	};

	class ComboBox : public List {
		int width_;
		int list_height_;
	public:
		ComboBox(const Window &parent, const std::string &name, const int &x, const int &y, const int &width,  const int &list_height);
		bool add_item(std::string item) const override;
		bool remove_item(int index) const override;
		int get_selected_index() const override;
		void clear() const override;
		
	};

	class ListBox : public List {
		int width_;
		int height_;
	public:
		ListBox(const Window &parent, const std::string &name, const int &x, const int &y, const int &width,  const int &height, bool multiple_selection);
		bool add_item(std::string item) const override;
		bool remove_item(int index) const override;
		// For single selection
		int get_selected_index() const override;
		void clear() const override;
		// For multiple selection
		std::vector<int> get_selected_indexes() const;
	};

	class TreeView : public Control {

		// TODO
	};
}