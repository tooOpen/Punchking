/*
 * MainMenuScene.h
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */

#ifndef MAINMENUSCENE_H_
#define MAINMENUSCENE_H_

#include "cocos2d.h"
#include "Manager/AudioManager.h"
#include "platform/android/jni/JniHelper.h"
using namespace edu_kmu_audio_solmiengine;
using namespace cocos2d;

class MainMenuScene :public CCLayer {
public:
	MainMenuScene();
	virtual ~MainMenuScene();

	virtual bool init();
	static CCScene* scene();
	static int count;		// 메뉴 씬에 들어올때 한번만 기기와의 연결되게 하기위한 변수
	CREATE_FUNC(MainMenuScene);
	JniMethodInfo t;		//JNI 메소드 정보 변수
	CCSprite* _background;	//배경 스프라이트
	CCSprite* _table;		//배경 스프라이트 2
	CCSprite* title;		//타이틀 스프라이트
	CCMenuItemImage* _menuTutorial;		//튜토리얼 메뉴아이템
	CCMenuItemImage* _menuGamePlay;		//게임시작 메뉴아이템
	CCMenuItemImage* _tempmenuTutorial;
	CCMenuItemImage* _menuGamePlayCouvered;	//비 활성화 된 게임시작 메뉴아이템
	CCMenu* _menu;
	CCMenu* _menucovered;

	virtual void keyBackClicked();
	void menuTutorialCallBack(CCObject* pSender);
	void menuGamePlayCallBack(CCObject* pSender);
	void checkConnect(float f);
};

#endif /* MAINMENUSCENE_H_ */
