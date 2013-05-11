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

#ifndef plasmaimport_h
#define plasmaimport_h

#include "max.h"
#include "iparamb2.h"
#include "resource.h"

class plSceneObject;

class PlasmaImport : public SceneImport 
{
public:
    PlasmaImport() {};
    ~PlasmaImport() {};

    int         ExtCount() { return 1; } //currently we only support PRP
    const TCHAR *Ext(int n);
    
    //Needed stuff (TODO)
    const TCHAR *LongDesc() { return _T(""); }
    const TCHAR *ShortDesc() { return _T(""); }
    const TCHAR *AuthorName() { return _T(""); }
    const TCHAR *CopyrightMessage() { return _T(""); }
    const TCHAR *OtherMessage1() { return _T(""); }
    const TCHAR *OtherMessage2() { return _T(""); }
    unsigned int Version() { return 1; }
    void        ShowAbout(HWND hWnd) {}
    
    //the actual import function
    /* TODO:
    -find out whats happening with the indices
    -import normals
    -import uv maps
    -import and apply materials
    -age import
    -import components
    */
    int         DoImport(const TCHAR *name,ImpInterface *ii,Interface *i, BOOL suppressPrompts=FALSE);
private:
    bool        IImportObject(plSceneObject *obj, ImpInterface *imp);
};

//ClassDesc
class ImporterClassDesc : public ClassDesc2 
{
public:
    int             IsPublic() { return 1; }
    void            *Create(BOOL loading = FALSE) { return new PlasmaImport; }
    const TCHAR     *ClassName() { return _T("PlasmaImport"); }
    SClass_ID       SuperClassID() { return SCENE_IMPORT_CLASS_ID; }
    Class_ID        ClassID() { return Class_ID(0x6eab41c7, 0x1e4b79a1); }
    const TCHAR     *Category() { return _T("Scene Import"); }
};

static ImporterClassDesc ClassDescInst;

#endif
