#pragma once

#define	STRINGIZE(x)	#x

#ifdef __ANDROID__

#define	SHADER(shader) "#version 300 es\n" STRINGIZE(shader)

#else

#define	SHADER(shader) "#version 330\n" STRINGIZE(shader)

#endif
