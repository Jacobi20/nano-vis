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
// i_filesys.h - interface for virtual file system


#pragma once


enum	fileSeekMode_t
	{
		FS_SEEK_BEGIN,
		FS_SEEK_END,
		FS_SEEK_CURRENT,
	};
	
enum	fileOpenMode_t
	{
		FS_OPEN_READ,
		FS_OPEN_WRITE,
		FS_OPEN_APPEND,
	};


class IFile : public IDisposable { 
	public:
		virtual				~IFile	( void ) {};
		
		virtual const char		*Name	( void ) const = 0;
		virtual uint			Size	( void ) const = 0;
		virtual fileOpenMode_t	Mode	( void ) const = 0;
		
		virtual uint			Read	( void *buffer, uint size ) = 0;
		virtual uint			Write	( const void *buffer, uint size ) = 0;
		virtual uint			Seek	( int dist, fileSeekMode_t origin ) = 0;
		
		virtual void			Flush	( void ) = 0;
		virtual void			Printf	( const char *frmt, ... ) = 0;
	};
	
class IFSSearch : public IDisposable { 
	public:
		virtual				~IFSSearch		( void ) {};
		virtual	uint		GetItemNum		( void ) const = 0;
		virtual const char	*GetItemName	( uint n ) const = 0;
	};


typedef hard_ref<IFile>		IPxFile;
typedef hard_ref<IFSSearch>	IPxFSSearch;

class IFileSystem : public IDisposable { 
	public:
		virtual					~IFileSystem	( void ) {}
		
		virtual void			UpdatePackInfo	( void ) = 0;

		virtual	void			MkDir		( const char *path ) = 0;
		virtual void			RmDir		( const char *path ) = 0;
		virtual void			ChDir		( const char *path ) = 0;
		virtual void			MkPath		( const char *path ) = 0;
		virtual const char		*GetDir		( void ) = 0;

		virtual bool			FileExists	( const char *path ) = 0;
		virtual void			FileDelete	( const char *path ) = 0;
		virtual const char		*UniqueName	( const char *path, const char *part, const char *ext ) = 0;
		
		virtual IPxFile			FileOpen	( const char *name, fileOpenMode_t mode ) = 0;
		
		virtual	IPxFSSearch		Search		( const char *dir, const char *mask, bool ignore_archives=false ) = 0;
	};
	
	
	
	