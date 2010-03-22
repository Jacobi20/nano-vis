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
///	Интерфейс к объекту трехмерного звука.
///	Типичное применение - звуки выстрелов, ракеты, звуки окружающего мира.
///	В системе может присутствовать несколько объектов ISound3D
///
class ISound3D : public ILocatable {
	public:
		virtual					~ISound3D	( void ) {}

		///	Начинает воспроиведение звука. 
		///	Изначально звук не воспроивзодится.
		///	Возможно указывать режим воспроизведения - зацикленный и однократный.
		///	Слудет отметить, что после однократного воспроизведения, объект не должен уничтожаться,
		///	и должна присутствовать возможность осуществить воспроизведение еще раз.
		///	Если в момент вызова функции ISound::Play воспроизведение уже осуществлялось, то звук должен начать воспроизведение сначала, 
		///	с новым параметром кругового посроизведения.
		/// @param	loop	-	определяет режим воспроиведения звука. Однократный или зацикленный.
		virtual void			Play			( bool loop ) = 0;
		
		///	Останавливает воспроизведение звука. Следует отметить, что после остановки звук не должен уничтожаться.
		/// Т.е. должна иметься возможность начать воспроиведение заново.
		virtual void			Stop			( void ) = 0;
		
		///	Приостанавливает вопсроизведение звука.
		///	При вызове Play - воспроизведение начинается с последеней точки воспроизведения 
		virtual void			Pause			( void ) = 0;

		///	Проверяет, воспроизводится ли в данный момент звук.		
		/// @return			-	воспроизведение.
		virtual bool			IsPlaying		( void ) = 0;
		
		/// Устанавливает громкость звука. 
		///	Значение по умолчанию 1.0.
		///	@param	volume	-	громкость звука. Диапазон значений [0..1].
		virtual void			SetVolume		( float volume ) = 0;
		
		/// Устанавливает относительную высоту звука. 
		///	Например, если значение равно 2, то звук воспроизводится в 2 раза быстрее, 
		///	и он имеет в два раза большую относительную высоту.
		///	Значение по умолчанию 1.0. 
		///	@param	pitch	-	относительная высота звука. Диапазон значений [0.5..2.0]
		///						(значения следует уточнить!)
		virtual void			SetPitch		( float pitch ) = 0;

		///	Включает или выключает звук.
		/// @param	mute	-	определяет воспроизводить ли звук.
		virtual	void			SetMute			( bool mute ) = 0;	
	};


///
///	Интерфейс к объекту музыкального сопровождения.
///	Типичное применение - фоновая музыка.
///	В системе может присутствовать несколько объектов ISoundTrack.
///	Формальное отличие от ISound3D и 2D-звуковых файлов, вопсроизовдимых функцией ISoundSystem::Play2DSound
///	заключается в том, что звук является 3-мерным, и воспроизводимым потоково. 
///	Т.е. звук загружается постепенно по мере воспроизведения.
///
class ISoundTrack : public IDisposable {
	public:
		virtual					~ISoundTrack		( void ) {}
		
		///	Начинает вопсроизведение музыки.
		///	По умолчанию, звук не воспроизводится.
		virtual	void			Play			( void ) = 0;
		
		///	Приостанавливает вопсроизведение музыки.
		///	При вызове Play - воспроизведение начинается с последенего места воспроизведения 
		virtual void			Pause			( void ) = 0;
		
		///	Останавливает воспроизведение мызки.
		///	При вызове Play - воспроизведение начинается с начала звука.
		virtual void			Stop			( void ) = 0;
		
		/// Устанавливает громкость звука. 
		///	Значение по умолчанию 1.0.
		///	@param	volume	-	громкость звука. Диапазон значений [0..1].
		virtual void			SetVolume		( float volume ) = 0;
		
		///	Возваращает длину звука в секундах.
		///	@return			-	длина звука в секундах, значение может быть дробным.
		virtual float			GetLength		( void ) = 0;
		
		///	Устанавливает текующую точку воспроизведения.
		///	Точка воспрозивдение по умолчанию - 0.0.
		///	@param	time	-	устанавливает точку воспроизведения.
		virtual void			SetPos			( float time ) = 0;

		///	Возвращает текующую точку воспроизведения.
		///	@return	time	-	текущая точка воспроизведения.
		virtual float			GetPos			( void ) = 0;

		///	Включает или выключает звук.
		/// @param	mute	-	определяет воспроизводить ли звук.
		virtual	void			SetMute			( bool mute ) = 0;	
	};


///
///	Интерфейс к объекту звуковой системы
///	Является синглетном.
///	Звуковая система предназначена для управления и воспроизведения 
///	2D- и 3D-звуков и музыкального сопровождения, 
///	а также настройки акустических свойств окружающей среды. 
///	Для 3D-операций используется правосторонняя система координат.
///
class ISoundSystem : public IDisposable {
	public:
		virtual					~ISoundSystem		( void ) {}

		///	Удаляет все загруженные звуки, музыку и т.п. 
		///	Используется для освобождения памяти перед выгрузкой уровня и загрузкой нового.
		/// Все указатели, после вызова этой функции становятся некорректными.
		virtual void			FreeSounds			( void ) = 0;

		///	Выполняет воспроизведение 2D-звука. 
		///	Основное назначение – воспроизведение звуков пользовательского интерфейса (UI, HUD и т.п.).
		/// @param	path	-	путь к файлу звука.
		/// @param	volume	-	значение громкости воспроизводимого звука. Диапазон значений [0..1].
		/// @param	panning -	стереофоническое положение звука. Диапазон значений [-1..1]
		virtual void			Play2DSound			( const char *path, float volume, float panning ) = 0;

		///	Устанавливает параметры слушателя.
		///			
		///	@param	pos		-	позиция слушателя
		///	@param	view	-	вектор направления, куда смотрит слушатель
		///	@param	up		-	условный "верх" слушателя
		///	@param	vel		-	скорость движеия слушателя
		virtual void			SetListener			( const EVec4 &pos, const EVec4 &view, const EVec4 &up, const EVec4 &vel ) = 0;

		///	Создает экземпляр класса, реализующего интерфейс ISound3D, и возвращает на него указатель.
		///	Созданный пространственный звук не воспроизводится, и находится в положении (0,0,0). Т.е. ни как себя не проявляет.
		///	Дальнейшая работа должна осуществляться с использованием методов интерфейса ISound3D.
		/// @param	path	-	путь к файлу звука.
		/// @return			-	указатель на объект трехмерного звука
		virtual IPxSound3D		CreateSoundSource	( const char *path ) = 0;
		
		///	Уничтожает экземпляр класса ISound3D.
		/// @param	sound	-	указатель на объект звука.
		//virtual void			DestroySoundSource	( ISound3D *sound ) = 0;

		///	Создает экземпляр класса, реализующего интерфейс ISoundTrack, и возвращает на него указатель.
		///	Дальнейшая работа должна осуществляться с использованием методов интерфейса ISoundTrack.
		/// @param	path	-	путь к файлу звука.
		/// @return			-	указатель на объект музыкального сопровождения
		virtual IPxSoundTrack	CreateSoundTrack	( const char *path ) = 0;

		///	Создает экземпляр класса, реализующего интерфейс ISoundGeomerty, и возвращает на него указатель.
		///	Дальнейшая работа должна осуществляться с использованием методов интерфейса ISoundGeomerty.
		/// @param	input_mesh	-	геометрия
		/// @param	pos			-	положение геометрии
		/// @param	orient		-	ориентация геометрии
		/// @return			-	указатель на объект звуковой геометрии
		virtual IPxSoundGeometry CreateSoundGeometry	( const IPxTriMesh input_mesh, const EVec4 &pos, const EQuat &orient  ) = 0;
		
		///	Уничтожает экземпляр класса ISound3D.
		/// @param	track	-	указатель на объект музыкального сопровождения.
		//virtual void			DestroySoundTrack	( ISoundTrack *track ) = 0;

		///	Вызывается каждый кадр. 
		///	Используетися для покадрового контроля воспроизведения звуков и музыки.
		virtual void			Frame				( void ) = 0;
	
		///	Вызывается каждый кадр. 
		///	Используется для отображения местоположения звуков.
		virtual	void			RenderDebug			( void ) = 0;	

		///	Включает или выключает звук.
		/// @param	mute	-	определяет воспроизводить ли звук.
		virtual	void			SetMute			( bool mute ) = 0;	

		///	Создает зону реверберации.
		/// @param	pos	-	определяет положение
		/// @param	min_distance	-	определяет минимальную дистанцию
		/// @param	max_distance	-	определяет максимальную дистанцию
		/// @return		-	указатель на объект звуковой реверберации.
		virtual IPxSoundReverb CreateReverb ( const EVec4& pos, float min_distance, float max_distance, soundReverbType_t type ) = 0;
};