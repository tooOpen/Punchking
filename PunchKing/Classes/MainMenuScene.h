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
	static int count;		// �޴� ���� ���ö� �ѹ��� ������ ����ǰ� �ϱ����� ����
	CREATE_FUNC(MainMenuScene);
	JniMethodInfo t;		//JNI �޼ҵ� ���� ����
	CCSprite* _background;	//��� ��������Ʈ
	CCSprite* _table;		//��� ��������Ʈ 2
	CCSprite* title;		//Ÿ��Ʋ ��������Ʈ
	CCMenuItemImage* _menuTutorial;		//Ʃ�丮�� �޴�������
	CCMenuItemImage* _menuGamePlay;		//���ӽ��� �޴�������
	CCMenuItemImage* _tempmenuTutorial;
	CCMenuItemImage* _menuGamePlayCouvered;	//�� Ȱ��ȭ �� ���ӽ��� �޴�������
	CCMenu* _menu;
	CCMenu* _menucovered;

	virtual void keyBackClicked();
	void menuTutorialCallBack(CCObject* pSender);
	void menuGamePlayCallBack(CCObject* pSender);
	void checkConnect(float f);
};

#endif /* MAINMENUSCENE_H_ */
