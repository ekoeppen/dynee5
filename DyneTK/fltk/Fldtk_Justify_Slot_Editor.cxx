//
// DyneTK, the Dyne Toolkit
// Copyright (C) 2007 Matthias Melcher
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "Fldtk_Justify_Slot_Editor.h"
#include <FL/Fl_Input.H>

Fl_Menu_Item Fldtk_Justify_Slot_Editor::menu_wHParent[] = {
 {"Left Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Center Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Right Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Full Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item Fldtk_Justify_Slot_Editor::menu_wHSibling[] = {
 {"None", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Left Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Center Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Right Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Full Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item Fldtk_Justify_Slot_Editor::menu_wVParent[] = {
 {"Top Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Center Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Bottom Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Full Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item Fldtk_Justify_Slot_Editor::menu_wVSibling[] = {
 {"None", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Top Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Center Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Bottom Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Full Relative", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item Fldtk_Justify_Slot_Editor::menu_wHText[] = {
 {"Left", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Center", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Right", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Full", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item Fldtk_Justify_Slot_Editor::menu_wVText[] = {
 {"Top", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Center", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Bottom", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"Full", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item Fldtk_Justify_Slot_Editor::menu_wTextLimits[] = {
 {"No limit", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"One line only", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"One word only", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {"One character only", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 12, 0},
 {0,0,0,0,0,0,0,0,0}
};
Fldtk_Justify_Slot_Editor::Fldtk_Justify_Slot_Editor(Fl_Group *container, Dtk_Value_Slot *slot) 
:   Fldtk_Value_Slot_Editor(container, slot), 
    value_(0) 
{
    box(FL_FLAT_BOX);
    resize(0, 0, 425, 280);
    wValue_->hide();
    begin();
{ Fl_Group* o = new Fl_Group(5, 5, 410, 75, "View Position:");
  o->box(FL_THIN_DOWN_BOX);
  o->labelsize(12);
  o->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
  { Fl_Box* o = new Fl_Box(15, 25, 65, 20, "Horizontal:");
    o->labelsize(12);
    o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  } // Fl_Box* o
  { wHParent = new Fl_Choice(125, 25, 115, 20, "Parent:");
    wHParent->down_box(FL_BORDER_BOX);
    wHParent->labelsize(12);
    wHParent->textsize(12);
    wHParent->menu(menu_wHParent);
  } // Fl_Choice* wHParent
  { wHSibling = new Fl_Choice(290, 25, 115, 20, "Sibling:");
    wHSibling->down_box(FL_BORDER_BOX);
    wHSibling->labelsize(12);
    wHSibling->textsize(12);
    wHSibling->menu(menu_wHSibling);
  } // Fl_Choice* wHSibling
  { Fl_Box* o = new Fl_Box(15, 50, 65, 20, "Vertical:");
    o->labelsize(12);
    o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  } // Fl_Box* o
  { wVParent = new Fl_Choice(125, 50, 115, 20, "Parent:");
    wVParent->down_box(FL_BORDER_BOX);
    wVParent->labelsize(12);
    wVParent->textsize(12);
    wVParent->menu(menu_wVParent);
  } // Fl_Choice* wVParent
  { wVSibling = new Fl_Choice(290, 50, 115, 20, "Sibling:");
    wVSibling->down_box(FL_BORDER_BOX);
    wVSibling->labelsize(12);
    wVSibling->textsize(12);
    wVSibling->menu(menu_wVSibling);
  } // Fl_Choice* wVSibling
  o->end();
} // Fl_Group* o
{ Fl_Group* o = new Fl_Group(5, 90, 410, 60, "Proportional Bounds:");
  o->box(FL_THIN_DOWN_BOX);
  o->labelsize(12);
  o->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
  { wPropLeft = new Fl_Check_Button(20, 115, 70, 20, "Left");
    wPropLeft->down_box(FL_DOWN_BOX);
    wPropLeft->labelsize(12);
  } // Fl_Check_Button* wPropLeft
  { wPropRight = new Fl_Check_Button(90, 115, 70, 20, "Right");
    wPropRight->down_box(FL_DOWN_BOX);
    wPropRight->labelsize(12);
  } // Fl_Check_Button* wPropRight
  { wPropTop = new Fl_Check_Button(160, 115, 70, 20, "Top");
    wPropTop->down_box(FL_DOWN_BOX);
    wPropTop->labelsize(12);
  } // Fl_Check_Button* wPropTop
  { wPropBottom = new Fl_Check_Button(230, 115, 70, 20, "Bottom");
    wPropBottom->down_box(FL_DOWN_BOX);
    wPropBottom->labelsize(12);
  } // Fl_Check_Button* wPropBottom
  o->end();
} // Fl_Group* o
{ Fl_Group* o = new Fl_Group(5, 160, 240, 110, "Text/Graphics:");
  o->box(FL_THIN_DOWN_BOX);
  o->labelsize(12);
  o->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
  { wHText = new Fl_Choice(85, 185, 150, 20, "Horizontal:");
    wHText->down_box(FL_BORDER_BOX);
    wHText->labelsize(12);
    wHText->textsize(12);
    wHText->menu(menu_wHText);
  } // Fl_Choice* wHText
  { wVText = new Fl_Choice(85, 210, 150, 20, "Vertical:");
    wVText->down_box(FL_BORDER_BOX);
    wVText->labelsize(12);
    wVText->textsize(12);
    wVText->menu(menu_wVText);
  } // Fl_Choice* wVText
  { wTextLimits = new Fl_Choice(85, 235, 150, 20, "Text Limits:");
    wTextLimits->down_box(FL_BORDER_BOX);
    wTextLimits->labelsize(12);
    wTextLimits->textsize(12);
    wTextLimits->menu(menu_wTextLimits);
  } // Fl_Choice* wTextLimits
  o->end();
} // Fl_Group* o
{ Fl_Group* o = new Fl_Group(255, 160, 160, 70, "Printing:");
  o->box(FL_THIN_DOWN_BOX);
  o->labelsize(12);
  o->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
  { wReflow = new Fl_Check_Button(265, 180, 115, 20, "Reflow");
    wReflow->down_box(FL_DOWN_BOX);
    wReflow->labelsize(12);
  } // Fl_Check_Button* wReflow
  { wLasso = new Fl_Check_Button(265, 200, 115, 20, "Lasso Children");
    wLasso->down_box(FL_DOWN_BOX);
    wLasso->labelsize(12);
  } // Fl_Check_Button* wLasso
  o->end();
} // Fl_Group* o
{ wAnchored = new Fl_Check_Button(265, 240, 115, 20, "Anchored");
  wAnchored->down_box(FL_DOWN_BOX);
  wAnchored->labelsize(12);
} // Fl_Check_Button* wAnchored
{ Fl_Box* o = new Fl_Box(415, 270, 10, 10);
  Fl_Group::current()->resizable(o);
} // Fl_Box* o
end();
resize(container->x(), container->y(), container->w(), container->h());
}

Fldtk_Justify_Slot_Editor::~Fldtk_Justify_Slot_Editor() {
}

double Fldtk_Justify_Slot_Editor::value() {
  return (double)value_;
}

void Fldtk_Justify_Slot_Editor::value(double v) {
  value_ = (unsigned int)v;
}


//
// DyneTK, the Dyne Toolkit
// Copyright (C) 2007 Matthias Melcher
//