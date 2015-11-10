/*
 * MainMenuScene.cpp
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */


#include "MainMenuScene.h"
#include "TutorialScene.h"
#include "GamePlayScene.h"
#include "GameResultScene.h"
#include "manager/CoordinationManager.h"

using namespace edu_kmu_coordination_solmiengine;
USING_NS_CC;
int MainMenuScene::count = 0;

CCScene* MainMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MainMenuScene *layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

/*
 * 씬 초기화 함수
 */
bool MainMenuScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    //////////////////////////////
   // 한번만 모듈의 연결 함수를 JNI를 이용하여 native call
    if(count<1)
        {
        if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/punchking/PunchKing","connectDevice","()V"))
       				    {
       					  t.env->CallStaticVoidMethod(t.classID, t.methodID);
       					  t.env->DeleteLocalRef(t.classID);
       				    }
        count++;	//카운트 증가
        }
    ////////////////////
   // 타이틀 스프라이트 객체 생성, 애니메이션 객체 생성
    CCAnimation* animation = CCAnimation::create();
    animation->setDelayPerUnit(0.3);
    animation->addSpriteFrameWithFileName("mainmenu/images/title.png");
    animation->addSpriteFrameWithFileName("mainmenu/images/titledark.png");
    title = CCSprite::create("mainmenu/images/title.png");
    title->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TITLE).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_TITLE).y));
    this->addChild(title,3);
    CCAnimate *animate = CCAnimate::create(animation);
    CCAction* action = CCRepeatForever::create(animate);
    title->runAction(action);
    this->setKeypadEnabled(true);

    ////////////////////
   // 배경 스프라이트 객체 생성, 좌표지정 후 씬에 추가
    _background = CCSprite::create("mainmenu/images/titleBg.png");
    _background->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).y) );
    this->addChild(_background,0);

    ////////////////////
   // 게임시작, 비활성 게임시작, 튜토리얼 메뉴 아이템 객체 생성
    _menuGamePlay = CCMenuItemImage::create("mainmenu/images/startBtn2.png","mainmenu/images/startBtn3.png",this,menu_selector(MainMenuScene::menuGamePlayCallBack) );
    _menuTutorial = CCMenuItemImage::create("mainmenu/images/tutorialBtn2.png","mainmenu/images/tutorialBtn3.png",this, menu_selector(MainMenuScene::menuTutorialCallBack));
    _menuGamePlayCouvered = CCMenuItemImage::create("mainmenu/images/startBtn1.png","mainmenu/images/startBtn1.png",this, NULL);
    _tempmenuTutorial = CCMenuItemImage::create("mainmenu/images/tutorialBtn2.png","mainmenu/images/tutorialBtn3.png",this, menu_selector(MainMenuScene::menuTutorialCallBack));

    ////////////////////
   // 각 메뉴 아이템을 포함 시킨 메뉴 객체 생성 후 씬에 추가
    _menu = CCMenu::create(_menuGamePlay, _menuTutorial,NULL);
    _menu->alignItemsVertically();
    _menu->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_BUTTON).x,CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_BUTTON).y));
    _menucovered = CCMenu::create(_menuGamePlayCouvered, _tempmenuTutorial,NULL);
    _menucovered->alignItemsVertically();
    _menucovered->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_BUTTON).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_BUTTON).y));
    this->addChild(_menu, 1);
    this->addChild(_menucovered, 2);

    AudioManager::getAudioManager()->playSound(SOUND_MENUAUDIO);
    this->schedule(schedule_selector(MainMenuScene::checkConnect),1.0f,10,4.0f);

    return true;
}

/*
 * 기기와의 연결을 확인하는 함수
 */
void MainMenuScene::checkConnect(float f)
{
	jboolean connectedjni = false;
	bool connect = false;
	////////////////////
   // getConnected java 함수를 native call하여 boolean값으로 연결이 되었는지를 확인함.
	  if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/punchking/PunchKing","getConnected","()Z"))
	      				    {
		  	  	  	  	  	  connectedjni = (jboolean)t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	      					  connect = (bool)connectedjni;
	      					  t.env->DeleteLocalRef(t.classID);
	      				    }
	  if(connect){					//if(연결이 되었다면)
		this->removeChild(_menucovered);	//비 활성 메뉴를 씬에서 삭제
	  }
}

/*
 * 안드로이드 백버튼 함수 재정의
 */
void MainMenuScene::keyBackClicked(){
	CCDirector::sharedDirector()->end();
}

/*
 * 튜토리얼 메뉴 선택시 호출 되는 콜백 함수.
 */
void MainMenuScene::menuTutorialCallBack(CCObject* pSender){
	AudioManager::getAudioManager()->playEffectAudio(SOUND_CLICK);
	AudioManager::getAudioManager()->stopSound();
	CCScene* scene = TutorialScene::scene();
	CCDirector::sharedDirector()->pushScene(scene);

}

/*
 * 게임 시작 메뉴 선택시 호출 되는 콜백 함수.
 */
void MainMenuScene::menuGamePlayCallBack(CCObject* pSender){
	AudioManager::getAudioManager()->playEffectAudio(SOUND_CLICK);
	AudioManager::getAudioManager()->stopSound();
	CCScene* scene = GamePlayScene::scene();
	CCDirector::sharedDirector()->pushScene(scene);

}

MainMenuScene::MainMenuScene() {
	// TODO Auto-generated constructor stub

}

/*
 * 소멸자 호출 시 java의 disConnect함수를 호출하여 기기와 연결 끊기
 */
MainMenuScene::~MainMenuScene() {
	// TODO Auto-generated destructor stub
	if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/punchking/PunchKing","disconnect","()V"))
								    {
									  t.env->CallStaticVoidMethod(t.classID, t.methodID);
									  t.env->DeleteLocalRef(t.classID);
								    }
	AudioManager::getAudioManager()->stopSound();
}

