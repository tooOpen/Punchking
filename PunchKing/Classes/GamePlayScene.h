/*
 * GamePlayScene.h
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */

#ifndef GAMEPLAYSCENE_H_
#define GAMEPLAYSCENE_H_

#include "cocos2d.h"
#include "ResourcesID.h"
#include "manager/AudioManager.h"
#include "manager/CoordinationManager.h"
#include "platform/android/jni/JniHelper.h"
#include "DataPossessor.h"

////////////////////
// 게임 중 상태 변수
enum STATE_NAME{
	STATE_CHECK_AXIS, STATE_READY, STATE_GAME, STATE_OVER, STATE_CLEAR, STATE_MAXIMUM,
};

////////////////////
// 펀치 머신 상태 변수
enum MACHINE_MOVE_STATE{
	MACHINE_MOVE_READY = STATE_MAXIMUM + 1, MACHINE_MOVE_START, MACHINE_MOVE_END, MACHINE_MOVE_MAXIMUM,
};
using namespace edu_kmu_coordination_solmiengine;
using namespace edu_kmu_audio_solmiengine;
using namespace cocos2d;

class GamePlayScene :public CCLayer {
public:
	//GamePlayScene();
	virtual ~GamePlayScene();
	virtual bool init();
	static CCScene* scene();
	int _state;
	int _power;
	int _score;
	int _totalScore;
	int _stageLevel;
	int _machineState;
	int _accX,_accY,_accZ;
	int _totalScoreNumber;
	float _CheckNum;
	float _pointSpeed;
	float _powerScale;
	float _timingRectScale;
	float _powerScaleCheck;

	JniMethodInfo t;				//JNI 메소드 정보 변수
	CREATE_FUNC(GamePlayScene);
	CCSprite* _background;			//배경 스프라이트
	CCSprite* _machine;				//펀치머신 스프라이트
	CCAnimate* _machineUpAnimate;	//펀치머신이 올라오는 애니메이션
	CCAnimate* _machineDownAnimate;	//펀치 머신이 뒤로 넘어가는 애니메이션
	CCSprite* _stageSprite;			//stage 스프라이트
	CCSprite* _stageNumber[2];		//스테이지 숫자 스프라이트
	CCSprite* _timmingBar;			//타이밍 바 스프라이트
	CCSprite* _timmingPoint;		//포인트 스프라이트
	CCSprite* _timingRect;			//테두리 스프라이트
	CCSprite* _powerGaugeBar;		//파워 게이지 테두리 스프라이트
	CCSprite* _powerGauge;			//안에 차는 게이지 스프라이트
	CCSprite* _powerGaugeIcon;		//아이콘
	CCSprite* _handError;			//기기를 잘못잡았을 시 보여주는 스프라이트
	CCSprite* _scoreNumber[4];		//펀치머신에 출력되는 점수
	CCSprite* _accuracyText;		//파워 스프라이트
	CCSprite* _effect;				//펀치를 내지를때 나타나는 효과 스프라이트
	CCSprite* _gameOverImage;		//게임 오버 스프라이트



	void shakeMachine(float dt);
	virtual void keyBackClicked();
	void tick(float f);
	void moveTimmingPoint(float f);

	void showAccuracyText(int accuracy);
	void changeStage(int stage);
	void checkMachineAxis();

	void scoreChange(int score);
	void raiseGauge(int f);
	void reduceTimingRect();
	void changeTimingRectX();

};

#endif /* GAMEPLAYSCENE_H_ */
