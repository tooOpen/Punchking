/*
 * GameResultScene.cpp
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */


#include "GameResultScene.h"
#include "MainMenuScene.h"
#include "GamePlayScene.h"
#include "manager/CoordinationManager.h"
#include "manager/RankManager.h"
using namespace edu_kmu_coordination_solmiengine;
using namespace com_solmi_happplus_RankManager;
USING_NS_CC;

extern int accumulatedStage;	//플레이 씬에 쌓은 블럭 수 가져오기
extern int finalScore;			//플레이 씬에 점수 가져오기

CCScene* GameResultScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    GameResultScene *layer = GameResultScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameResultScene::init()
{

    if ( !CCLayer::init() )
    {
        return false;
    }
    _accumulatedStage = accumulatedStage;
    _finalScore = finalScore;

    //////////////////////////////
   // getPlayerName함수를 호출 하기 위해 dropking의 activity object의 정보를 가져옴
    if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/punchking/PunchKing", "cppCall_logsth", "()Ljava/lang/Object;"))
    {
     jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

     //////////////////////////////
    // 가져온 activity정보로 dropking 클래스 안에 getPlayerName함수를 호출 하여 사용자의 이름을 가져옴
     if(JniHelper::getMethodInfo(t, "com/solmi/refitgames/punchking/PunchKing", "getplayername", "()Ljava/lang/String;"))
     {
      jstring temp =  (jstring)t.env->CallObjectMethod(jobj, t.methodID); // �샇異쒗빀�땲�떎.
      playerName = JniHelper::jstring2string(temp).c_str();
     }
    }

    //////////////////////////////
   // 사용자 이름의 길이가 6이상이면 6개로 자른다
    if(playerName.length()>6)
        {
        	  CCLog("%d",playerName.length());
        	playerName = playerName.substr(0,6);
        }
    AudioManager::getAudioManager()->playSound(SOUND_RESULTAUDIO);

    ////////////////////
   // 배경 스프라이트 객체 생성, 좌표지정 후 씬에 추가
    _background = CCSprite::create("common/images/background.png");
    _background->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).y));
    this->addChild(_background,0);

    ////////////////////
   // 결과창 스프라이트 객체 생성, 좌표지정 후 씬에 추가
    _resultWindow = CCSprite::create("gameresult/images/result.png");
    _resultWindow->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_RESULT_WINDOW).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_RESULT_WINDOW).y));
    this->addChild(_resultWindow,1);

    ////////////////////
   // 메뉴 돌아가기, 재시작 메뉴 아이템 객체 생성
    _menuReturnMainMenu = CCMenuItemImage::create("gameresult/images/endBtn1.png","gameresult/images/endBtn2.png",
    		this,menu_selector(GameResultScene::menuRetrunMainMenuCallBack) );
    _menuReturnGamePlay = CCMenuItemImage::create("gameresult/images/replayBtn1.png","gameresult/images/replayBtn2.png",
    		this,menu_selector(GameResultScene::menuRetrunGamePlayCallBack) );


    ////////////////////
   // 메뉴아이템 포함 후 씬에 추가
    _resultMenu = CCMenu::create(_menuReturnGamePlay, _menuReturnMainMenu, NULL);
    _resultMenu->alignItemsHorizontally();
    _resultMenu->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_HORIZONTAL_BUTTON).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_HORIZONTAL_BUTTON).y));
    this->addChild(_resultMenu,2);

    ////////////////////
   // 현재 사용자의 정보 출력 레이블 객체 생성
	_nameLabel = CCLabelTTF::create(playerName.c_str(), "fonts/arial.ttf", 60);
	_nameLabel->setColor(ccc3(255, 255, 255));
	_nameLabel->setAnchorPoint(ccp(0,0.5));
	_nameLabel->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_NAME).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_NAME).y));
	this->addChild(_nameLabel, 4);

	_stageLabel = CCLabelTTF::create(
			CCString::createWithFormat("%2d", _accumulatedStage)->getCString(),
			"fonts/arial.ttf", 60);
	_stageLabel->setColor(ccc3(255, 255, 255));
	_stageLabel->setAnchorPoint(ccp(0,0.5));
	_stageLabel->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_STAGE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_STAGE).y));
	this->addChild(_stageLabel, 4);

	_finalScoreLabel = CCLabelTTF::create(
			CCString::createWithFormat("%5d", _finalScore)->getCString(),
			"fonts/arial.ttf", 60);
	_finalScoreLabel->setColor(ccc3(255, 255, 255));
	_finalScoreLabel->setAnchorPoint(ccp(0,0.5));
	_finalScoreLabel->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_SCORE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_SCORE).y));
	this->addChild(_finalScoreLabel, 4);

	char* pStr;
	pStr = const_cast<char*>(playerName.c_str());
	RankManager::getRankManager()->insertData(pStr, _accumulatedStage,
			_finalScore);
	RankManager::getRankManager()->selectData(rankData);

	 ////////////////////
	// 레이블 객체 생성 후 좌표 지정
	_firstName = CCLabelTTF::create(
			CCString::createWithFormat("1.%s", rankData[0].name)->getCString(),
			"fonts/arial.ttf", 60);
	_firstName->setColor(ccc3(255, 255, 255));
	_firstName->setAnchorPoint(ccp(0,0.5));
	_firstName->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FIRST_NAME).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FIRST_NAME).y));

	_secondName = CCLabelTTF::create(
			CCString::createWithFormat("2.%s", rankData[1].name)->getCString(),
			"fonts/arial.ttf", 60);
	_secondName->setColor(ccc3(255, 255, 255));
	_secondName->setAnchorPoint(ccp(0,0.5));
	_secondName->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_SECOND_NAME).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_SECOND_NAME).y));

	_thirdName = CCLabelTTF::create(
			CCString::createWithFormat("3.%s", rankData[2].name)->getCString(),
			"fonts/arial.ttf", 60);
	_thirdName->setColor(ccc3(255, 255, 255));
	_thirdName->setAnchorPoint(ccp(0,0.5));
	_thirdName->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_THIRD_NAME).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_THIRD_NAME).y));

	_fourthName = CCLabelTTF::create(
			CCString::createWithFormat("4.%s", rankData[3].name)->getCString(),
			"fonts/arial.ttf", 60);
	_fourthName->setColor(ccc3(255, 255, 255));
	_fourthName->setAnchorPoint(ccp(0,0.5));
	_fourthName->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FOURTH_NAME).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FOURTH_NAME).y));

	_fifthName = CCLabelTTF::create(
			CCString::createWithFormat("5.%s", rankData[4].name)->getCString(),
			"fonts/arial.ttf", 60);
	_fifthName->setColor(ccc3(255, 255, 255));
	_fifthName->setAnchorPoint(ccp(0,0.5));
	_fifthName->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FIFTH_NAME).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FIFTH_NAME).y));

	_firstStage = CCLabelTTF::create(
			CCString::createWithFormat("%2d", rankData[0].stage)->getCString(),
			"fonts/arial.ttf", 60);
	_firstStage->setColor(ccc3(255, 255, 255));
	_firstStage->setAnchorPoint(ccp(0,0.5));
	_firstStage->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FIRST_STAGE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FIRST_STAGE).y));

	_secondStage = CCLabelTTF::create(
			CCString::createWithFormat("%2d ", rankData[1].stage)->getCString(),
			"fonts/arial.ttf", 60);
	_secondStage->setColor(ccc3(255, 255, 255));
	_secondStage->setAnchorPoint(ccp(0,0.5));
	_secondStage->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_SECOND_STAGE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_SECOND_STAGE).y));

	_thirdStage = CCLabelTTF::create(
			CCString::createWithFormat("%2d", rankData[2].stage)->getCString(),
			"fonts/arial.ttf", 60);
	_thirdStage->setColor(ccc3(255, 255, 255));
	_thirdStage->setAnchorPoint(ccp(0,0.5));
	_thirdStage->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_THIRD_STAGE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_THIRD_STAGE).y));

	_fourthStage = CCLabelTTF::create(
			CCString::createWithFormat("%2d", rankData[3].stage)->getCString(),
			"fonts/arial.ttf", 60);
	_fourthStage->setColor(ccc3(255, 255, 255));
	_fourthStage->setAnchorPoint(ccp(0,0.5));
	_fourthStage->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FOURTH_STAGE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FOURTH_STAGE).y));

	_fifthStage = CCLabelTTF::create(
			CCString::createWithFormat("%2d", rankData[4].stage)->getCString(),
			"fonts/arial.ttf", 60);
	_fifthStage->setColor(ccc3(255, 255, 255));
	_fifthStage->setAnchorPoint(ccp(0,0.5));
	_fifthStage->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FIFTH_STAGE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FIFTH_STAGE).y));

	_firstScore = CCLabelTTF::create(
			CCString::createWithFormat("%4d", rankData[0].score)->getCString(),
			"fonts/arial.ttf", 60);
	_firstScore->setColor(ccc3(255, 255, 255));
	_firstScore->setAnchorPoint(ccp(0,0.5));
	_firstScore->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FIRST_SCORE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FIRST_SCORE).y));

	_secondScore = CCLabelTTF::create(
			CCString::createWithFormat("%4d", rankData[1].score)->getCString(),
			"fonts/arial.ttf", 60);
	_secondScore->setColor(ccc3(255, 255, 255));
	_secondScore->setAnchorPoint(ccp(0,0.5));
	_secondScore->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_SECOND_SCORE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_SECOND_SCORE).y));

	_thirdScore = CCLabelTTF::create(
			CCString::createWithFormat("%4d", rankData[2].score)->getCString(),
			"fonts/arial.ttf", 60);
	_thirdScore->setColor(ccc3(255, 255, 255));
	_thirdScore->setAnchorPoint(ccp(0,0.5));
	_thirdScore->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_THIRD_SCORE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_THIRD_SCORE).y));

	_fourthScore = CCLabelTTF::create(
			CCString::createWithFormat("%4d", rankData[3].score)->getCString(),
			"fonts/arial.ttf", 60);
	_fourthScore->setColor(ccc3(255, 255, 255));
	_fourthScore->setAnchorPoint(ccp(0,0.5));
	_fourthScore->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FOURTH_SCORE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FOURTH_SCORE).y));

	_fifthScore = CCLabelTTF::create(
			CCString::createWithFormat("%4d", rankData[4].score)->getCString(),
			"fonts/arial.ttf", 60);
	_fifthScore->setColor(ccc3(255, 255, 255));
	_fifthScore->setAnchorPoint(ccp(0,0.5));
	_fifthScore->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(LABLE_FIFTH_SCORE).x,CoordinationManager::getCoordinationManager()->getPos(LABLE_FIFTH_SCORE).y));

	switch (RankManager::getRankManager()->getDataRowNum()) {
	case 1:
		this->addChild(_firstName, 4);
		this->addChild(_firstStage, 4);
		this->addChild(_firstScore, 4);
		break;
	case 2:
		this->addChild(_firstName, 4);
		this->addChild(_firstStage, 4);
		this->addChild(_firstScore, 4);
		this->addChild(_secondName, 4);
		this->addChild(_secondStage, 4);
		this->addChild(_secondScore, 4);
		break;
	case 3:
		this->addChild(_firstName, 4);
		this->addChild(_firstStage, 4);
		this->addChild(_firstScore, 4);
		this->addChild(_secondName, 4);
		this->addChild(_secondStage, 4);
		this->addChild(_secondScore, 4);
		this->addChild(_thirdName, 4);
		this->addChild(_thirdStage, 4);
		this->addChild(_thirdScore, 4);
		break;
	case 4:
		this->addChild(_firstName, 4);
		this->addChild(_firstStage, 4);
		this->addChild(_firstScore, 4);
		this->addChild(_secondName, 4);
		this->addChild(_secondStage, 4);
		this->addChild(_secondScore, 4);
		this->addChild(_thirdName, 4);
		this->addChild(_thirdStage, 4);
		this->addChild(_thirdScore, 4);
		this->addChild(_fourthName, 4);
		this->addChild(_fourthStage, 4);
		this->addChild(_fourthScore, 4);
		break;
	case 5:
		this->addChild(_firstName, 4);
		this->addChild(_firstStage, 4);
		this->addChild(_firstScore, 4);
		this->addChild(_secondName, 4);
		this->addChild(_secondStage, 4);
		this->addChild(_secondScore, 4);
		this->addChild(_thirdName, 4);
		this->addChild(_thirdStage, 4);
		this->addChild(_thirdScore, 4);
		this->addChild(_fourthName, 4);
		this->addChild(_fourthStage, 4);
		this->addChild(_fourthScore, 4);
		this->addChild(_fifthName, 4);
		this->addChild(_fifthStage, 4);
		this->addChild(_fifthScore, 4);
		break;
	default:
		this->addChild(_firstName, 4);
		this->addChild(_firstStage, 4);
		this->addChild(_firstScore, 4);
		this->addChild(_secondName, 4);
		this->addChild(_secondStage, 4);
		this->addChild(_secondScore, 4);
		this->addChild(_thirdName, 4);
		this->addChild(_thirdStage, 4);
		this->addChild(_thirdScore, 4);
		this->addChild(_fourthName, 4);
		this->addChild(_fourthStage, 4);
		this->addChild(_fourthScore, 4);
		this->addChild(_fifthName, 4);
		this->addChild(_fifthStage, 4);
		this->addChild(_fifthScore, 4);
		break;
	}
	return true;
}
void GameResultScene::menuRetrunMainMenuCallBack(CCObject* pSender){
	AudioManager::getAudioManager()->playEffectAudio(SOUND_CLICK);
	AudioManager::getAudioManager()->playSound(SOUND_MENUAUDIO);
	CCDirector::sharedDirector()->popScene();
}

/*
 * 메인 메뉴로 돌아간뒤 게임 씬으로 감.
 */
void GameResultScene::menuRetrunGamePlayCallBack(CCObject* pSender){
	AudioManager::getAudioManager()->playEffectAudio(SOUND_CLICK);
	CCDirector::sharedDirector()->popScene();
	CCScene* scene = GamePlayScene::scene();
	CCDirector::sharedDirector()->pushScene(scene);
}
GameResultScene::GameResultScene() {
	// TODO Auto-generated constructor stub

}

GameResultScene::~GameResultScene() {
	// TODO Auto-generated destructor stub
}

