#include "ButtonR.hpp"

ButtonR::ButtonR()
{
}

ButtonR::ButtonR(string text, float x, float y)
{
    this->text = text;
    rect.x = x;
    rect.y = y;

    buildBtn();
}

ButtonR::ButtonR(string text, float x, float y, float fontSize)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->fontSize = fontSize;

    buildBtn();
}

ButtonR::ButtonR(string text, float x, float y, float fontSize, Color fontColor, Color btnColor)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->fontSize = fontSize;
    this->fontColor = fontColor;
    btnIdle = btnColor;

    buildBtn();
}

//--------------------------------------------------------------------- Constructor-4
/*---------------------------------------------------

    constructor-4
    - raylib default font
    text, position, font size, font color,
    button color, border color

 ----------------------------------------------------*/
ButtonR::ButtonR(string text, float x, float y, float fontSize, Color fontColor, Color btnColor, Color borderColor)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->fontSize = fontSize;
    this->fontColor = fontColor;
    btnIdle = btnColor;
    borderIdle = borderColor;

    buildBtn();
}

//--------------------------------------------------------------------- Constructor-5
/*---------------------------------------------------

    constructor-5
    - loaded font
    text, position, font

 ----------------------------------------------------*/
ButtonR::ButtonR(string text, float x, float y, Font &font)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->font = font;

    isRayFont = false;

    buildBtn();
}

//--------------------------------------------------------------------- Constructor-6
/*---------------------------------------------------

    constructor-6
    - loaded font
    text, position, font, font size

 ----------------------------------------------------*/
ButtonR::ButtonR(string text, float x, float y, Font &font, float fontSize)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->font = font;
    this->fontSize = fontSize;

    isRayFont = false;

    buildBtn();
}

//--------------------------------------------------------------------- Constructor-7
/*---------------------------------------------------

    constructor-7
    - loaded font
    text, position, font, font size, font color,
    button color

 ----------------------------------------------------*/
ButtonR::ButtonR(string text, float x, float y, Font &font, float fontSize, Color fontColor, Color btnColor)
{
    this->text = text;
    rect.x = x;
    rect.y = y;
    this->font = font;
    this->fontSize = fontSize;
    this->fontColor = fontColor;
    btnIdle = btnColor;

    isRayFont = false;

    buildBtn();
}

ButtonR::ButtonR(string text, float x, float y, Font &font, float fontSize, Color fontColor, Color btnColor, Color borderColor)
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

    buildBtn();
}

ButtonR::ButtonR(string text, Font &font, float fontSize)
{
    this->text = text;
    this->font = font;
    this->fontSize = fontSize;

    isRayFont = false;

    buildBtn();
}
void ButtonR::draw()
{
    update();

    if (isShadow)
        DrawRectangleRec(shadow, shadowColor);
    DrawRectangleRec(rect, btnLiveColor);
    if (isBorder)
        DrawRectangleLinesEx(border, borderThickness, borderLiveColor);
    DrawTextEx(font, text.c_str(), textPos, fontSize, fontSpacing, fontColor);
}

int ButtonR::update()
{
    int result = -1;
    // Mouse position
    Vector2 mouse_pos = GetMousePosition();

    if (CheckCollisionPointRec(mouse_pos, rect))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            btnLiveColor = btnIdle;
            borderLiveColor = borderIdle;
            result = MOUSE_BUTTON_LEFT;
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            btnLiveColor = btnPressed;
            borderLiveColor = borderPressed;
            result = MOUSE_BUTTON_RIGHT;
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            btnLiveColor = btnPressed;
            borderLiveColor = borderPressed;
            result = MOUSE_BUTTON_MIDDLE;
        }
        else
        {
            btnLiveColor = btnHover;
            borderLiveColor = borderHover;
        }
    }
    else
    {
        btnLiveColor = btnIdle;
        borderLiveColor = borderIdle;
    }

    return result;
}
void ButtonR::buildBtn()
{
    float ratioWidth = (isRayFont) ? 3.5f : 4.0f,
          ratioHeight = (isRayFont) ? 2.0f : 1.4f;
    textSize = MeasureTextEx(font, text.c_str(), fontSize, fontSpacing);
    oneCharSize = MeasureTextEx(font, "C", fontSize, fontSpacing);

    if (txtResizeBtn)
    {
        //--- Button size
        rect.width = (textSize.x + ratioWidth * oneCharSize.x);
        rect.height = textSize.y * (float)(ratioHeight * (textSize.y / fontSize));
    }
    textPos =
        {
            rect.x + (rect.width - textSize.x) / 2,
            rect.y + (rect.height - textSize.y) / 2};
    border = rect;
    shadow = {rect.x + 5, rect.y + 5, rect.width, rect.height};
}
void ButtonR::setFont(Font font)
{
    this->font = font;
    txtResizeBtn = true;
    isRayFont = false;
    buildBtn();
}

void ButtonR::setFontSize(float fontSize)
{
    this->fontSize = fontSize;
    txtResizeBtn = true;
    buildBtn();
}

void ButtonR::setFontSizeNoResize(float fontSize)
{
    this->fontSize = fontSize;
    txtResizeBtn = false;
    buildBtn();
}

void ButtonR::setText(string text)
{
    this->text = text;
    txtResizeBtn = true;
    buildBtn();
}

void ButtonR::setTextNoResize(string text)
{
    this->text = text;
    txtResizeBtn = false;
    buildBtn();
}

void ButtonR::setTextPosition(float x, float y)
{
    textPos.x = x;
    textPos.y = y;
}

void ButtonR::setBtnPosition(float btnX, float btnY)
{
    rect.x = btnX;
    rect.y = btnY;
    buildBtn();
}

void ButtonR::setBtnSize(float btnWidth, float btnHeight)
{
    rect.width = btnWidth;
    rect.height = btnHeight;
    txtResizeBtn = false;
    buildBtn();
}

void ButtonR::setShadowPos(float x, float y)
{
    shadow = {x, y, shadow.width, shadow.height};
}

void ButtonR::setShadowSize(float width, float height)
{
    shadow = {shadow.x, shadow.y, width, height};
}

bool ButtonR::isHover()
{
    Vector2 mouse_pos = GetMousePosition();
    if (CheckCollisionPointRec(mouse_pos, rect))
        return true;
    else
        return false;
}
