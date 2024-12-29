#include "Audio.h"

void Audio::initial()
{
	result = FMOD::System_Create(&system);
	result = system->init(32, FMOD_INIT_NORMAL, extraDriverdata);
}

void Audio::playSound1()
{
	if(!isPlayed){
	result = system->playSound(sound1, 0, true, &channel);
	channel->setPitch(1);
	channel->setPaused(false);
	isPlayed = true;
	}
}

void Audio::playSound2()
{
	if (!isPlayed) {
		result = system->playSound(sound2, 0, true, &channel);
		channel->setPitch(1.5);
		channel->setPaused(false);
		isPlayed = true;
	}
}

void Audio::playSound3()
{
	if (!isPlayed) {
		result = system->playSound(sound3, 0, true, &channel);
		channel->setVolume(2);
		channel->setPaused(false);
		isPlayed = true;
	}
}

void Audio::loadSound()
{
	result = system->createSound("walk.mp3", FMOD_DEFAULT, 0, &sound1);
	result = sound1->setMode(FMOD_LOOP_NORMAL);

	result = system->createSound("attack.wav", FMOD_DEFAULT, 0, &sound2);
	result = sound2->setMode(FMOD_LOOP_OFF);

	result = system->createSound("spin.wav", FMOD_DEFAULT, 0, &sound3);
	result = sound3->setMode(FMOD_LOOP_NORMAL);
}

void Audio::updateSound()
{
	result = system->update();
}

Audio::Audio()
{
}

Audio::~Audio()
{
	system->release();
}

void Audio::stopMusic() {
	channel->stop();
	channel = nullptr;
	isPlayed = false;
}