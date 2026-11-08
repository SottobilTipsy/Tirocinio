#ifndef NBOX_H
#define NBOX_H

#include "Button.hpp"
#include "Checkbox.hpp"

class Nbox
{
	private:
		int n;
		int outer_square_size;
		bool inside_inner_rect;
    	bool MouseOnText;
		Font ConfirmFont;
		Button ConfirmButton;
		Checkbox Ncheckbox;

	public:
		Nbox(int,int,bool,bool,Font);
		Checkbox get_checkbox();
		void Draw(Font);
		Button get_button();
};
#endif