//
// "$Id$"
//
// Dtk_Template implementation for the Dyne Toolkit.
//
// Copyright 2007 by Matthias Melcher.
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

#include "Dtk_Template_List.h"
#include "Dtk_Template.h"

#include "allNewt.h"


/*---------------------------------------------------------------------------*/
Dtk_Template::Dtk_Template(Dtk_Layout_Document *layout, Dtk_Template_List *list)
:   layout_(layout),
    list_(list),
    tmplList_(0L)
{
}


/*---------------------------------------------------------------------------*/
Dtk_Template::~Dtk_Template()
{
    delete tmplList_;
}

/*---------------------------------------------------------------------------*/
int Dtk_Template::load(newtRef node)
{
	newtRef value = NewtGetFrameSlot(node, NewtFindSlotIndex(node, NSSYM(value)));
	if (!NewtRefIsFrame(value))
		return -1;
/*

	int32_t wl, wr, wt, wb;
	uint32_t justify = 0;
	char *text = 0L;
	{	// read the view bounds
		newtRef bnds = NewtGetFrameSlot(v, NewtFindSlotIndex(v, NSSYM(viewBounds)));
		if (NewtRefIsFrame(bnds)) {
			newtRef v = NewtGetFrameSlot(bnds, NewtFindSlotIndex(bnds, NSSYM(value)));
			if (NewtRefIsFrame(v)) {
				 newtRef i;
				 i = NewtGetFrameSlot(v, NewtFindSlotIndex(v, NSSYM(top)));
				 if (NewtRefIsInteger(i)) wt = NewtRefToInteger(i);
				 i = NewtGetFrameSlot(v, NewtFindSlotIndex(v, NSSYM(left)));
				 if (NewtRefIsInteger(i)) wl = NewtRefToInteger(i);
				 i = NewtGetFrameSlot(v, NewtFindSlotIndex(v, NSSYM(bottom)));
				 if (NewtRefIsInteger(i)) wb = NewtRefToInteger(i);
				 i = NewtGetFrameSlot(v, NewtFindSlotIndex(v, NSSYM(right)));
				 if (NewtRefIsInteger(i)) wr = NewtRefToInteger(i);
			}
		}
	}
	{	// read the view justify flags
		newtRef jst = NewtGetFrameSlot(v, NewtFindSlotIndex(v, NSSYM(viewJustify)));
		if (NewtRefIsFrame(jst)) {
			newtRef v = NewtGetFrameSlot(jst, NewtFindSlotIndex(jst, NSSYM(value)));
			if (NewtRefIsInteger(v)) justify = NewtRefToInteger(v);
		}
	}
	{	// read the label 
		newtRef txt = NewtGetFrameSlot(v, NewtFindSlotIndex(v, NSSYM(text)));
		if (NewtRefIsFrame(txt)) {
			newtRef v = NewtGetFrameSlot(txt, NewtFindSlotIndex(txt, NSSYM(value)));
			if (NewtRefIsString(v)) text = NewtRefToString(v);
		}
		if (!text) {
			newtRef v = NewtGetFrameSlot(r, NewtFindSlotIndex(r, NSSYM(__ntName)));
			if (NewtRefIsString(v)) text = NewtRefToString(v);
		}
	}


	Nt_Group *g = new Nt_Group(wl, wt, wr, wb, justify, text);
	g->align(FL_ALIGN_INSIDE | FL_ALIGN_WRAP);
	g->labelsize(9);
	g->end();
*/    

	{	// read the children
		newtRef c = NewtGetFrameSlot(value, NewtFindSlotIndex(value, NSSYM(stepChildren)));
		if (NewtRefIsFrame(c)) {
			newtRef a = NewtGetFrameSlot(c, NewtFindSlotIndex(c, NSSYM(value)));
			if (NewtRefIsArray(a)) {
				int i, n = NewtArrayLength(a);
                if (n) {
                    tmplList_ = new Dtk_Template_List(this);
                }
				for (i=0; i<n; i++) {
                    Dtk_Template *tmpl = new Dtk_Template(layout_, tmplList_);
                    tmplList_->add(tmpl);
                    tmpl->load(NewtGetArraySlot(a, i));
				}
			}
		}
	}

    return 0;
}


//
// End of "$Id$".
//
