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

// i_ui.h - 

class IFont;
class IUIFrame;

typedef hard_ref<IFont>		IPxFont;
typedef hard_ref<IUIFrame>	IPxUIFrame;


const uint UI_MOUSE_LEFT	=	0x0001;
const uint UI_MOUSE_RIGHT	=	0x0002;
const uint UI_MOUSE_MIDDLE	=	0x0004;

const uint UI_ALIGNH_LEFT	=	0x0001;
const uint UI_ALIGNH_RIGHT	=	0x0002;
const uint UI_ALIGNH_CENTER	=	0x0004;
const uint UI_ALIGNH_FILL	=	0x0008;

const uint UI_ALIGNV_TOP	=	0x0010;
const uint UI_ALIGNV_BOTTOM	=	0x0020;
const uint UI_ALIGNV_CENTER	=	0x0040;
const uint UI_ALIGNV_FILL	=	0x0080;

struct uiRect_s {
		uiRect_s(void) { x = y = w = h = 0; }
		uiRect_s(float x, float y, float w, float h) {
			this->x = x;	this->y = y;
			this->w = w;	this->h = h;
		}
		float x, y, w, h;
	};



class IFont : public IDisposable {
	public:
		virtual				~IFont			( void ) {}
		virtual const char	*Name			( void ) const = 0;
		virtual const char	*GetFontImage	( void ) const = 0;
		virtual float		GetLineHeight	( void ) const = 0;
		virtual float		GetBaseLine		( void ) const = 0;
						
		virtual float		GetXAdvance		( uchar ch ) const = 0;
		virtual EVec2		GetOffset		( uchar ch ) const = 0;
		virtual EVec2		GetUV0			( uchar ch ) const = 0;
		virtual EVec2		GetUV1			( uchar ch ) const = 0;
		virtual EVec2		GetGlyphSize	( uchar ch ) const = 0;
		virtual float		GetKerning		( uchar first, char second ) const = 0;
	};
	

class IUIFrame: public IDisposable {
	public:
		virtual				~IUIFrame			( void ) {};

		//	Message stuff :		
		virtual void		FMKeyPress			( uint code ) = 0;
		virtual void		FMKeyDown			( uint code ) = 0;
		virtual void		FMPaint				( void ) = 0;
		virtual void		FMMouseUp			( uint button, float x, float y ) = 0;
		virtual void		FMMouseDown			( uint button, float x, float y ) = 0;
		virtual void		FMMouseClick		( uint button, float x, float y ) = 0;
		virtual void		FMMouseIn			( void ) = 0;
		virtual void		FMMouseOut			( void ) = 0;
		virtual void		FMMouseMove			( uint button, float x, float y ) = 0;
		virtual void		FMProcess			( float delta, float time ) = 0;
		virtual void		FMResize			( float new_width, float new_height ) = 0;
		virtual void		FMMove				( float new_x, float new_y ) = 0;
		virtual	void		FMShowFrame			( void ) = 0;
		virtual void		FMHideFrame			( void ) = 0;
		
		//	Frame property stuff :
		virtual IPxUIFrame	GetClientArea		( void ) = 0;
		virtual bool		IsVisible			( void ) const = 0;
		virtual bool		IsEnabled			( void ) const = 0;
		virtual bool		IsDraggable			( void ) const = 0;
		virtual bool		IsSizeable			( void ) const = 0;
		virtual uiRect_s	GetFrameRect		( void ) const = 0;
		virtual float		GetFrameOpacity		( void ) const = 0;
		virtual void		GetScrollOffset		( float &dx, float &dy ) = 0;
		virtual const char	*GetFrameText		( void ) const = 0;
	};


class IUIHook {
	public:
		virtual				~IUIHook	( void ) {}
		virtual void		Hook		( uint code  ) = 0;
		virtual	uint		GetHook		( void ) = 0;
		virtual	IPxUIFrame	GetFrame	( void ) = 0;
		virtual	void		Clear		( void ) = 0;
	};


class	IUserInterface : public IDisposable {
	public:
							~IUserInterface		( void ) {}
						
		//	UI rendering API :
		virtual void		DrawStringMT		( float x, float y, float size, float tracking, const char *font_raster, const char *string, const EVec4 &color ) = 0;
		virtual void		DrawStringMTClr		( float x, float y, float size, float tracking, const char *font_raster, const char *string ) = 0;
		virtual void		DrawStringFont		( float x, float y, const IPxFont font, const char *string, const EVec4 &color=EVec4(1,1,1,1) ) = 0;
		virtual void		DrawTextInRect		( uiRect_s &r, const IPxFont font, const char *text, uint align, const EVec4 &color=EVec4(1,1,1,1) ) = 0;
		virtual float		StringFontLength	( const IPxFont font, const char *string ) = 0;
		virtual void		DrawRect			( float x, float y, float w, float h, const char *image, const EVec4 &color ) = 0;
		virtual void		DrawBorderRect		( float x, float y, float w, float h, const char *image, const EVec4 &color ) = 0;
		virtual void		DrawBorderRectTop	( float x, float y, float w, float h, const char *image, const EVec4 &color ) = 0;
		virtual void		DrawBorderRectBottom( float x, float y, float w, float h, const char *image, const EVec4 &color ) = 0;
		virtual IPxFont		RegisterFont		( const char *path ) = 0;

		//	UI frame management :
		virtual void		AttachFrame			( IPxUIFrame parent, IPxUIFrame frame ) = 0;
		virtual void		DetachFrame			( IPxUIFrame frame ) = 0;
		virtual uint		GetFrameChildNum	( IPxUIFrame frame ) = 0;
		virtual IPxUIFrame	GetFrameChild		( IPxUIFrame frame, uint n ) = 0;
		virtual IPxUIFrame	GetFrameParent		( IPxUIFrame frame ) = 0;

		virtual void		SetForegroundFrame	( IPxUIFrame frame ) = 0;
		virtual void		SetActiveFrame		( IPxUIFrame frame ) = 0;
		virtual void		ShowFrame			( IPxUIFrame frame ) = 0;
		virtual void		HideFrame			( IPxUIFrame frame ) = 0;
		virtual uiRect_s	GetDragRect			( void ) = 0;
		virtual IPxUIFrame	GetDraggingFrame	( void ) = 0;
		virtual IPxUIFrame	GetFrameUnderCursor( void ) = 0;
		virtual IPxUIFrame	GetActiveFrame		( void ) = 0;
		virtual void		RenderFrames		( void ) = 0;
		virtual void		ProcessFrames		( void ) = 0;
		virtual void		GetAbsolutePosition	( IPxUIFrame frame, float &x, float &y ) = 0;
		
		//	UI feedback stuff :		
		virtual void		KeyPress			( uint code ) = 0;
		virtual void		KeyDown				( uint code ) = 0;
		virtual void		MouseMove			( const mouseInput_s *mouse_input ) = 0;
		
		//	UI hook stuff :	
		virtual IUIHook	*	GetKeyboardHook			( void ) = 0;
		virtual void		SetKeyboardHook			( IUIHook * hook ) = 0;
		virtual IUIHook	*	GetMouseHook			( void ) = 0;
		virtual void		SetMouseHook			( IUIHook * hook ) = 0;
	};
	
	
	
	
	