#include <raylib.h>
#include "Nbox.hpp"
#include "Button.hpp"
#include "Checkbox.hpp"

Color outer_box_gray = {50,50,50,255};

Nbox::Nbox (int n_, int outer_square_size_, bool inside_inner_rect_, bool MouseOnText_, 
    Font  ConfirmFont_)
:   n(n_),
    outer_square_size(outer_square_size_),
    inside_inner_rect(inside_inner_rect_),
    MouseOnText(MouseOnText_),
    ConfirmFont(ConfirmFont_),
    ConfirmButton(Button({720, 570, MeasureTextEx(ConfirmFont_, "Confirm", 95, 1).x, MeasureTextEx(ConfirmFont_, "Confirm", 95, 1).y}, {720, 570})),
    Ncheckbox(Checkbox({720, 410, 360, 130}))
{}

Checkbox Nbox::get_checkbox() {return Ncheckbox;}

void Nbox::Draw (Font ConfirmFont)
{
    DrawRectangle(700,300,outer_square_size,outer_square_size,outer_box_gray);
    Rectangle outerborderrect = {690,290,(float)outer_square_size+20,(float)outer_square_size+20};
    DrawRectangleLinesEx(outerborderrect,10,{255,0,255,255});
    DrawText("Choose knot grid size",750,310,28,WHITE);
    DrawText("(Max is 500 )",815,350,28,WHITE);
    ConfirmButton.DrawAsFont(ConfirmFont,"Confirm?",{710,565},100,WHITE);
    Ncheckbox.Handler();
    Ncheckbox.Draw();
}		

Button Nbox::get_button()
{
    return ConfirmButton;
}



