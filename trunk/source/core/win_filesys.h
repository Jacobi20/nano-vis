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
// win_filesys.h - windows implementation of virtual file system


#pragma once
#include "unzip.h"


void	BackSlashesToForwardSlashes(char *string);

class EWinFileSystem;



class EWinZipArchive {
	public:
					EWinZipArchive	( EWinFileSystem *fs, const char *pack_name );
					~EWinZipArchive	( void );
		uint		GetFileNum		( void ) const { return file_list.size(); }
		const char  *GetFileName	( uint n ) const { return file_list[n].c_str(); }
		const char  *GetPackName	( void ) const { return pack_name.c_str(); }
		unzFile		GetZIP			( void ) const { return zip; }

	protected:	
		string			pack_name;
		unzFile			zip;
		vector<string>	file_list;
	};



class	EWinFileSystem: public IFileSystem
	{
	public:
								EWinFileSystem	( void );
		virtual					~EWinFileSystem	( void );

		virtual void			UpdatePackInfo	( void );
		
		virtual void			ChDir		( const char *path );
		virtual	void			MkDir		( const char *path );
		virtual void			RmDir		( const char *path );
		virtual void			MkPath		( const char *path );
		virtual const char		*GetDir		( void );

		virtual bool			FileExists	( const char *path );
		virtual void			FileDelete	( const char *path );
		virtual const char		*UniqueName	( const char *path, const char *part, const char *ext );
		
		virtual IPxFile			FileOpen	( const char *name, fileOpenMode_t mode );
		virtual	IPxFSSearch		Search		( const char *dir, const char *mask, bool ignore_archives=false );
		
	private:
		void		TestFileSystem	( void );
	
		const char	*SearchPath		( const char *name );
		bool		FileExistsLocal	( const char *name );
		void		CleanUpArchives	( void );
		IFile		*CreateZipFile	( const char *name );

		uint		ResearchInZip	(char * name);
		uint		ResearchInDisc	(char * dir);

		vector<EWinZipArchive*>	zip_list;		
	
		string		init_work_dir;
		string		game_work_dir;
	};



class	EWinFSSearch : public IFSSearch
	{
	public:
							EWinFSSearch	( void );
		virtual				~EWinFSSearch	( void );
		virtual	uint		GetItemNum		( void ) const;
		virtual const char	*GetItemName	( uint n ) const;

		vector<string>		list;		
	};



class	EFile: public IFile
	{
	friend class EFileSystem;
	public:
								EFile	( const char *name, fileOpenMode_t mode, IFileSystem *fs );
		virtual					~EFile	( void );
								
		virtual const char		*Name	( void ) const;
		virtual uint			Size	( void ) const;
		virtual fileOpenMode_t	Mode	( void ) const;
		
		virtual uint			Read	( void *buffer, uint size );
		virtual uint			Write	( const void *buffer, uint size );
		virtual uint			Seek	( int dist, fileSeekMode_t origin );
		
		virtual void			Flush	( void );
		virtual void			Printf	( const char *frmt, ... );
		
	private:
		EFile( void );
		
		IFileSystem			*fs;
		HANDLE				hFile;
		string				name;
		fileOpenMode_t		mode;
	};


class EZipFile: public IFile
	{
	public:
								EZipFile	( const char *name, EWinZipArchive *pack, IFileSystem *fs );
								~EZipFile	( void );
								
		virtual const char		*Name	( void ) const;
		virtual uint			Size	( void ) const;
		virtual fileOpenMode_t	Mode	( void ) const;
		
		virtual uint			Read	( void *buffer, uint size );
		virtual uint			Write	( const void *buffer, uint size );
		virtual uint			Seek	( int dist, fileSeekMode_t origin );
		
		virtual void			Flush	( void );
		virtual void			Printf	( const char *frmt, ... );
		
	private:
		EZipFile( void );

		EWinZipArchive		*pack;
		IFileSystem			*fs;
		string				name;
		unz_file_info info;
		vector<byte>		buffer;
		int					pointer;
	};

