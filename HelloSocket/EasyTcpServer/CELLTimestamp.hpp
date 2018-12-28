#ifndef _CELLTimestamp_hpp_
#define _CELLTimestamp_hpp_

#ifdef USING_WINDOWS_CLOCK
	#include <windows.h>
#else
	#include<chrono>
	using namespace std::chrono;
#endif


class CELLTimestamp
{
#ifdef USING_WINDOWS_CLOCK

public:
	CELLTimestamp()
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startCount);
	}
	~CELLTimestamp()
	{}

	void    update()
	{
		QueryPerformanceCounter(&_startCount);
	}
	/**
	*   获取当前秒
	*/
	double getElapsedSecond()
	{
		return  getElapsedTimeInMicroSec() * 0.000001;
	}
	/**
	*   获取毫秒
	*/
	double getElapsedTimeInMilliSec()
	{
		return this->getElapsedTimeInMicroSec() * 0.001;
	}
	/**
	*   获取微妙
	*/
	long long getElapsedTimeInMicroSec()
	{
		LARGE_INTEGER endCount;
		QueryPerformanceCounter(&endCount);

		double  startTimeInMicroSec =   _startCount.QuadPart * (1000000.0 / _frequency.QuadPart);
		double  endTimeInMicroSec   =   endCount.QuadPart * (1000000.0 / _frequency.QuadPart);

		return  endTimeInMicroSec - startTimeInMicroSec;
	}
protected:
	LARGE_INTEGER   _frequency;
	LARGE_INTEGER   _startCount;

#else

public:
	CELLTimestamp()
	{
		update();
	}
	~CELLTimestamp()
	{}

	void    update()
	{
		_begin = high_resolution_clock::now();
	}
	/**
	*   获取当前秒
	*/
	double getElapsedSecond()
	{
		return  getElapsedTimeInMicroSec() * 0.000001;
	}
	/**
	*   获取毫秒
	*/
	double getElapsedTimeInMilliSec()
	{
		return this->getElapsedTimeInMicroSec() * 0.001;
	}
	/**
	*   获取微妙
	*/
	long long getElapsedTimeInMicroSec()
	{
		return duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();
	}
protected:
	time_point<high_resolution_clock> _begin;
#endif

};

#endif // !_CELLTimestamp_hpp_