

#ifndef RESOURCESID_H_
#define RESOURCESID_H_
//resource type
#include "Manager/ResourcesNameRole.h"


/*
 * 게임에 사용되는 이미지 아이디들
 */
enum idImages{
	//common
	IMAGE_BACKGROUND = TYPE_IMAGE + SCENE_COMMON + STAGE_NONE + 1,
	//mainmenu
	IMAGE_VERTICALLY_BUTTON = TYPE_IMAGE + SCENE_MAINMENU + STAGE_NONE + 1,
	IMAGE_TITLE,
	//tutorial
	IMAGE_TUTORIAL = TYPE_IMAGE + SCENE_TUTORIAL + STAGE_NONE + 1,
	IMAGE_BACK_BOUTTON,
	//gameplay - gameplay
	IMAGE_PUNCHMACHINE = TYPE_IMAGE + SCENE_GAMEPLAY + STAGE_GAMEPLAY + 1,
	IMAGE_STAGE,
	IMAGE_STAGE_NUMBER,
	IMAGE_POWER_TEXT,
	IMAGE_TIMINGBAR,
	IMAGE_TIMING_POINT,
	IMAGE_POWEREFFECT,
	IMAGE_POWERGAUGE,
	IMAGE_POWERGAUGE_BAR,
	IMAGE_POWER_ICON,
	IMAGE_SCORE,
	IMAGE_TOTAL_SCORE,
	IMAGE_SCORENUMBER,
	IMAGE_ACCURACY,
	//gameplay - gameover
	IMAGE_GAMEOVER = TYPE_IMAGE + SCENE_GAMEPLAY + STAGE_GAMEOVER + 1,
	//gameresult
	IMAGE_HORIZONTAL_BUTTON = TYPE_IMAGE + SCENE_GAMERESULT + STAGE_NONE + 1,
	IMAGE_RESULT_WINDOW,

	//gameresultLable
	LABLE_NAME,
	LABLE_STAGE,
	LABLE_SCORE,

	LABLE_FIRST_NAME,
	LABLE_SECOND_NAME,
	LABLE_THIRD_NAME,
	LABLE_FOURTH_NAME,
	LABLE_FIFTH_NAME,

	LABLE_FIRST_STAGE,
	LABLE_SECOND_STAGE,
	LABLE_THIRD_STAGE,
	LABLE_FOURTH_STAGE,
	LABLE_FIFTH_STAGE,

	LABLE_FIRST_SCORE,
	LABLE_SECOND_SCORE,
	LABLE_THIRD_SCORE,
	LABLE_FOURTH_SCORE,
	LABLE_FIFTH_SCORE,
};
enum idSounds{
	//common
	SOUND_CLICK = TYPE_SOUND + SCENE_COMMON + STAGE_NONE + 1,
	//mainmenu
	SOUND_MENUAUDIO = TYPE_SOUND + SCENE_MAINMENU + STAGE_NONE + 1,
	//gameplay - none
	SOUND_PALYAUDIO = TYPE_SOUND + SCENE_GAMEPLAY + STAGE_NONE + 1,
	SOUND_EFFECT_BAD,
	SOUND_EFFECT_GOOD,
	SOUND_EFFECT_GREAT,
	SOUND_EFFECT_EXCEL,
	SOUND_PUNCH_BEGIN,
	//gameplay - gameplay
	SOUND_BLOCKDROP = TYPE_SOUND + SCENE_GAMEPLAY + STAGE_GAMEPLAY + 1,
	//gameplay - gameover
	SOUND_GAMEOVER = TYPE_SOUND + SCENE_GAMEPLAY + STAGE_GAMEOVER + 1,
	SOUND_GAMECLEAR,
	//gameresult
	SOUND_RESULTAUDIO = TYPE_SOUND + SCENE_GAMERESULT + STAGE_NONE + 1,
};


#endif /* RESOURCESID_H_ */

