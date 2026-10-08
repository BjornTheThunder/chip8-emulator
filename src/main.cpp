#include <chrono>
#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include "chip8.hpp"
#include "platform.hpp"

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int videoScale;
	int cycleDelay;
	try
	{
		videoScale = std::stoi(argv[1]);
		cycleDelay = std::stoi(argv[2]);
	}
	catch (std::exception const&)
	{
		std::cerr << "Error: Scale and Delay must be integers.\n";
		std::exit(EXIT_FAILURE);
	}

	if (videoScale <= 0 || cycleDelay < 0)
	{
		std::cerr << "Error: Scale must be > 0 and Delay must be >= 0.\n";
		std::exit(EXIT_FAILURE);
	}

	char const* romFilename = argv[3];

	Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	Chip8 chip8;
	if (!chip8.LoadROM(romFilename))
	{
		std::cerr << "Error: Failed to load ROM: " << romFilename << "\n";
		std::exit(EXIT_FAILURE);
	}

	int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	auto lastTimerTime = lastCycleTime;
	bool quit = false;

	const auto timerInterval = std::chrono::milliseconds(16); // ~60Hz

	while (!quit)
	{
		quit = platform.ProcessInput(chip8.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float cycleElapsed = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (cycleElapsed >= cycleDelay)
		{
			lastCycleTime = currentTime;

			try
			{
				chip8.Cycle();
			}
			catch (std::exception const& e)
			{
				std::cerr << "Emulation error: " << e.what() << "\n";
				break;
			}

			platform.Update(chip8.video, videoPitch);

			currentTime = std::chrono::high_resolution_clock::now();
		}

		if (currentTime - lastTimerTime >= timerInterval)
		{
			lastTimerTime += timerInterval;
			if (currentTime - lastTimerTime > timerInterval * 5)
			{
				lastTimerTime = currentTime;
			}
			chip8.TickTimers();
		}

		// Sleep only when idle long enough to avoid harming cycle rate.
		// SDL_Delay(1) can sleep ~15ms on Windows, so require >2ms spare.
		{
			auto now = std::chrono::high_resolution_clock::now();
			float toNextCycle = static_cast<float>(cycleDelay) - std::chrono::duration<float, std::chrono::milliseconds::period>(now - lastCycleTime).count();
			float toNextTimer = std::chrono::duration<float, std::chrono::milliseconds::period>(lastTimerTime + timerInterval - now).count();
			if (toNextCycle > 2.0f && toNextTimer > 2.0f)
			{
				SDL_Delay(1);
			}
		}
	}

	return 0;
}
