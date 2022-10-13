#pragma once
#include <string>

struct Window
{
	explicit Window( const std::string& title = "Title", float width = 320.0f, 
		float height = 180.0f, bool isVSyncOn = true );

	std::string title;
	float width;
	float height;
	bool isVSyncOn;
};


struct Point2f
{
	Point2f( );
	explicit Point2f( float x, float y );

	float x;
	float y;
};



struct Rectf
{
	Rectf( );
	explicit Rectf( float left, float bottom, float width, float height );
	explicit Rectf(Point2f leftBottom, float width, float height);
	
	float left;
	float bottom;
	float width;
	float height;

};


struct Color4f
{
	Color4f( );
	explicit Color4f( float r, float g, float b, float a );
	
	float r;
	float g;
	float b;
	float a;
};

struct Circlef
{
	Circlef( );
	explicit Circlef( const Point2f& center, float radius );
	explicit Circlef( float centerX, float centerY, float radius );

	Point2f center;
	float radius;
};


struct Ellipsef
{
	Ellipsef( );
	explicit Ellipsef( const Point2f& center, float radiusX, float radiusY );
	explicit Ellipsef( float centerX, float centerY, float radiusX, float radiusY );

	Point2f center;
	float radiusX;
	float radiusY;
};

struct Offset2f
{
	Offset2f();
	explicit Offset2f(float x, float y);

	float x;
	float y;
};


struct Time
{
	unsigned int hours;
	unsigned int minutes;
	unsigned int seconds;

	void AddTime(Time time)
	{
		hours += time.hours;
		minutes += time.minutes;
		if (minutes >= 60)
		{
			++hours;
			minutes = minutes - 60;
		}
		seconds += time.seconds;
		if (seconds >= 60)
		{
			++minutes;
			seconds = seconds - 60;
		}
	}
	void RestartTime()
	{
		hours = 0;
		minutes = 0;
		seconds = 0;
	}
};
