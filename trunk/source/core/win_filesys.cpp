/*
	The MIT License

	Copyright (c) 2010 IFMO/GameDev Studio

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
// win_filesys.cpp - windows implementation of virtual file system

#include "../core/core.h"
#include "unzip.h"
#include "win_filesys.h"
#pragma comment(lib, "zlibstat.lib")


/*-----------------------------------------------------------------------------
	EXPORT STUFF
-----------------------------------------------------------------------------*/
DLL_EXPORT	IFileSystem	*CreateFileSystem(void)
{
	return new EWinFileSystem();
}


/*-----------------------------------------------------------------------------
	File System Init/Shutdown stuff	
-----------------------------------------------------------------------------*/

EWinFileSystem::EWinFileSystem(void)
{
	init_work_dir	=	GetDir();	

	LOG_SPLIT("File system initialization");

	LOGF("working directory : %s\r\n", init_work_dir.c_str());

	LOG_SPLIT("");
}


EWinFileSystem::~EWinFileSystem(void)
{
	CleanUpArchives();
	LOGF("SHUTDOWN : File system\r\n");
}


//
//	EWinFileSystem::TestFileSystem
//
void EWinFileSystem::TestFileSystem(void)
{
	try {
		IPxFile f = FileOpen("seek_test.txt", FS_OPEN_READ);
		//	file should contain string: "0123456789ABCDEF"
		
		char buffer[32];
		memset(buffer, 0, 32);
		
		f->Seek(0, FS_SEEK_BEGIN);	
		f->Read(buffer, 32);				
		//	result: "0123456789ABCDEF"
		memset(buffer, 0, 32);

		f->Seek(0, FS_SEEK_BEGIN);	
		f->Read(buffer, 4);
		//	result: "0123"
		
		f->Seek(4, FS_SEEK_BEGIN);	
		f->Read(buffer, 4);
		//	result: "4567"

		f->Seek(2, FS_SEEK_CURRENT);	
		f->Read(buffer, 4);
		//	result: "ABCD"

		f->Seek(-4, FS_SEEK_CURRENT);	
		f->Read(buffer, 4);
		//	result: "ABCD"

		f->Seek(-8, FS_SEEK_END);	
		f->Read(buffer, 4);
		//	result: "89AB"
		
	} catch (exception &e) {
		LOGF("TestFileSystem() failed : %s\r\n", e.what());
	}
	
	IPxFSSearch fss = Search("music/", "*.wav", false);
	
	for (uint i=0; i<fss->GetItemNum(); i++) {
		LOGF("  %s\r\n", fss->GetItemName(i));
	}
}

/*-----------------------------------------------------------------------------
	Directory stuff :
-----------------------------------------------------------------------------*/
//
//	EWinFileSystem::ChDir
//
void EWinFileSystem::ChDir(const char *path)
{
	if (!path) { 
		SetCurrentDirectory(init_work_dir.c_str());
		return;
	}

	if (strlen(path)==0) return;

	if (!SetCurrentDirectory(path)) {
		RAISE_EXCEPTION("SetCurrentDirectory() failed");
	}
}

//
//	EWinFileSystem::MkDir
//
void EWinFileSystem::MkDir(const char *path)
{
	if (strlen(path)==0) return;

	if ( !CreateDirectory(path, NULL) ) {
		if ( GetLastError() != ERROR_ALREADY_EXISTS ) {
			RAISE_EXCEPTION("CreateDirectory() failed");
		}
	}
}


//
//	EWinFileSystem::RmDir
//
void EWinFileSystem::RmDir(const char *path)
{
	if (strlen(path)==0) return;

	if ( !RemoveDirectory(path) )
		RAISE_EXCEPTION("RemoveDirectory() failed");
}


//
//	EWinFileSystem::MkPath
//
void EWinFileSystem::MkPath( const char *cpath )
{
	ASSERT(strlen(cpath) < OS_MAX_PATH);

	char spath[OS_MAX_PATH], *path;
	path = &spath[0];
	strcpy(path, cpath);	
	

	char	*ofs, c;

	if (path[1] == ':')
		path += 2;

	for (ofs = path+1 ; *ofs ; ofs++)
	{
		c = *ofs;
		if (c == '/' || c == '\\')
		{	// create the directory
			*ofs = 0;
			MkDir(path);
			*ofs = c;
		}
	}
}



//
//	EWinFileSystem::GetDir
//
const char *EWinFileSystem::GetDir(void)
{
	static char buffer[1024];
	GetCurrentDirectory(1023, buffer);
	BackSlashesToForwardSlashes(buffer);
	strcat(buffer, "/");
	return buffer;
}


/*-----------------------------------------------------------------------------
	File managment stuff :
-----------------------------------------------------------------------------*/
//
//	EWinFileSystem::FileExists
//		- public function, searchs for file 
//		  entire all virtual file system, 
//  	  including startup dir, game dir and packs.
//
bool EWinFileSystem::FileExists(const char *path)
{
	if (!SearchPath(path)) { 
		return false;
	} else {
		return true;
	}
}


//
//	EWinFileSystem::FileExistsLocal
//
bool EWinFileSystem::FileExistsLocal(const char *path)
{
	HANDLE	hFile = CreateFile(path, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile==INVALID_HANDLE_VALUE) return false;
	CloseHandle(hFile);
	return true;
}


//			
//	EWinFileSystem::UniqueName
//
const char *EWinFileSystem::UniqueName(const char *path, const char *part, const char *ext)
{
	static	char	unique[1024];

	for (int i=0; i<100; i++)
	{
		sprintf(unique, "%s/%s%02d%s", path, part, i, ext);
		if (!FileExists(unique)) break;
	}
	
	return unique;
}


//
//	EWinFileSystem::FileDelete
//
void EWinFileSystem::FileDelete(const char *path)
{
	if (!DeleteFile(path))
		RAISE_EXCEPTION("DeleteFile() failed");
}


//
//	EWinFileSystem::SearchPath
//	- finds appropriate path, search order:
//		1) mission dir
//		2) packs
//		3) system dir
//		
const char *EWinFileSystem::SearchPath(const char *name)
{
	static char path[OS_MAX_PATH];

	//	search mission dir :	
	sprintf(path, "%s", name);
	if (FileExistsLocal(path)) {
		return path;
	}	

	//	search packs :
	for (uint i=0; i<zip_list.size(); i++) {
		for (uint j=0; j<zip_list[i]->GetFileNum(); j++) {
			if (strcmp(zip_list[i]->GetFileName(j), name)==0) {
				sprintf(path, "zip:%s/%s", zip_list[i]->GetPackName(), zip_list[i]->GetFileName(j));
				return path;
			}
		}
	}

	//	search system dir :	
	sprintf(path, "%s%s", init_work_dir.c_str(), name);
	if (FileExistsLocal(path)) {
		return path;
	}

	//	return if nothing found :
	return NULL;
}


//
//	EWinFileSystem::FileOpen
//
IPxFile EWinFileSystem::FileOpen(const char *name, fileOpenMode_t mode)
{
	if (mode==FS_OPEN_READ) 
	{
		const char* path = SearchPath(name);
		
		if (!path) {
			RAISE_EXCEPTION(va("file '%s' does not exist", name));
		}

		if ( path[0]=='z' && path[1]=='i' && path[2]=='p' && path[3]==':' ) {
			if (mode!=FS_OPEN_READ) {
				RAISE_EXCEPTION("zip-files are readonly");
			}
			return CreateZipFile(path);
		}
		
		return new EFile(path, mode, this);
	}
	

	if (mode==FS_OPEN_WRITE || mode==FS_OPEN_APPEND) {	
		return new EFile(name, mode, this);
	}
	
	ASSERT(0);
	return NULL;	
}


/*-----------------------------------------------------------------------------
	File Search Stuff :
-----------------------------------------------------------------------------*/

static bool SortString (string &a, string &b) 
{
	return ( strcmp(a.c_str(), b.c_str()) > 0 );
}

//
//	EWinFileSystem::BeginSearch
//
IPxFSSearch EWinFileSystem::Search(const char *dir, const char *mask, bool ignore_archives)
{
	EWinFSSearch	*fss	=	new EWinFSSearch;
	
	char current_dir[1024];
	ASSERT( GetCurrentDirectory(1023, current_dir) );

	//
	//	Search disk space :
	//
	try	{
		ChDir(dir);

		char current_dir2[1024];
		ASSERT( GetCurrentDirectory(1023, current_dir2) );
		
		WIN32_FIND_DATA	find_data;
		HANDLE			hFind	=	FindFirstFile("*", &find_data);
		if (hFind==INVALID_HANDLE_VALUE) RAISE_EXCEPTION("FindFirstFile() failed");

		while(1) {
			if (WildMatch(mask, find_data.cFileName)) {
				fss->list.push_back(find_data.cFileName);
			}

			if (!FindNextFile(hFind, &find_data)) break;
		}
		
		FindClose(hFind);
		
	}
	catch(exception &e)	{
		e.what();
		// nothing terrible happened, we will check packs later.
		fss->list.clear();
		ChDir(current_dir);
	}

	ChDir(current_dir);

	//
	//	Search packs :	
	//
	//	add trailing forward slash "/":
	if (!ignore_archives) 
	{
		char pack_dir[OS_MAX_PATH];
		strcpy(pack_dir, dir);
		if (pack_dir[ strlen(pack_dir)-1 ]!='/') {
			strcat(pack_dir, "/");
		}

		//	search for all packs :	
		for (uint i=0; i<zip_list.size(); i++) {
			for (uint j=0; j<zip_list[i]->GetFileNum(); j++) 
			{
				char name[OS_MAX_PATH];
				strcpy(name, zip_list[i]->GetFileName(j));

				const char *start = strstr( name, pack_dir );
				if (!start) {
					continue;	// another directory
				}
				start+=strlen(pack_dir);
				
				if (strstr(start, "/")) {
					continue;	// file is in subfolder
				}
				
				if (WildMatch(mask, start)) {
					fss->list.push_back(start);
				}
			}
		}
	}

	//
	//	Sort results by name :
	//	MSDN :
	//	The order in which the search returns the files, such as alphabetical order, 
	//	is not guaranteed, and is dependent on the file system. You cannot depend on 
	//	any specific ordering behavior. If the data must be sorted, you must do the 
	//	ordering yourself after obtaining all the results.
	//
	if (!fss->list.empty()) {
		sort(fss->list.begin(), fss->list.end(), SortString);
	}
	
	return fss;
}


EWinFSSearch::EWinFSSearch(void)
{
	list.clear();
}


EWinFSSearch::~EWinFSSearch(void)
{
}


uint EWinFSSearch::GetItemNum(void) const
{
	return (uint)list.size();
}


const char *EWinFSSearch::GetItemName(uint n) const
{
	return list[n].c_str();
}


/*-----------------------------------------------------------------------------
	File routines
-----------------------------------------------------------------------------*/

EFile::EFile(void){}

EFile::EFile( const char *name, fileOpenMode_t mode, IFileSystem *fs )
{
	this->name	=	name;
	this->fs	=	fs;
	this->mode	=	mode;
	hFile		=	NULL;
	
	if		(mode==FS_OPEN_READ)	
	{
		hFile	= CreateFile(name, GENERIC_READ,	0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE) {
			RAISE_EXCEPTION(va("file \"%s\" does not exist", name));
		}
		Seek(0, FS_SEEK_BEGIN);
	}
	else if	(mode==FS_OPEN_WRITE)	
	{
		hFile	= CreateFile(this->name.c_str(), GENERIC_WRITE,	0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE) {
			RAISE_EXCEPTION(va("cann`t create file \"%s\" for writing", name));
		}
		Seek(0, FS_SEEK_BEGIN);
	}
	else if (mode==FS_OPEN_APPEND)	
	{
		hFile	= CreateFile(this->name.c_str(), GENERIC_WRITE,	0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE) {
			ASSERT(0);
			RAISE_EXCEPTION(va("cann`t create file \"%s\" for appending", name));
		}
		Seek(Size(), FS_SEEK_BEGIN);
	}
	else 
	{
		RAISE_EXCEPTION(va("wrong file opening mode"));
	}
}


EFile::~EFile(void)
{
	FlushFileBuffers( hFile );	//	fix bug with file appending?
	if ( !CloseHandle(hFile) )	RAISE_EXCEPTION("Can`not close file");
}


const char *EFile::Name(void) const
{
	return name.c_str();
}


uint EFile::Size(void) const
{
	return GetFileSize(hFile, NULL);
}


fileOpenMode_t EFile::Mode(void) const
{
	return mode;
}


uint EFile::Read(void *buffer, uint size)
{
	unsigned long int	num;
	if (mode!=FS_OPEN_READ) RAISE_EXCEPTION(va("file \"%s\" is not opened for reading", name.c_str()));
	
	return ReadFile(hFile, buffer, size, &num, NULL);
}


uint EFile::Write(const void *buffer, uint size)
{
	unsigned long int	num;
	if (mode!=FS_OPEN_WRITE && mode!=FS_OPEN_APPEND) RAISE_EXCEPTION(va("file \"%s\" is not opened for writing", name.c_str()));
	
	return WriteFile(hFile, buffer, size, &num, NULL);;
}


uint EFile::Seek(int dist, fileSeekMode_t origin)
{
	if (origin==FS_SEEK_BEGIN)		return SetFilePointer(hFile, dist, NULL, FILE_BEGIN);
	if (origin==FS_SEEK_CURRENT)	return SetFilePointer(hFile, dist, NULL, FILE_CURRENT);
	if (origin==FS_SEEK_END)		return SetFilePointer(hFile, dist, NULL, FILE_END);
	return 0;
}


void EFile::Printf(const char *frmt, ...)
{
	char text[16384];
	va_list	argptr;

	va_start (argptr, frmt);
	vsprintf (text, frmt, argptr);
	va_end (argptr);

	Write(text, (uint)strlen(text));
}

void EFile::Flush(void)
{
	FlushFileBuffers(hFile);
}

