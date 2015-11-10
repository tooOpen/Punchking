/*
 * AudioManager.cpp
 *
 *  Created on: 2014. 2. 7.
 *      Author: bsh
 */

#include "AudioManager.h"
#include "../ResourcesID.h"
namespace edu_kmu_audio_solmiengine {

AudioManager::AudioManager() {
	// TODO Auto-generated constructor stub

}

AudioManager::~AudioManager() {
	// TODO Auto-generated destructor stub
}

/*
 * id에 맞는 효과음을 재생하는 함수
 */
void AudioManager::playEffectAudio(int id){
	switch(id){
	case SOUND_CLICK:
		SimpleAudioEngine::sharedEngine()->playEffect("common/sounds/click.ogg");
		break;
	case SOUND_EFFECT_BAD:
		SimpleAudioEngine::sharedEngine()->playEffect("gameplay/sounds/powEffectBad.ogg");
		break;
	case SOUND_EFFECT_GOOD:
		SimpleAudioEngine::sharedEngine()->playEffect("gameplay/sounds/powEffectGood.ogg");
		break;
	case SOUND_EFFECT_GREAT:
		SimpleAudioEngine::sharedEngine()->playEffect("gameplay/sounds/powEffectGreat.ogg");
		break;
	case SOUND_EFFECT_EXCEL:
		SimpleAudioEngine::sharedEngine()->playEffect("gameplay/sounds/powEffectExcel.ogg");
		break;
	case SOUND_PUNCH_BEGIN:
		SimpleAudioEngine::sharedEngine()->playEffect("gameplay/sounds/punchBegin.ogg");
		break;
	case SOUND_GAMEOVER:
		SimpleAudioEngine::sharedEngine()->playEffect("gameplay/sounds/gameOver.ogg");
		break;
	case SOUND_GAMECLEAR:
		SimpleAudioEngine::sharedEngine()->playEffect("gameplay/sounds/gameClear.ogg");
		break;
	default:

		break;
	}
	//
}



AudioManager* AudioManager::s_Audiomanager;


/*
 * id에 맞는 배경음을 재생하는 함수
 */
void AudioManager::playSound(int id){
	switch(id){
	case SOUND_MENUAUDIO:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("mainmenu/sounds/menuAudio.ogg",true);
		break;
	case SOUND_PALYAUDIO:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("gameplay/sounds/playAudio.ogg",true);
		break;
	case SOUND_RESULTAUDIO:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("gameresult/sounds/resultAudio.ogg",false);
		break;
	default:

		break;
	}
}
bool AudioManager::isPlaySound()
{
	return SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
}
void AudioManager::pauseSound(){
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}
void AudioManager::stopSound(){
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
void AudioManager::resumeSound(){
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
void AudioManager::preloadSound(int id){

}
void AudioManager::clearSound(int id){

}

} /* namespace edu_kmu_audio_solmiengine */
