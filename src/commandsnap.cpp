/***************************************************************************
                          commandsnap.cpp  -  description
                             -------------------
    begin                : Fri Apr 12 2002
    copyright            : (C) 2002 by Henrik Enqvist
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
#include <cmath>
// qt includes
#include <qpainter.h>
// application includes
#include "commandsnap.h"
#include "pineditdoc.h"
#include "view2d.h"
// emilia includes
#include "Private.h"
#include "Shape3D.h"
#include "EMath.h"

CommandSnap::CommandSnap(PinEditDoc * doc) : Command(doc) {
	m_fFactor = 1;
	m_bX = false;
	m_bY = false;
	m_bZ = false;
}

CommandSnap::~CommandSnap() {
}

void CommandSnap::clearObjects() {
}

void CommandSnap::execute(const CommandContext & context) {
	EM_CERR("CommandSnap::execute " << m_fFactor);
	assert(context.shape != NULL);
	p_Context->copy(context);

	int index = 0;
	Vertex3D * vtx = context.shape->getVertex3D(p_Doc->getSelectedVertex(index));
	while (vtx != NULL) {
		m_vVertex.push_back(*vtx);
		m_vIndex.push_back(p_Doc->getSelectedVertex(index));
		if (m_bX) vtx->x = (int)((vtx->x)/m_fFactor + (vtx->x > 0 ? 0.5f : -0.5f))*m_fFactor;
		if (m_bY) vtx->y = (int)((vtx->y)/m_fFactor + (vtx->y > 0 ? 0.5f : -0.5f))*m_fFactor;
		if (m_bZ) vtx->z = (int)((vtx->z)/m_fFactor + (vtx->z > 0 ? 0.5f : -0.5f))*m_fFactor;
		++index;
		vtx = context.shape->getVertex3D(p_Doc->getSelectedVertex(index));
	}

	p_Doc->setModified(true);
	p_Doc->updateAll("polygon");
	p_Doc->pushUndo(this);
}

void CommandSnap::undo() {
	EM_CERR("CommandSnap::undo");
	assert(p_Context->shape != NULL);
	assert(m_vVertex.size() == m_vIndex.size());
	vector<int>::iterator indexiter = m_vIndex.begin();
	vector<int>::iterator indexend = m_vIndex.end();
	vector<Vertex3D>::iterator vtxiter = m_vVertex.begin();
	for (; indexiter != indexend; ++indexiter, ++vtxiter) {
		Vertex3D * vtx = p_Context->shape->getVertex3D(*indexiter);
		assert(vtx != NULL);
		vtx->x = (*vtxiter).x;
		vtx->y = (*vtxiter).y;
		vtx->z = (*vtxiter).z;
	}
	p_Doc->updateAll("polygon");
}

Command * CommandSnap::build() {
	//CommandSnap * snap =  new CommandSnap(*this);
	CommandSnap * snap =  new CommandSnap(p_Doc);
	snap->setX(m_bX);
	snap->setY(m_bY);
	snap->setZ(m_bZ);
	snap->setFactor(m_fFactor);
	return snap;
}
