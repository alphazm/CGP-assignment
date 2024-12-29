//#include "Audio.h"
//
//void Audio::initial()
//{
//	result = FMOD::System_Create(&system);
//	result = system->init(32, FMOD_INIT_NORMAL, extraDriverdata);
//}
//
//void Audio::playSound1()
//{
//	result = system->playSound(sound1, 0, false, &channel);
//}
//
//void Audio::loadSound()
//{
//	result = system->createSound("", FMOD_DEFAULT, 0, &sound1);
//	result = sound1->setMode(FMOD_LOOP_OFF);
//}
//
//void Audio::updateSound()
//{
//	result = system->update();
//}
//
//Audio::Audio()
//{
//}
//
//Audio::~Audio()
//{
//}
