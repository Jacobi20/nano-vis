/*
	The MIT License

	Copyright (c) 2010

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
// i_sound_system.h - 


#pragma once

class ISound3D;
class ISoundTrack;
class ISoundGeometry;
class ISoundReverb;

typedef hard_ref<ISound3D>		IPxSound3D;
typedef hard_ref<ISoundTrack>	IPxSoundTrack;
typedef hard_ref<ISoundGeometry>	IPxSoundGeometry;
typedef hard_ref<ISoundReverb>	IPxSoundReverb;

enum	soundReverbType_t
	{
		SOUND_REVERB_OFF = 0,
		SOUND_REVERB_GENERIC,
		SOUND_REVERB_PADDEDCELL,
		SOUND_REVERB_BATHROOM,
		SOUND_REVERB_LIVINGROOM,
		SOUND_REVERB_STONEROOM,
		SOUND_REVERB_AUDITORIUM,
		SOUND_REVERB_CONCERTHALL,
		SOUND_REVERB_CAVE,
		SOUND_REVERB_ARENA,
		SOUND_REVERB_HANGAR,
		SOUND_REVERB_CARPETTEDHALLWAY,
		SOUND_REVERB_HALLWAY,
		SOUND_REVERB_STONECORRIDOR,
		SOUND_REVERB_ALLEY,
		SOUND_REVERB_FOREST,
		SOUND_REVERB_CITY,
		SOUND_REVERB_MOUNTAINS,
		SOUND_REVERB_QUARRY,
		SOUND_REVERB_PLAIN,
		SOUND_REVERB_ROOM,
		SOUND_REVERB_PARKINGLOT,
		SOUND_REVERB_SEWERPIPE,
		SOUND_REVERB_UNDERWATER,
	};

class ISoundReverb : public IDisposable {
	public:
		virtual					~ISoundReverb			( void ) {}
		virtual	void			Set3DProperties			( const EVec4& pos, float min_distance, float max_distance ) = 0;
		virtual	EVec4			GetPosition				( void ) = 0;
		virtual void			GetDistance				( float &min_distance, float &max_distance ) = 0;
};


class ISoundGeometry : public IDisposable {
	public:
		virtual					~ISoundGeometry			( void ) {}
		virtual	void			SetPosition				( const EVec4 &pos ) = 0;
		virtual void			SetOrient				( const EQuat &orient ) = 0;
};


///
///	��������� � ������� ����������� �����.
///	�������� ���������� - ����� ���������, ������, ����� ����������� ����.
///	� ������� ����� �������������� ��������� �������� ISound3D
///
class ISound3D : public ILocatable {
	public:
		virtual					~ISound3D	( void ) {}

		///	�������� �������������� �����. 
		///	���������� ���� �� ���������������.
		///	�������� ��������� ����� ��������������� - ����������� � �����������.
		///	������ ��������, ��� ����� ������������ ���������������, ������ �� ������ ������������,
		///	� ������ �������������� ����������� ����������� ��������������� ��� ���.
		///	���� � ������ ������ ������� ISound::Play ��������������� ��� ��������������, �� ���� ������ ������ ��������������� �������, 
		///	� ����� ���������� ��������� ��������������.
		/// @param	loop	-	���������� ����� �������������� �����. ����������� ��� �����������.
		virtual void			Play			( bool loop ) = 0;
		
		///	������������� ��������������� �����. ������� ��������, ��� ����� ��������� ���� �� ������ ������������.
		/// �.�. ������ ������� ����������� ������ �������������� ������.
		virtual void			Stop			( void ) = 0;
		
		///	���������������� ��������������� �����.
		///	��� ������ Play - ��������������� ���������� � ���������� ����� ��������������� 
		virtual void			Pause			( void ) = 0;

		///	���������, ��������������� �� � ������ ������ ����.		
		/// @return			-	���������������.
		virtual bool			IsPlaying		( void ) = 0;
		
		/// ������������� ��������� �����. 
		///	�������� �� ��������� 1.0.
		///	@param	volume	-	��������� �����. �������� �������� [0..1].
		virtual void			SetVolume		( float volume ) = 0;
		
		/// ������������� ������������� ������ �����. 
		///	��������, ���� �������� ����� 2, �� ���� ��������������� � 2 ���� �������, 
		///	� �� ����� � ��� ���� ������� ������������� ������.
		///	�������� �� ��������� 1.0. 
		///	@param	pitch	-	������������� ������ �����. �������� �������� [0.5..2.0]
		///						(�������� ������� ��������!)
		virtual void			SetPitch		( float pitch ) = 0;

		///	�������� ��� ��������� ����.
		/// @param	mute	-	���������� �������������� �� ����.
		virtual	void			SetMute			( bool mute ) = 0;	
	};


///
///	��������� � ������� ������������ �������������.
///	�������� ���������� - ������� ������.
///	� ������� ����� �������������� ��������� �������� ISoundTrack.
///	���������� ������� �� ISound3D � 2D-�������� ������, ��������������� �������� ISoundSystem::Play2DSound
///	����������� � ���, ��� ���� �������� 3-������, � ��������������� ��������. 
///	�.�. ���� ����������� ���������� �� ���� ���������������.
///
class ISoundTrack : public IDisposable {
	public:
		virtual					~ISoundTrack		( void ) {}
		
		///	�������� ��������������� ������.
		///	�� ���������, ���� �� ���������������.
		virtual	void			Play			( void ) = 0;
		
		///	���������������� ��������������� ������.
		///	��� ������ Play - ��������������� ���������� � ����������� ����� ��������������� 
		virtual void			Pause			( void ) = 0;
		
		///	������������� ��������������� �����.
		///	��� ������ Play - ��������������� ���������� � ������ �����.
		virtual void			Stop			( void ) = 0;
		
		/// ������������� ��������� �����. 
		///	�������� �� ��������� 1.0.
		///	@param	volume	-	��������� �����. �������� �������� [0..1].
		virtual void			SetVolume		( float volume ) = 0;
		
		///	����������� ����� ����� � ��������.
		///	@return			-	����� ����� � ��������, �������� ����� ���� �������.
		virtual float			GetLength		( void ) = 0;
		
		///	������������� �������� ����� ���������������.
		///	����� �������������� �� ��������� - 0.0.
		///	@param	time	-	������������� ����� ���������������.
		virtual void			SetPos			( float time ) = 0;

		///	���������� �������� ����� ���������������.
		///	@return	time	-	������� ����� ���������������.
		virtual float			GetPos			( void ) = 0;

		///	�������� ��� ��������� ����.
		/// @param	mute	-	���������� �������������� �� ����.
		virtual	void			SetMute			( bool mute ) = 0;	
	};


///
///	��������� � ������� �������� �������
///	�������� ����������.
///	�������� ������� ������������� ��� ���������� � ��������������� 
///	2D- � 3D-������ � ������������ �������������, 
///	� ����� ��������� ������������ ������� ���������� �����. 
///	��� 3D-�������� ������������ �������������� ������� ���������.
///
class ISoundSystem : public IDisposable {
	public:
		virtual					~ISoundSystem		( void ) {}

		///	������� ��� ����������� �����, ������ � �.�. 
		///	������������ ��� ������������ ������ ����� ��������� ������ � ��������� ������.
		/// ��� ���������, ����� ������ ���� ������� ���������� �������������.
		virtual void			FreeSounds			( void ) = 0;

		///	��������� ��������������� 2D-�����. 
		///	�������� ���������� � ��������������� ������ ����������������� ���������� (UI, HUD � �.�.).
		/// @param	path	-	���� � ����� �����.
		/// @param	volume	-	�������� ��������� ���������������� �����. �������� �������� [0..1].
		/// @param	panning -	���������������� ��������� �����. �������� �������� [-1..1]
		virtual void			Play2DSound			( const char *path, float volume, float panning ) = 0;

		///	������������� ��������� ���������.
		///			
		///	@param	pos		-	������� ���������
		///	@param	view	-	������ �����������, ���� ������� ���������
		///	@param	up		-	�������� "����" ���������
		///	@param	vel		-	�������� ������� ���������
		virtual void			SetListener			( const EVec4 &pos, const EVec4 &view, const EVec4 &up, const EVec4 &vel ) = 0;

		///	������� ��������� ������, ������������ ��������� ISound3D, � ���������� �� ���� ���������.
		///	��������� ���������������� ���� �� ���������������, � ��������� � ��������� (0,0,0). �.�. �� ��� ���� �� ���������.
		///	���������� ������ ������ �������������� � �������������� ������� ���������� ISound3D.
		/// @param	path	-	���� � ����� �����.
		/// @return			-	��������� �� ������ ����������� �����
		virtual IPxSound3D		CreateSoundSource	( const char *path ) = 0;
		
		///	���������� ��������� ������ ISound3D.
		/// @param	sound	-	��������� �� ������ �����.
		//virtual void			DestroySoundSource	( ISound3D *sound ) = 0;

		///	������� ��������� ������, ������������ ��������� ISoundTrack, � ���������� �� ���� ���������.
		///	���������� ������ ������ �������������� � �������������� ������� ���������� ISoundTrack.
		/// @param	path	-	���� � ����� �����.
		/// @return			-	��������� �� ������ ������������ �������������
		virtual IPxSoundTrack	CreateSoundTrack	( const char *path ) = 0;

		///	������� ��������� ������, ������������ ��������� ISoundGeomerty, � ���������� �� ���� ���������.
		///	���������� ������ ������ �������������� � �������������� ������� ���������� ISoundGeomerty.
		/// @param	input_mesh	-	���������
		/// @param	pos			-	��������� ���������
		/// @param	orient		-	���������� ���������
		/// @return			-	��������� �� ������ �������� ���������
		virtual IPxSoundGeometry CreateSoundGeometry	( const IPxTriMesh input_mesh, const EVec4 &pos, const EQuat &orient  ) = 0;
		
		///	���������� ��������� ������ ISound3D.
		/// @param	track	-	��������� �� ������ ������������ �������������.
		//virtual void			DestroySoundTrack	( ISoundTrack *track ) = 0;

		///	���������� ������ ����. 
		///	������������� ��� ����������� �������� ��������������� ������ � ������.
		virtual void			Frame				( void ) = 0;
	
		///	���������� ������ ����. 
		///	������������ ��� ����������� �������������� ������.
		virtual	void			RenderDebug			( void ) = 0;	

		///	�������� ��� ��������� ����.
		/// @param	mute	-	���������� �������������� �� ����.
		virtual	void			SetMute			( bool mute ) = 0;	

		///	������� ���� ������������.
		/// @param	pos	-	���������� ���������
		/// @param	min_distance	-	���������� ����������� ���������
		/// @param	max_distance	-	���������� ������������ ���������
		/// @return		-	��������� �� ������ �������� ������������.
		virtual IPxSoundReverb CreateReverb ( const EVec4& pos, float min_distance, float max_distance, soundReverbType_t type ) = 0;
};