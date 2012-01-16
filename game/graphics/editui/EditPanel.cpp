#include "EditPanel.h"

EditPanel::EditPanel()
: group(0, 0, *this)
, subgroup(0, 40, *this)
, item(0, 80, *this)
, gravity(*this, "gravity")
, light(*this, "light")
{

}

