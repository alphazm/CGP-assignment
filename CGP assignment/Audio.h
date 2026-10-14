#pragma once
#include "fmod.hpp"

class Audio
{
public:
	FMOD::System* system;
	FMOD::Sound* sound1, * sound2, * sound3;
	FMOD::Channel* channel = 0;
	FMOD_RESULT result;
	void* extraDriverdata = 0;
	bool isPlayed;

	void initial();
	void playSound1();
	void playSound2();
	void playSound3();
	void loadSound();
	void updateSound();
	void stopMusic();

	Audio();
	~Audio();
	
};

