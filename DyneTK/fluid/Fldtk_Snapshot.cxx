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

#include "Fldtk_Snapshot.h"
static char *prevSnapshotPath; 

Fl_Double_Window *snapshot=(Fl_Double_Window *)0;

Fl_Box *wImage=(Fl_Box *)0;

static void cb_Save(Fl_Button*, void*) {
  Flmp_Image *img = (Flmp_Image*)wImage->image();
if (!img) 
  return;
  
if (!prevSnapshotPath) {
  if (dtkProject && dtkProject->pathname()) {
    prevSnapshotPath = strdup(dtkProject->pathname());
  }
}
  
const char *filename = fl_file_chooser("Save As JPEG", "*.jpg", prevSnapshotPath);
if (filename) {
  // set the default extension to .jpg
  char buf[2048];
  const char *ext = fl_filename_ext(filename);
  if (ext==0L || *ext==0) {
    strcpy(buf, filename);
    fl_filename_setext(buf, 2047, ".jpg");
    filename = buf;
  }
  img->save_jpeg(filename, 90);
  // remember the path to this snapshot for later
  if (prevSnapshotPath)
    free(prevSnapshotPath);
  prevSnapshotPath = strdup(filename);
};
}

void show_snapshot_window() {
  if (!snapshot) {
    { snapshot = new Fl_Double_Window(350, 542, "Snapshot Manager");
      { wImage = new Fl_Box(10, 10, 330, 490, "Loading Snapshot...");
        wImage->box(FL_THIN_DOWN_BOX);
        wImage->labelsize(12);
      } // Fl_Box* wImage
      { Fl_Button* o = new Fl_Button(230, 510, 110, 20, "Save As...");
        o->labelsize(12);
        o->callback((Fl_Callback*)cb_Save);
      } // Fl_Button* o
      snapshot->end();
    } // Fl_Double_Window* snapshot
      }
      // show message about loading the image
Fl_Image *old_img = wImage->image();
delete old_img;
wImage->image(0);
wImage->label("Loading Snapshot...");

snapshot->show();
}

void update_snapshot_window(Flmp_Image *img) {
  // replace label with snapshot image
if (!snapshot)
  show_snapshot_window();

if (img)
  wImage->label(0);
else
  wImage->label("Error loading Snapshot");
  
Fl_Image *oldImg = wImage->image();
wImage->image(0);
delete oldImg;

wImage->image(img);
wImage->redraw();
  
snapshot->show();
}

//
// DyneTK, the Dyne Toolkit
// Copyright (C) 2007 Matthias Melcher
//
