
#ifndef BUTTON_O_HPP
#define BUTTON_O_HPP

#ifdef _MSC_VER
#pragma once
#endif  // _MSC_VER

#include "ButtonR.hpp"

class ButtonO : public ButtonR 
{

public:
    float roundness = 4;
    ButtonO();
    ButtonO(string text, float x, float y);
    ButtonO(string text, float x, float y, float fontSize);
    ButtonO(string text, float x, float y, float fontSize, Color fontColor, Color btnColor);
    ButtonO(string text, float x, float y, float fontSize, Color fontColor, Color btnColor, Color borderColor);
    ButtonO(string text, float x, float y, Font &font);
    ButtonO(string text, float x, float y, Font &font, float fontSize,float roundness);
    ButtonO(string text, float x, float y, Font &font, float fontSize, Color fontColor, Color btnColor);
    ButtonO(string text, float x, float y, Font &font, float fontSize, Color fontColor, Color btnColor, Color borderColor);
    ButtonO(string text, Font &font, float fontSize);
    void draw();
private:

};

#endif
