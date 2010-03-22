/*
	The MIT License

	Copyright (c) 2008 IFMO/GameDev Studio

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/
// shell_vars.cpp - 


#include "..\global.h"	
#include "shell.h"		



/*-----------------------------------------------------------------------------
	Var registering stuff :
-----------------------------------------------------------------------------*/

IPxVar EShell::RegisterVar(const char *name, const char *value, uint flags )
{
//	ASSERT(IsConsoleIdent(name));

	for (uint i=0; i<var_list.size(); i++)
	{
		if (strcmp(name, var_list[i]->Name())==0) 
		{
			EVar *var = var_list[i].To<EVar>();
			var->SetDefault(value);
			
			var->SetFlags(flags);
			if (flags & SHLF_NEW) {
				var->ResetFlags(SHLF_READONLY);
				var->SetValue(value);
			}
			
			return var;
		}
	}
	
	IPxVar	new_var = new EVar(name, value, flags);
	var_list.push_back(new_var);
	
	return var_list[ var_list.size()-1 ];
}


IPxVar EShell::FindVar(const char *name)
{
	for (uint i=0; i<var_list.size(); i++)
	{
		if (strcmp(name, var_list[i]->Name())==0) 
		{
			return var_list[i];
		}
	}
	
	return NULL;
}


bool CheckPrefix(const char *prefix, const char *name)
{
	uint len1  = (uint)strlen(prefix);
	uint len2  = (uint)strlen(name);
	
	if (len1>len2) {
		return false;
	}
	
	if (strncmp(prefix, name, len1)==0) { 
		return true;
	} else {
		return false;
	}
}


void EShell::ResetVariables(const char *prefix)
{
	uint num = 0;
	uint num_all = (uint)var_list.size();
	
	for (uint i=0; i<var_list.size(); i++) 
	{
		if (CheckPrefix(prefix, var_list[i]->Name())) {
			var_list[i]->ResetValue();
			num++;
		}
	}
	LOGF("Reseted vars : %d of %d. Commit required!\r\n", num, num_all);
}


void EShell::CleanVars(void)
{
	var_list.clear();
}


/*-----------------------------------------------------------------------------
	Shell var functions :
-----------------------------------------------------------------------------*/

static const char *VarFlagsToString(uint flags)
{
	static char str[5];
	strcpy(str, "....");
	if (flags & SHLF_EXECUTABLE)	str[0] = 'X';
	if (flags & SHLF_READONLY)		str[1] = 'r';
	if (flags & SHLF_NOSAVE)		str[2] = 's';
	if (flags & SHLF_NEW)			str[3] = 'n';
	
	return str;
}


bool IsNamespace(const char *name) 
{
	const char *scan = name;
	for ( ; *scan ; scan++) {
		if (*scan==':') return true;
	}
	return false;
}


void EShell::ListVars_f(EShell *self, int argc, const char **argv)
{
	uint num = (uint)self->var_list.size();

	LOGF("\r\n");
	for (uint i=0; i<num; i++)	
	{
		EVar *var = self->var_list[i].To<EVar>();
		if ( !IsNamespace(var->Name()) ) {
			LOGF("%4s %-24s = \"%s\"\r\n", VarFlagsToString(var->Flags()), var->Name(), var->String());
		}
	}
	
	LOGF("%d vars\r\n", num);
}


void EShell::Set_f(EShell *self, int argc, const char **argv)
{
	if (argc!=3) {
		LOGF("usage : set <var name> <new value>\r\n");
		return;
	}
	
	IPxVar var	=	self->RegisterVar(argv[1], argv[2], 0);
	ASSERT(var);
	
	var->SetValue(argv[2]);
}


void EShell::SetX_f(EShell *self, int argc, const char **argv)
{
	if (argc!=3) {
		LOGF("usage : setx <var name> <new value>\r\n");
		return;
	}
	
	IPxVar var	=	self->RegisterVar(argv[1], argv[2], SHLF_EXECUTABLE);
	ASSERT(var);
	
	var->SetValue(argv[2]);
}


void EShell::Get_f(EShell *self, int argc, const char **argv)
{
	if (argc!=2) {
		LOGF("usage : ïet <var name>\r\n");
		return;
	}
	
	EVar *var	=	self->FindVar(argv[1]).To<EVar>();
	
	if (!var) {
		LOGF("Var \"%s\" does not exist\r\n", argv[1]);
		return;
	}
	
	LOGF("\r\n");
	LOGF("variable : %s\r\n",		var->Name());
	LOGF("  string : \"%s\"\r\n",	var->String());
	LOGF(" default : \"%s\"\r\n",	var->Default());
	LOGF("   flags : [%s]\r\n",		VarFlagsToString(var->Flags()));
	LOGF("   float : %f\r\n",		var->Float());
	LOGF("     int : %d\r\n",		var->Int());
	LOGF("\r\n");
}


void EShell::VarCmd_f(EShell *self, int argc, const char **argv)
{
	// IConsole	*con	=	self->con;
	
	EVar	*var	=	self->FindVar(argv[0]).To<EVar>();
	
	if (!var) {
		RAISE_EXCEPTION(va("unknown variable \"%s\"", argv[0]));
	}

	if (var->Flags() & SHLF_EXECUTABLE) {
	
		self->ExecuteString(va("%s\n", var->String()));
	
	} else {
		if (argc==1) {
			uint	flags	=	var->Flags();
			LOGF("%s = \"%s\"\r\n", var->Name(), var->String());
			var->SetFlags(flags);
		};
		
		if (argc==2) {
			var->SetValue(argv[1]);
		}
	}
}


void EShell::ResetVars_f(EShell *self, int argc, const char **argv)
{
	// IConsole	*con	=	self->con;
	if (argc==1) {
		self->ResetVariables("");
	}
	if (argc==2) {
		self->ResetVariables(argv[1]);
	}
}

/*-----------------------------------------------------------------------------
	Var stuff :
-----------------------------------------------------------------------------*/

EVar::EVar(const char *name, const char *value, uint flags) {
	this->default_value		=	value;
	this->current_value		=	value;
	this->name	=	name;
	this->flags	=	flags;
	Update();
}


EVar::~EVar(void) {
}


const char *EVar::Name(void) const 
{
	return name.c_str();
}

void EVar::SetValue(const char *val) 
{
	if (flags & SHLF_READONLY) RAISE_EXCEPTION(va("var \"%s\" is read only", name.c_str()));

	current_value	=	val;	
	
	Update();
}


void EVar::ResetValue(void)
{
	SetValue(default_value.c_str());
}


void EVar::Update(void)
{
	val_float	=	(float)atof(current_value.c_str());
	val_int		=	atoi(current_value.c_str());

	val_vec4	=	EVec4(0,0,0,0);
	
	sscanf(current_value.c_str(), "%f%f%f%f", &val_vec4.x, &val_vec4.y, &val_vec4.z, &val_vec4.w);

	val_vec3.x = val_vec4.x;
	val_vec3.y = val_vec4.y;
	val_vec3.z = val_vec4.z;

	val_vec2.x = val_vec4.x;
	val_vec2.y = val_vec4.y;
}


void EVar::SetFlags(uint f) 
{
	flags |= f;	
}


void EVar::ResetFlags(uint f) 
{
	flags &= (~f);
}











