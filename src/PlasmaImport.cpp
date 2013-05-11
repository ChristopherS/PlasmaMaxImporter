/*==LICENSE==

This file is part of PlasmaImport.
Copyright (C) 2013 Christopher Schmidt

This plugin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Lemuria. If not, see <http://www.gnu.org/licenses/>.

*==LICENSE==*/

#include <stdio.h>
#include "PlasmaImport.h"

#include "Util/plString.h"
#include "ResManager/plResManager.h"

#include "PRP/Object/plSceneObject.h"
#include "PRP/Object/plDrawInterface.h"
#include "PRP/Object/plCoordinateInterface.h"
#include "PRP/Geometry/plDrawableSpans.h"

const TCHAR *PlasmaImport::Ext(int n)
{
    switch (n)
    {
    case 0:
        return _T("PRP"); //currently we only support prp
    default:
        return _T("");
    }
}

//some parts of the following two functions are copied from prp2obj
int PlasmaImport::DoImport(const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts)
{
    plResManager rm;
    plPageInfo *page;
    char* msg;

    //Reading the Page
    try
    {
        page = rm.ReadPage(name);
    }
    catch (const std::exception &e)
    {
        msg = const_cast<char*>(e.what());
        gi->PushPrompt(_M(msg));
        return 0;
    }
    catch (...)
    {
        gi->PushPrompt(_M("Unkown Error during Import (1)"));
        return 0;
    }
    //Extract the Object

    size_t objCount = 0;
    try
    {
        std::vector<plKey> objList = rm.getKeys(page->getLocation(), kSceneObject);
        for (size_t j = 0; j < objList.size(); j++)
        {
            plSceneObject* obj = plSceneObject::Convert(rm.getObject(objList[j]));
            if (obj->getDrawInterface().Exists())
                if(IImportObject(obj, i))
                    objCount++;
        }
    }
    catch (...)
    {
        gi->PushPrompt(_M("Unknown Error during Import (2)"));
        return 0;
    }

    sprintf(msg, "Successfully imported %u objects", (unsigned int)objCount);
    gi->PushPrompt(_M(msg));
    delete msg;
    return 1;
}

bool PlasmaImport::IImportObject(plSceneObject *obj, ImpInterface *imp)
{
    if (!obj->getDrawInterface().isLoaded())
        return false;

    plDrawInterface* draw = plDrawInterface::Convert(obj->getDrawInterface()->getObj());
    plCoordinateInterface* coord = NULL;
    if (obj->getCoordInterface().Exists())
        coord = plCoordinateInterface::Convert(obj->getCoordInterface()->getObj());

    TriObject *maxObj = CreateNewTriObject();
    Mesh *msh = &maxObj->GetMesh();

    std::vector<Point3> vertexList;
    std::vector<int> indexList;

    if (!maxObj)
        return false;

    // read the object from PRP
    for (size_t i = 0; i < draw->getNumDrawables(); i++)
    {
        if (draw->getDrawableKey(i) == -1)
            continue;

        plDrawableSpans* span = plDrawableSpans::Convert(draw->getDrawable(i)->getObj());
        plDISpanIndex di = span->getDIIndex(draw->getDrawableKey(i));
        if ((di.fFlags & plDISpanIndex::kMatrixOnly) != 0)
            continue;

        for (size_t idx = 0; idx < di.fIndices.size(); idx++)
        {
            plIcicle* ice = (plIcicle*)span->getSpan(di.fIndices[idx]);
            std::vector<plGBufferVertex> verts = span->getVerts(ice);
            std::vector<unsigned short> indices = span->getIndices(ice);

            for (size_t j = 0; j < verts.size(); j++)
            {
                hsVector3 pos;
                if (coord != NULL)
                    pos = coord->getLocalToWorld().multPoint(verts[j].fPos) * 10.0f;
                else
                    pos = ice->getLocalToWorld().multPoint(verts[j].fPos) * 10.0f;
                vertexList.push_back(Point3(pos.X, pos.Y, pos.Z));
            }
            
            indexList.insert(indexList.end(), indices.begin(), indices.end());
        }
    }
    
    //create the Mesh
    msh->setNumVerts(vertexList.size());
    msh->setNumFaces(indexList.size()/3);
    for (size_t i = 0; i < vertexList.size(); i++)
        msh->setVert(i, vertexList[i]);

    for (size_t i = 0; i < indexList.size()/3; i++)
    {
        int v1, v2, v3;
        v1 = indexList[3*i];
        v2 = indexList[3*i+1];
        v3 = indexList[3*i+2];

        msh->faces[i].setVerts(v1, v2, v3);
        msh->faces[i].setEdgeVisFlags(1,1,1); //we asume we have Triangles obnly
    }

    msh->buildNormals();
    msh->buildBoundingBox();
    msh->InvalidateEdgeList();

    //Add Object To Scene
    ImpNode *node = imp->CreateNode();
    Matrix3 tm;
    tm.IdentityMatrix();
    node->Reference(maxObj);
    node->SetTransform(0,tm);
    imp->AddNodeToScene(node);
    node->SetName(obj->getKey()->getName());
    imp->RedrawViews();

    return true;
}