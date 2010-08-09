#ifndef ERRORSTREAM_H
#define ERRORSTREAM_H

#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif

class ErrorStream : public NxUserOutputStream
	{
	public:
	void reportError(NxErrorCode e, const char* message, const char* file, int line)
		{
		const char *error = "unknown error";
		switch (e) {
			case NXE_INVALID_PARAMETER:	error	= "invalid parameter"	;	break;
			case NXE_INVALID_OPERATION:	error	= "invalid operation"	;	break;
			case NXE_OUT_OF_MEMORY:		error	= "out of memory"		;	break;
			case NXE_DB_INFO:			error	= "info"				;	break;
			case NXE_DB_WARNING:		error	= "warning"				;	break;
			}

		LOG_ERROR("NX: %s (%d) :", file, line);
		LOG_ERROR("    %s : %s", error, message);
		}

	NxAssertResponse reportAssertViolation(const char* message, const char* file, int line)
		{
		LOG_ERROR("NX: access violation : %s (%s line %d)\n", message, file, line);
#ifdef WIN32
		switch (MessageBox(0, message, "AssertViolation, see console for details.", MB_ABORTRETRYIGNORE))
			{
			case IDRETRY:
				return NX_AR_CONTINUE;
			case IDIGNORE:
				return NX_AR_IGNORE;
			case IDABORT:
			default:
				return NX_AR_BREAKPOINT;
			}
#elif LINUX
		assert(0);
#elif _XBOX
		return NX_AR_BREAKPOINT;
#elif __CELLOS_LV2__
		return NX_AR_BREAKPOINT;
#endif
		}

	void print(const char* message)
		{
		LOGF(message);
		}
	};



#endif
