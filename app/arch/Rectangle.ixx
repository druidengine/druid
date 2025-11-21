module;

#include <raylib.h>

export module rectangle;

export namespace druid::arch
{
	struct Rectangle
	{
		int x;
		int y;
		int width;
		int height;
		Color color;
	};
}