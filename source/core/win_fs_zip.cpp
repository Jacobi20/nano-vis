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
// win_filesys.cpp - zip-archive

#include "../core/core.h"
#include "unzip.h"
#include "win_filesys.h"
#pragma comment(lib, "zlibstat.lib")


#define ZIP_CHECK(expr) if (expr!=UNZ_OK) RAISE_EXCEPTION(va("ZIP : %s failed", #expr));

/*-----------------------------------------------------------------------------
	ZIP :
-----------------------------------------------------------------------------*/

//
//	EWinFileSystem::UpdatePackInfo
//
void EWinFileSystem::UpdatePackInfo(void)
{
	LOG_SPLIT("Loading archives");
	
	CleanUpArchives();
	
	IPxFSSearch fss = Search("", "*.zip", true);

	// Add packs in reverse order to allow pack proper overriding :
	for (int i=fss->GetItemNum()-1; i>=0; i--) {

		try {
			LOGF(va("Zip : %s", fss->GetItemName(i)));

				EWinZipArchive *arch = new EWinZipArchive(this, fss->GetItemName(i));			
				zip_list.push_back(arch);
		
		} catch (exception &e) {
			LOG_WARNING("%s", e.what());
		}	
	}
	
	LOGF("%d loaded acrhives", zip_list.size());
	
	LOG_SPLIT("");
	
	//TestFileSystem();
}


//
//	EWinFileSystem::CleanUpArchives
//
void EWinFileSystem::CleanUpArchives(void)
{
	for (uint i=0; i<zip_list.size(); i++) {
		delete zip_list[i];
	}
	zip_list.clear();
}


//
//	EWinFileSystem::CreateZipFile
//
IFile *EWinFileSystem::CreateZipFile(const char *name)
{
	name += 4;
	string pack_name;
	string file_name;

	while (*name!='/') {
		pack_name += *name;
		name++;
	}
	name++;
	
	file_name = name;
	
	EWinZipArchive *pack = NULL;
	
	for (uint i=0; i<zip_list.size(); i++) {
		if ( strcmp(zip_list[i]->GetPackName(), pack_name.c_str())==0 ) {
			pack = zip_list[i];
		}
	}
	
	ASSERT(pack);
	
	return new EZipFile(file_name.c_str(), pack, this);
}


/*-----------------------------------------------------------------------------
	Zip Archive stuff :
-----------------------------------------------------------------------------*/

EWinZipArchive::EWinZipArchive(EWinFileSystem *fs, const char *pack_name)
{
	this->pack_name = pack_name;

	zip = unzOpen(pack_name);
	if (!zip) {
		RAISE_EXCEPTION(va("cannot unzip file \"%s\"", pack_name));
	}
	
	file_list.clear();
	
	ZIP_CHECK(unzGoToFirstFile(zip));

	while (1) {
		char name[OS_MAX_PATH];	
		unzGetCurrentFileInfo(zip, NULL, name, OS_MAX_PATH, NULL, 0, NULL, 0);

		// directories has name ending "/"
		if (name[strlen(name)-1]!='/') {	
			file_list.push_back(name);
		}
		
		if (unzGoToNextFile(zip)==UNZ_END_OF_LIST_OF_FILE) {
			break;
		}
	}
}


EWinZipArchive::~EWinZipArchive(void)
{
	if (zip) {
		unzClose(zip);
	}
}


/*-----------------------------------------------------------------------------
	ZipFile routines
-----------------------------------------------------------------------------*/

EZipFile::EZipFile(void){}

EZipFile::EZipFile( const char *name, EWinZipArchive *pack, IFileSystem *fs )
{
	this->name	=	name;
	this->fs	=	fs;
	this->pack	=	pack;
	
	ZIP_CHECK( unzLocateFile( pack->GetZIP(), name, 0 ) );  
	unzOpenCurrentFile( pack->GetZIP() );		
	unzGetCurrentFileInfo( pack->GetZIP(), &info, NULL, 0, NULL, 0, NULL, 0 );

	buffer.resize( max(Size(), 1), 0);
	unzReadCurrentFile( pack->GetZIP(), &buffer[0], info.uncompressed_size );
	unzCloseCurrentFile( pack->GetZIP() );
	
	pointer = 0;
}

EZipFile::~EZipFile(void)
{
}


const char *EZipFile::Name(void) const
{
	return name.c_str();
}


uint EZipFile::Size(void) const
{
	return info.uncompressed_size;
}


fileOpenMode_t EZipFile::Mode(void) const
{
	return FS_OPEN_READ;
}


uint EZipFile::Read(void *buffer, uint size)
{
	int size_to_read = min(Size()-pointer, size);
	
	memcpy(buffer, &this->buffer[pointer], size_to_read);
	Seek(size, FS_SEEK_CURRENT);

	return 1;
}


uint EZipFile::Seek(int dist, fileSeekMode_t origin)
{
	if (origin==FS_SEEK_BEGIN)	 pointer =  dist;
	if (origin==FS_SEEK_CURRENT) pointer += dist;
	if (origin==FS_SEEK_END)	 pointer =  Size() + dist;
	pointer = Clamp<int>(pointer, 0, Size());
	return pointer;
}


uint EZipFile::Write(const void *buffer, uint size)
{
	RAISE_EXCEPTION("cannot write to zipped file");
}


void EZipFile::Printf(const char *frmt, ...)
{
	RAISE_EXCEPTION("cannot write to zipped file");
}


void EZipFile::Flush(void)
{
	RAISE_EXCEPTION("cannot flush zipped file");
}



