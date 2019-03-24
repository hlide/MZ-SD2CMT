#pragma once

#define HAS_LCD16X2_DISPLAY 0
#define HAS_OLED128X32_DISPLAY 1

extern unsigned long progress_size;
extern bool ultrafast_enabled;

enum class DisplayCode : int8_t
{
	none,

	no_sdcard,

	set_entry_name,
	scroll_entry_name,

	startPlaying,
	stopPlaying,
	pausePlaying,
	resumePlaying,
	cancelPlaying,

	updateProgressBar
};

struct DummyDisplay
{
	static void displayCode(DisplayCode code) {}
	static void setup() {}
	static void configure() {}
};

#define SCROLL_SPEED  250    // Text scroll delay
#define SCROLL_WAIT   3000   // Delay before scrolling starts

#include "mz-sd2cmt.display.lcd16x2.h"
#include "mz-sd2cmt.display.oled128x32.h"
#include "mz-sd2cmt.display.serial.h"

template<typename Head, typename... Rest> struct DisplaySelector
{
	static void displayCode(DisplayCode code)
	{
		Head::displayCode(code);
		DisplaySelector<Rest...>::displayCode(code);
	}

	static void setup()
	{
		Head::setup();
		InputReaderSelector<Rest...>::setup();
	}

	static void configure()
	{
		Head::configure();
		InputReaderSelector<Rest...>::configure();
	}
};

template<typename Tail> struct DisplaySelector< Tail >
{
	static void displayCode(DisplayCode code)
	{
		return Tail::displayCode(code);
	}

	static void setup()
	{
		Tail::setup();
	}

	static void configure()
	{
		Tail::configure();
	}
};

using Display = DisplaySelector< LCD16x2Display, OLED128x32Display, SerialDisplay >;
