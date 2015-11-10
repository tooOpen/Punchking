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
 * �� �ʱ�ȭ �Լ�
 */
bool MainMenuScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    //////////////////////////////
   // �ѹ��� ����� ���� �Լ��� JNI�� �̿��Ͽ� native call
    if(count<1)
        {
        if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/punchking/PunchKing","connectDevice","()V"))
       				    {
       					  t.env->CallStaticVoidMethod(t.classID, t.methodID);
       					  t.env->DeleteLocalRef(t.classID);
       				    }
        count++;	//ī��Ʈ ����
        }
    ////////////////////
   // Ÿ��Ʋ ��������Ʈ ��ü ����, �ִϸ��̼� ��ü ����
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
   // ��� ��������Ʈ ��ü ����, ��ǥ���� �� ���� �߰�
    _background = CCSprite::create("mainmenu/images/titleBg.png");
    _background->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).y) );
    this->addChild(_background,0);

    ////////////////////
   // ���ӽ���, ��Ȱ�� ���ӽ���, Ʃ�丮�� �޴� ������ ��ü ����
    _menuGamePlay = CCMenuItemImage::create("mainmenu/images/startBtn2.png","mainmenu/images/startBtn3.png",this,menu_selector(MainMenuScene::menuGamePlayCallBack) );
    _menuTutorial = CCMenuItemImage::create("mainmenu/images/tutorialBtn2.png","mainmenu/images/tutorialBtn3.png",this, menu_selector(MainMenuScene::menuTutorialCallBack));
    _menuGamePlayCouvered = CCMenuItemImage::create("mainmenu/images/startBtn1.png","mainmenu/images/startBtn1.png",this, NULL);
    _tempmenuTutorial = CCMenuItemImage::create("mainmenu/images/tutorialBtn2.png","mainmenu/images/tutorialBtn3.png",this, menu_selector(MainMenuScene::menuTutorialCallBack));

    ////////////////////
   // �� �޴� �������� ���� ��Ų �޴� ��ü ���� �� ���� �߰�
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
 * ������ ������ Ȯ���ϴ� �Լ�
 */
void MainMenuScene::checkConnect(float f)
{
	jboolean connectedjni = false;
	bool connect = false;
	////////////////////
   // getConnected java �Լ��� native call�Ͽ� boolean������ ������ �Ǿ������� Ȯ����.
	  if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/punchking/PunchKing","getConnected","()Z"))
	      				    {
		  	  	  	  	  	  connectedjni = (jboolean)t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	      					  connect = (bool)connectedjni;
	      					  t.env->DeleteLocalRef(t.classID);
	      				    }
	  if(connect){					//if(������ �Ǿ��ٸ�)
		this->removeChild(_menucovered);	//�� Ȱ�� �޴��� ������ ����
	  }
}

/*
 * �ȵ���̵� ���ư �Լ� ������
 */
void MainMenuScene::keyBackClicked(){
	CCDirector::sharedDirector()->end();
}

/*
 * Ʃ�丮�� �޴� ���ý� ȣ�� �Ǵ� �ݹ� �Լ�.
 */
void MainMenuScene::menuTutorialCallBack(CCObject* pSender){
	AudioManager::getAudioManager()->playEffectAudio(SOUND_CLICK);
	AudioManager::getAudioManager()->stopSound();
	CCScene* scene = TutorialScene::scene();
	CCDirector::sharedDirector()->pushScene(scene);

}

/*
 * ���� ���� �޴� ���ý� ȣ�� �Ǵ� �ݹ� �Լ�.
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
 * �Ҹ��� ȣ�� �� java�� disConnect�Լ��� ȣ���Ͽ� ���� ���� ����
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

