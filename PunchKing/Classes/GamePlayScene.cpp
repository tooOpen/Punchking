/*
 * GamePlayScene.cpp
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */

#include "GamePlayScene.h"
#include "MainMenuScene.h"
#include "GameResultScene.h"
#include "time.h"
#include "math.h"

#define ACCURACYEXCEL 1
#define ACCURACYGREAT 2
#define ACCURACYGOOD 3
#define ACCURACYBAD 4
#define DIVISION_NUMBER 133 //파워 값은 1800이 최대 게이지 그림한칸에 52 기준 해상도는 720 즉 게이지 13 칸일 들어갈수 있다 1800/13은 대략 133
#define TOTALSTAGE 30		//전체 스테이지

class DataPossessor;
class GameResultScene;
DataPossessor dataPossessor;

int accumulatedStage;
int finalScore;

using namespace cocos2d;


CCScene* GamePlayScene::scene() {
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	GamePlayScene *layer = GamePlayScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GamePlayScene::init() {

	if (!CCLayer::init()) {
		return false;
	}

	_accX = _accY = _accZ = 0;
	_powerScale = 0.0f;
	_totalScoreNumber = 0;
	_timingRectScale = 1.0f;
	_totalScore = 0;
	_score = 0;
	_power = 0;
	_state = STATE_CHECK_AXIS;
	_stageLevel = 1;
	_CheckNum = 0;
	_machineState = MACHINE_MOVE_READY;
	_pointSpeed = 10.0f;
	_powerScaleCheck = 0.0f;
	AudioManager::getAudioManager()->playSound(SOUND_PALYAUDIO);

	this->setKeypadEnabled(true);

	 //////////////////////////////
	// 배경 스프라이트 객체 생성
	_background = CCSprite::create("gameplay/images/background.png");
	_background->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).x,
					CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).y));
	this->addChild(_background, 0);

	_machine = CCSprite::create("gameplay/images/punchMachine5.png");
	_machine->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_PUNCHMACHINE).x,
					CoordinationManager::getCoordinationManager()->getPos(IMAGE_PUNCHMACHINE).y));
	this->addChild(_machine,4);

	 //////////////////////////////
	// 펀치머신 애니메이션 객체 생성
	CCAnimation* machineDownAnimation = CCAnimation::create();
	machineDownAnimation->setDelayPerUnit(0.05f);

	machineDownAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine1.png");
	machineDownAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine2.png");
	machineDownAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine3.png");
	machineDownAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine4.png");
	machineDownAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine5.png");

	CCAnimation* machineUpAnimation = CCAnimation::create();
	machineUpAnimation->setDelayPerUnit(0.2f);

	machineUpAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine5.png");
	machineUpAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine4.png");
	machineUpAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine3.png");
	machineUpAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine2.png");
	machineUpAnimation->addSpriteFrameWithFileName(
			"gameplay/images/punchMachine1.png");
	_machineUpAnimate = CCAnimate::create(machineUpAnimation);
	_machineUpAnimate->retain();

	_machineDownAnimate = CCAnimate::create(machineDownAnimation);
	_machineDownAnimate->retain();

	 //////////////////////////////
	// 스테이지 스프라이트 객체 생성
	_stageSprite = CCSprite::create("gameplay/images/stage.png");
	_stageNumber[0] = CCSprite::create("gameplay/images/stageNum.png",
			CCRectMake(200*9,0,200,281));
	_stageNumber[1] = CCSprite::create("gameplay/images/stageNum.png",
			CCRectMake(0,0,200,281));
	_stageSprite->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_STAGE).x,CoordinationManager::getCoordinationManager()->getPos(IMAGE_STAGE).y));
	_stageNumber[0]->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_STAGE_NUMBER).x - 100
			,CoordinationManager::getCoordinationManager()->getPos(IMAGE_STAGE_NUMBER).y));
	_stageNumber[1]->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_STAGE_NUMBER).x + 100
			,CoordinationManager::getCoordinationManager()->getPos(IMAGE_STAGE_NUMBER).y));
	this->addChild(_stageSprite, -1);
	this->addChild(_stageNumber[0],-1);
	this->addChild(_stageNumber[1],-1);

	 //////////////////////////////
	// 타이밍 바 & 파워 바 객체 생성
	_timmingBar = CCSprite::create("gameplay/images/timingBar.png");
	_timmingBar->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMINGBAR).x,CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMINGBAR).y));
	this->addChild(_timmingBar, 1);
	_timmingPoint = CCSprite::create("gameplay/images/timingPoint.png");
	_timmingPoint->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMING_POINT).x,
			CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMING_POINT).y));
	this->addChild(_timmingPoint, 4);
	_timingRect = CCSprite::create("gameplay/images/timingRect.png");
	_timingRect->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMING_POINT).x,
			CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMING_POINT).y));
	this->addChild(_timingRect, 3);


	_powerGaugeBar = CCSprite::create("gameplay/images/powGauge.png");
	_powerGaugeBar->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_POWERGAUGE_BAR).x,
					CoordinationManager::getCoordinationManager()->getPos(IMAGE_POWERGAUGE_BAR).y));
	this->addChild(_powerGaugeBar, 1);
	_powerGauge = CCSprite::create("gameplay/images/powGaugePoint.png");
	_powerGauge->setAnchorPoint(ccp(0.0f,0.5f));
	_powerGauge->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_POWERGAUGE).x,
			CoordinationManager::getCoordinationManager()->getPos(IMAGE_POWERGAUGE).y));
	this->addChild(_powerGauge, -1);
	_powerGaugeIcon = CCSprite::create("gameplay/images/powIcon.png");
	_powerGaugeIcon->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_POWER_ICON).x,
			CoordinationManager::getCoordinationManager()->getPos(IMAGE_POWER_ICON).y));
	this->addChild(_powerGaugeIcon, 3);

	_handError = CCSprite::create("gameplay/images/handError.png");
	_handError->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).x,
					CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).y));

	 //////////////////////////////
    // 펀치 머신 회색 점수 배경
	CCSprite* basicScoreNumber[4];
	for(int i = 0 ; i< 4 ; i++){
		basicScoreNumber[i] = CCSprite::create("gameplay/images/score.png",CCRectMake(115*10,0,115,199));
		basicScoreNumber[i]->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_SCORENUMBER).x + (115*i)+20, CoordinationManager::getCoordinationManager()->getPos(IMAGE_SCORENUMBER).y));
		this->addChild(basicScoreNumber[i],1);
	}

	 //////////////////////////////
	// 실제 점수
	for(int i = 0 ; i< 4 ; i++){
		_scoreNumber[i] = CCSprite::create("gameplay/images/score.png",CCRectMake(115*10,0,115,199));
		_scoreNumber[i]->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_SCORENUMBER).x + (115*i)+20, CoordinationManager::getCoordinationManager()->getPos(IMAGE_SCORENUMBER).y));
		this->addChild(_scoreNumber[i],2);
	}

	_accuracyText = CCSprite::create("gameplay/images/accuracyGood.png");
	_accuracyText->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_ACCURACY).x,CoordinationManager::getCoordinationManager()->getPos(IMAGE_ACCURACY).y));
	this->addChild(_accuracyText, -1);

	_gameOverImage = CCSprite::create("gameplay/images/overTex.png");
	_gameOverImage->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_GAMEOVER).x,CoordinationManager::getCoordinationManager()->getPos(IMAGE_GAMEOVER).y));
	this->addChild(_gameOverImage,-1);

	this->addChild(_handError, -1);
	this->schedule(schedule_selector(GamePlayScene::tick));
	return true;
}

/*
 * 게임 틱 함수
 */
void GamePlayScene::tick(float f) {

	switch (_state) {
	//////////////////////////////
	// 기기를 올바르게 잡았는가?
	case STATE_CHECK_AXIS: {
		checkMachineAxis();
		break;
	}
		//////////////////////////////
		// 게임 준비화면 - 펀치 머신올라가는 애니메이션 , 스테이지 표시
	case STATE_READY: {
		if (_CheckNum < 1.0f) { //스테이지 표시
			_accuracyText->setZOrder(-1);
			_stageSprite->setZOrder(5);
			_stageNumber[0]->setZOrder(5);
			_stageNumber[1]->setZOrder(5);
			_CheckNum += f;
		} else if (_CheckNum >= 1.0f && _CheckNum < 2.0f) { //펀치 머신 올라오는 효과
			_stageSprite->setZOrder(-1);
			_stageNumber[0]->setZOrder(-1);
			_stageNumber[1]->setZOrder(-1);
			_machine->runAction(_machineUpAnimate);
			_CheckNum += 1.0f;
		} else if (_CheckNum >= 2.0f && _CheckNum < 3.0f) {
			_CheckNum += f;
		} else if (_CheckNum >= 3.0f) { //타이밍 바 움직임 게임 시작 상태로
			this->schedule(schedule_selector(GamePlayScene::moveTimmingPoint));
			_state = STATE_GAME;
			_CheckNum = 0;
		}
		//////////////////////////////
		// 기기 가속도 값 얻기
		_accX = dataPossessor.getAccx();
		_accY = dataPossessor.getAccy();
		_accZ = dataPossessor.getAccz();
		_powerScale = 0.0f;
		_powerScaleCheck = 0.0f;
		this->unschedule(schedule_selector(GamePlayScene::raiseGauge));
		_powerGauge->setZOrder(-1);

		break;
	}
	case STATE_GAME: {
		if (AudioManager::getAudioManager()->isPlaySound())
			AudioManager::getAudioManager()->stopSound();
		shakeMachine(f); //기기를 움직였는지 체크
		break;
	}
	case STATE_OVER: {
		if (_CheckNum == 0) {
			if (_stageLevel == TOTALSTAGE) { //끝판을 깬것이라면
				_gameOverImage->setTexture(
						CCTextureCache::sharedTextureCache()->addImage( //게임 클리어 스프라이트 출력
								"gameplay/images/winTex.png"));
				AudioManager::getAudioManager()->playEffectAudio(
						SOUND_GAMECLEAR);
			} else {
				AudioManager::getAudioManager()->playEffectAudio( //아니라면 게임오버 출력
						SOUND_GAMEOVER);
			}
			_gameOverImage->setZOrder(6);
		}
		if (_CheckNum > 2.5f) {
			accumulatedStage = _stageLevel; //스테이지 값 결과 씬으로 넘기기
			finalScore = _totalScore; //점수 값 결과 씬으로 넘기기
			_CheckNum = 0;
			CCScene* scene = GameResultScene::scene(); //결과 창 열기
			CCDirector::sharedDirector()->popScene();
			CCDirector::sharedDirector()->pushScene(scene);
		}
		_CheckNum += f;
		break;
	}
	case STATE_CLEAR: { //스테이지 클리어 시
		_CheckNum += f;
		if (_CheckNum > 2.5f) {
			_CheckNum = 0;
			_gameOverImage->setZOrder(-1);
			changeStage(_stageLevel); //스테이지 변환
			changeTimingRectX();
			if (_stageLevel % 5 == 0) {
				reduceTimingRect();
			}
		}
		break;
	}
	}
}
/*
 * 사용자가 세로로 올바르게 잡고 있는지 판별 하여 알려주는 함수
 */
void GamePlayScene::checkMachineAxis() {
	jboolean axisCheckjni = false;
	bool axisCheck = false;
	if (JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/punchking/PunchKing",
			"getAxis", "()Z")) {
		axisCheckjni = (jboolean) t.env->CallStaticBooleanMethod(t.classID,
				t.methodID);
		axisCheck = (bool) axisCheckjni;
		t.env->DeleteLocalRef(t.classID);
	}
	if (axisCheck) {
		_state = STATE_READY;
		this->removeChild(_handError);
	} else {
		_handError->setZOrder(5);
	}
}

/*
 * 기기를 흔들었는지 얼마나 쎄게 흔들었는지 판별하는 함수
 */
void GamePlayScene::shakeMachine(float dt) {
	int power;
	switch (_machineState) {
	case MACHINE_MOVE_READY: {					//움직임이 준비상태보다 300이상 차이난다면 움직이기 시작했다고  판별
		if((abs(dataPossessor.getAccx() - _accX)
				+ abs(dataPossessor.getAccy() - _accY)
				+ abs(dataPossessor.getAccz() - _accZ)) > 300){
			_machineState = MACHINE_MOVE_START;
		}
		break;
	}
	case MACHINE_MOVE_START: {					// 약 1초동안 갚을 받으며 가장 큰 값을 저장한다
		_CheckNum += dt;
		power = (abs(dataPossessor.getAccx() - _accX)
				+ abs(dataPossessor.getAccy() - _accY)
				+ abs(dataPossessor.getAccz() - _accZ));
		if(power > _power){
			_power = power;
		}

		if(_CheckNum >0.15f){
			_machineState = MACHINE_MOVE_END;
		}
		break;
	}
	case MACHINE_MOVE_END: {
		if (_power > 300) {								//가장 큰 값이 300이 넘으면 그 값을 파워값으로 사용
			CCRect point = _timmingPoint->boundingBox();
			CCRect powRect = _timingRect->boundingBox();
			if (powRect.intersectsRect(point)) {		//타이밍 포인트가 타이밍 블럭안에 있는지 판별
				_powerScale = _power/DIVISION_NUMBER;	//게이지 바에 맞게 변환

				this->unschedule(
						schedule_selector(GamePlayScene::moveTimmingPoint));
				_machine->runAction(_machineDownAnimate);
				//////////////////////////////
				// 파워 값에 따라 효과음 과 텍스트 출력
				if(_power >= 1500){
					AudioManager::getAudioManager()->playEffectAudio(SOUND_EFFECT_EXCEL);
					showAccuracyText(ACCURACYEXCEL);
				}
				else if(_power < 1500 && _power>=1000){
					AudioManager::getAudioManager()->playEffectAudio(SOUND_EFFECT_GREAT);
					showAccuracyText(ACCURACYGREAT);
				}
				else if(_power < 1000){
					AudioManager::getAudioManager()->playEffectAudio(SOUND_EFFECT_GOOD);
					showAccuracyText(ACCURACYGOOD);
				}

				if (_stageLevel == TOTALSTAGE) { //끝판 이라면 게임오버
					_state = STATE_OVER;
				} else {						//아니면 클리어
					_state = STATE_CLEAR;
					_stageLevel++;				//스테이지 증가
				}
				_totalScore += _power/10;		//점수 계산
				this->schedule(schedule_selector(GamePlayScene::raiseGauge),1.0f/60.0f);
				_powerGauge->setZOrder(2);
			}else{								//타이밍 포인트에 맞추지 못하였다면
				AudioManager::getAudioManager()->playEffectAudio(SOUND_EFFECT_BAD);
				this->unschedule(
						schedule_selector(GamePlayScene::moveTimmingPoint));
				_machine->runAction(_machineDownAnimate);
				showAccuracyText(ACCURACYBAD);
				_state = STATE_OVER;			//게임 오버로
			}
		}
		scoreChange(_totalScore);				//점수 출력
		_CheckNum = 0;
		_power = 0;
		break;
	}
	default:
		break;
	}

}

/*
 * 게이지를 스케일 하여 즘가시켜주는 함수
 */
void GamePlayScene::raiseGauge(int f){
	_powerScaleCheck += 0.2f;
	if(_powerScaleCheck < _powerScale){
		_powerGauge->setScaleX(_powerScaleCheck);
	}
}

/*
 * 파워에 맞추어 텍스트 스프라이트 출력
 */
void GamePlayScene::showAccuracyText(int accuracy){
	switch(accuracy){
	case ACCURACYEXCEL:
		_accuracyText->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/accuracyExcel.png"));
		_accuracyText->setZOrder(5);
		break;
	case ACCURACYGREAT:
		_accuracyText->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/accuracyGreat.png"));
		_accuracyText->setZOrder(5);
		break;
	case ACCURACYGOOD:
		_accuracyText->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/accuracyGood.png"));
		_accuracyText->setZOrder(5);
		break;
	case ACCURACYBAD:
		_accuracyText->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/accuracyBad.png"));
		_accuracyText->setZOrder(5);
		break;
	}
}

/*
 * 타이밍 바 움직이는 함수
 */
void GamePlayScene::moveTimmingPoint(float f) {
	if (_state == STATE_GAME) {
		_timmingPoint->setPosition(
				ccp(_timmingPoint->getPositionX() + _pointSpeed, _timmingPoint->getPositionY()));

		if (_timmingPoint->getPositionX()
				< _timmingPoint->getContentSize().width / 2 + 10
				|| _timmingPoint->getPositionX()
						> 720 - _timmingPoint->getContentSize().width / 2
								- 10) {
			_pointSpeed *= -1;
		}
	}
}

/*
 * 스테이지 스프라이트 출력 및 준비 상태로 귀한
 */
void GamePlayScene::changeStage(int stage) {
	int stageOper = stage / 10;
	if (stageOper == 0)
		stageOper = 10;
	_stageNumber[0]->setTextureRect(CCRectMake(200*(stageOper-1),0,200,281));
	stageOper = stage % 10;
	if (stageOper == 0)
		stageOper = 10;
	_stageNumber[1]->setTextureRect(CCRectMake(200*(stageOper-1),0,200,281));

	_pointSpeed += 0.25;

	_state = STATE_READY;
	_machineState = MACHINE_MOVE_READY;
}

/*
 * 점수 스프라이트 잘라서 출력
 */
void GamePlayScene::scoreChange(int score){
	int divideNumber = 1000;
	for(int i = 0 ; i<4 ; i++){
		int number = score/divideNumber;
		if(number == 0)
			number = 10;

		_scoreNumber[i]->setTextureRect(CCRectMake(115*(number-1),0,115,199));
		score = score%divideNumber;
		divideNumber = divideNumber/10;
	}
}

/*
 * 타이밍 블럭 길이 줄이는 함수
 */
void GamePlayScene::reduceTimingRect(){

	_timingRectScale -= 1.0f/6.0f;
	_timingRect->setScaleX(_timingRectScale);
}

/*
 * 안드로이드 뒤로가기 버튼 재정의
 */
void GamePlayScene::keyBackClicked() {
	CCDirector::sharedDirector()->end();
}

/*
 * 타이밍 블럭의 위치를 랜덤으로 배치하는 함수 +- 0~250 까지
 */
void GamePlayScene::changeTimingRectX(){
	srand(time(NULL));
	int rand = random()%250;
	int ran = random()%2;
	if(ran == 0)
		rand = (-1) * rand;
	_timingRect->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMING_POINT).x+rand, _timingRect->getPositionY()));
}

/*
 GamePlayScene::GamePlaySene() {
 // TODO Auto-generated constructor stub

 }
 */
GamePlayScene::~GamePlayScene() {
	// TODO Auto-generated destructor stub
	AudioManager::getAudioManager()->stopSound();
	//this->
	this->cleanup();
	this->removeAllChildren();
}

/*
 * 기기로 부터 값 얻기
 */
#ifdef __cplusplus
extern "C" {
#endif
void Java_com_solmi_refitgames_punchking_PunchKing_getValue(JNIEnv* env, jobject thisObj,
		jobject valueObj) {
	int message = 0;
	jfieldID fid;
	jmethodID mid;
	jclass cls = env->GetObjectClass(valueObj);
	mid = env->GetMethodID(cls, "getDataType", "()I");
	jint jmessage = env->CallIntMethod(valueObj, mid);
	message = (int) jmessage;
	jint accx, accy, accz, ecg, stess, hr;
	switch (message) {
	case 1:
		mid = env->GetMethodID(cls, "getACCDataX", "()I");
		accx = env->CallIntMethod(valueObj, mid);
		mid = env->GetMethodID(cls, "getACCDataY", "()I");
		accy = env->CallIntMethod(valueObj, mid);
		mid = env->GetMethodID(cls, "getACCDataZ", "()I");
		accz = env->CallIntMethod(valueObj, mid);
		dataPossessor.setAccData((int) accx, (int) accy, (int) accz);
		break;
	case 2:
		mid = env->GetMethodID(cls, "getECGData", "()I");
		ecg = env->CallIntMethod(valueObj, mid);
		dataPossessor.setECG((int) ecg);

		break;
	case 4:
		mid = env->GetMethodID(cls, "getStressData", "()I");
		stess = env->CallIntMethod(valueObj, mid);
		dataPossessor.setStress((int) stess);

		break;
	case 8:
		mid = env->GetMethodID(cls, "getHRData", "()I");
		hr = env->CallIntMethod(valueObj, mid);
		dataPossessor.setHR((int) hr);

		break;
	}
}

#ifdef __cplusplus
}
#endif

