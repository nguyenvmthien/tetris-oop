
#include "ButtonO.hpp"


ButtonO::ButtonO()
{
   
}


ButtonO::ButtonO(string text, float x, float y)
{
    this->text = text;
    rect.x = x;
    rect.y = y;

    ButtonR::buildBtn();
}

ButtonO::ButtonO(string text, float x, float y, float fontSize)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->fontSize = fontSize;
    
    ButtonR::buildBtn();
}

ButtonO::ButtonO(string text, float x, float y, float fontSize, Color fontColor, Color btnColor)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->fontSize = fontSize;
    this->fontColor = fontColor;
    btnIdle = btnColor;

    ButtonR::buildBtn();
}

ButtonO::ButtonO(string text, float x, float y, float fontSize, Color font_color, Color btnColor, Color borderColor)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->fontSize = fontSize;
    this->fontColor = font_color;
    btnIdle = btnColor;
    borderIdle = borderColor;

    ButtonR::buildBtn();
}

ButtonO::ButtonO(string text, float x, float y, Font &font)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->font = font;

    isRayFont = false;

    ButtonR::buildBtn();
}

ButtonO::ButtonO(string text, float x, float y, Font &font, float fontSize,float roundness)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->font = font;
    this->fontSize = fontSize;
    this->roundness = roundness;
    isRayFont = false;

    ButtonR::buildBtn();
}
ButtonO::ButtonO(string text, float x, float y, Font &font, float fontSize, Color fontColor, Color btnColor)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->font = font;
    this->fontSize = fontSize;
    this->fontColor = fontColor;
    btnIdle = btnColor;

    isRayFont = false;

    ButtonR::buildBtn();
}

ButtonO::ButtonO(string text, float x, float y, Font &font, float fontSize, Color fontColor, Color btnColor, Color borderColor)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->font = font;
    this->fontSize = fontSize;
    this->fontColor = fontColor;
    btnIdle = btnColor;
    borderIdle = borderColor;

    isRayFont = false;

    ButtonR::buildBtn();
}

ButtonO::ButtonO(string text, Font &font, float fontSize)
{
    this->text = text;
    this->font = font;
    this->fontSize = fontSize;

    isRayFont = false;

    buildBtn();
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

// Accessors Methods
//----------------------------------------------------------------------------------


//--------------------------------------------------------------------- Function draw()
/*----------------------------------------------------

    Draws button

 -----------------------------------------------------*/
void ButtonO::draw()
{
    ButtonR::update();

    if (isShadow) DrawRectangleRounded(shadow, roundness, 4, shadowColor);
    DrawRectangleRounded(rect, roundness, 1, btnLiveColor);
    if (isBorder)  DrawRectangleRoundedLines(border, roundness, 4, borderThickness, borderLiveColor);
    DrawTextEx(font, text.c_str(), textPos, fontSize, fontSpacing, fontColor);
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

// Mutators Methods
//---------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

// Class Methods Operations (private)
//---------------------------------------------------------------------------------



