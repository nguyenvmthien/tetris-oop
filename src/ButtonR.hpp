
#ifndef BUTTON_R_HPP
#define BUTTON_R_HPP

#ifdef _MSC_VER
#pragma once
#endif  // _MSC_VER

#include "raylib.h"

#include <iostream>	
#include <string>
#include <vector>

using namespace std;
class ButtonR
{

public:
    Font font = GetFontDefault();
    float fontSize = 60,
          fontSpacing = 3.0f;
    Color fontColor = BLACK;
    bool isRayFont= true;
    string text = "Button";
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, fontSpacing),
            oneCharSize = MeasureTextEx(font, "C", fontSize, fontSpacing);
    bool txtResizeBtn = true;
    float btnWidth = (textSize.x + 3.5f * oneCharSize.x),
          btnHeight = textSize.y * (float)(2.0f * (textSize.y / fontSize));
    Rectangle rect{ 100, 100, btnWidth, btnHeight };
    // colors
    Color btnLiveColor = { 222, 214, 202, 255 },
    // Botton state
          btnHover = { 135, 195, 74, 100 },
          btnPressed = { 66, 165, 245, 100 },
          btnIdle = btnLiveColor; // idle state
    Rectangle border{ rect.x, rect.y, btnWidth, btnHeight };
    // color
    Color borderLiveColor = BLACK,
    // Botton state
          borderHover = { 57, 73, 171, 100 },
          borderPressed = { 0, 105, 92, 100 },
          borderIdle = borderLiveColor; 
    float borderThickness = (rect.width + rect.height) / 150;
    bool isBorder = true,
         isBorderResized = false;
    double shadowOffset = 0.05;
    Rectangle shadow{ rect.x + 5,  rect.y + 5, btnWidth, btnHeight };
    Color shadowColor = GRAY;
    bool isShadow = true;

    //--- Centers text in button
    Vector2 textPos =
    {
        rect.x + (rect.width - textSize.x) / 2,
        rect.y + (rect.height - textSize.y) / 2
    };
    ButtonR();
    ButtonR(string text, float x, float y);
    ButtonR(string text, float x, float y, float fontSize);
    ButtonR(string text, float x, float y, float fontSize, Color fontColor, Color btnColor);
    ButtonR(string text, float x, float y, float fontSize, Color fontColor, Color btnColor, Color borderColor);
    ButtonR(string text, float x, float y, Font &font);
    ButtonR(string text, float x, float y, Font &font, float fontSize);
    ButtonR(string text, float x, float y, Font &font, float fontSize, Color fontColor, Color btnColor);
    ButtonR(string text, float x, float y, Font &font, float fontSize, Color fontColor, Color btnColor, Color borderColor);
    ButtonR(string text, Font &font, float fontSize);
    virtual void draw();
    void buildBtn();
    int update();
    void setFont(Font font);
    void setFontSize(float fontSize);
    void setFontSizeNoResize(float fontSize);
    void setText(string text);
    void setTextNoResize(string text);
    void setTextPosition(float x, float y);
    void setBtnPosition(float btnX, float btnY);
    void setBtnSize(float btnWidth, float btnHeight);
    void setShadowPos(float x, float y);
    void setShadowSize(float width, float height);
    bool isHover();
private:
};
#endif
