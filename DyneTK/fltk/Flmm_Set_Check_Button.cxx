//
// "$Id$"
//
// Flmm_Set_Input implementation for the FLMM extension to FLTK.
//
// Copyright 2002-2007 by Matthias Melcher.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "flmm@matthiasm.com".
//

#ifdef WIN32
#pragma warning(disable : 4996)
#endif


#include "Flmm_Set_Check_Button.h"


#include <stdlib.h>
#include <string.h>


Flmm_Set_Check_Button::Flmm_Set_Check_Button(int X, int Y, int W, int H, const char *L)
:	Fl_Check_Button(X, Y, W, H, L),
	data_(0)
{
}


Flmm_Set_Check_Button::~Flmm_Set_Check_Button()
{
}


int Flmm_Set_Check_Button::get()
{
	return data_;
}


void Flmm_Set_Check_Button::set(int d)
{
	data_ = d;
}


void Flmm_Set_Check_Button::update_data()
{
	set(value());
}


void Flmm_Set_Check_Button::update_widget()
{
	value(get());
}



//
// End of "$Id$".
//
