/***************************************************************************
                          pineditview.cpp  -  description
                             -------------------
    begin                : Tue Nov 27 19:39:03 EET 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// general includes
#include <iostream>
#include "assert.h"
// qt includes
#include <qsizepolicy.h>
#include <q3hbox.h>
#include <q3vbox.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qgl.h>
#include <q3listbox.h>
#include <q3button.h>
#include <qsplitter.h>
#include <q3canvas.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3BoxLayout>
#include <Q3HBoxLayout>
// emilia includes
#include "Private.h"
// application includes
#include "pineditview.h"
#include "glengine.h"
#include "view2d.h"
#include "command.h"
#include "shapeview.h"
#include "polygonview.h"
#include "textureview.h"

PinEditView * PinEditView::p_CurrentView = NULL;

PinEditView::PinEditView(QWidget * parent, PinEditDoc * doc) : QWidget(parent) {
  assert(doc != NULL);
  p_CurrentView = this;
  // connect doc with the view
  connect(doc, SIGNAL(documentChanged()), this, SLOT(slotDocumentChanged()));

  /*	
    this->setMinimumSize(800, 480);
    this->setMaximumSize(1600,960);
    //this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding,	1, 1));
    QHBox * hbox = new QHBox(this);
    hbox->setMinimumSize(800, 480);
    hbox->setMaximumSize(1600,960);
    //hbox->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding,	1, 1));
    QVBox * vboxl = new QVBox(hbox);
    //vboxl->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding,	2, 2));
    vboxl->setMinimumSize(300, 480);
    //vboxl->setMaximumSize(600, 960);
    QVBox * vboxm = new QVBox(hbox);
    //vboxm->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding,	2, 2));
    vboxm->setMinimumSize(300, 480);
    QVBox * vboxr = new QVBox(hbox);
    //vboxr->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding,	2, 2));
    vboxr->setMinimumSize(200, 480);

    hbox->setStretchFactor(vboxl, 3);
    hbox->setStretchFactor(vboxm, 3);
    hbox->setStretchFactor(vboxr, 2);
  */

  QGLFormat format;
  format.setDepth(true);
  format.setDoubleBuffer(true);
  p_GLEngine = new GLEngine(format, this, "glengine", doc);
  p_GLEngine->setMinimumSize(300, 240);

  p_XYView = new View2D(XY, doc, this, 0, 0);
  p_XYView->setMinimumSize(300, 240);

  p_XZView = new View2D(XZ, doc, this, 0, 0);
  p_XZView->setMinimumSize(300, 240);

  p_ZYView = new View2D(ZY, doc, this, 0, 0);
  p_ZYView->setMinimumSize(300, 240);
	
  p_TabWidget = new QTabWidget(this);
  p_TabWidget->setMinimumSize(240, 480);
  p_TabWidget->setMaximumSize(300, 1600);

  p_ShapeView = new ShapeView(doc, this, 0, 0);
  p_PolygonView = new PolygonView(doc, this, 0, 0);
  p_TextureView = new TextureView(doc, this, 0, 0);
  p_TabWidget->addTab(p_ShapeView, "objects");
  p_TabWidget->addTab(p_PolygonView, "polygons");
  p_TabWidget->addTab(p_TextureView, "texture");

  Q3BoxLayout * hlayout = new Q3HBoxLayout(this);

  Q3BoxLayout * vlayoutl = new Q3VBoxLayout(hlayout);
  Q3BoxLayout * vlayoutm = new Q3VBoxLayout(hlayout);
  Q3BoxLayout * vlayoutr = new Q3VBoxLayout(hlayout);
  hlayout->setStretchFactor(vlayoutl, 4);
  hlayout->setStretchFactor(vlayoutm, 4);
  hlayout->setStretchFactor(vlayoutr, 2);

  vlayoutl->addWidget(p_XYView);
  vlayoutl->addWidget(p_ZYView);		
  vlayoutl->setStretchFactor(p_XYView, 1);
  vlayoutl->setStretchFactor(p_ZYView, 1);

  vlayoutm->addWidget(p_XZView);		
  vlayoutm->addWidget(p_GLEngine);
  vlayoutm->setStretchFactor(p_XZView, 1);
  vlayoutm->setStretchFactor(p_GLEngine, 1);

  vlayoutr->addWidget(p_TabWidget);
}

PinEditView::~PinEditView() {
}

void PinEditView::setViewMode(int mode) {
  p_XYView->setMode(mode);
  p_XZView->setMode(mode);
  p_ZYView->setMode(mode);
}

void PinEditView::setZoom(int zoom) {
  p_XYView->setZoom(zoom);
  p_XZView->setZoom(zoom);
  p_ZYView->setZoom(zoom);
}

void PinEditView::slotDocumentChanged() {
  p_ShapeView->doUpdate();
  //TODO update the view
  EM_CERR("pineditview::slotdocumentchanged");
}

