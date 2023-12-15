#include "colors.h"

const Color darkGrey = {26, 31, 40, 255};
const Color green = {112,195,121,255};
const Color red = {255, 0, 0, 255};
const Color orange = {255, 107, 1, 255};
const Color yellow = {255, 214, 0, 255};
const Color purple = {148, 82, 255, 255};
const Color cyan = {30, 181, 218, 255};
const Color blue = {13, 64, 216, 255};
const Color lightBlue = {59, 85, 162, 255};
const Color darkBlue = {44, 44, 127, 255};
const Color white = {255, 255, 255, 255};

std::vector<Color> GetCellColors()
{
    return {darkGrey, green, orange, yellow, white, cyan, purple, red};
}
