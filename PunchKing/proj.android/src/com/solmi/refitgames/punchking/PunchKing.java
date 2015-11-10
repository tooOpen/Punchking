/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.solmi.refitgames.punchking;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.WindowManager;
import edu.kmu.sensor.SensorBroker;
import edu.kmu.sensor.SensorBroker.m_SensorEvent;
import edu.kmu.solmi.SolmiBroker;


public class PunchKing extends Cocos2dxActivity{
	
	public static final int DEVICE_CONNECT = 1;
	public static final int DEVICE_DISCONNECT = 2;

	private static Object activity;			//플레이어 네임을 static으로 하지 않기 위해
	public static boolean connected = false;//연결 상태
	static Handler b_handler;
	private String PlayerName = "JHON SILVER";	//사용자 이름
	private static SolmiBroker solmi;			//모듈 객체
	
	private String deviceName = "00:00:00:00:00:00";	//기기 초기 값 

    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		activity = this.getContext();
		solmi = new SolmiBroker(getApplicationContext());		//블루투스 연결을 위한 콘텍스트 전달
		solmi.setCallbackfunc(cocoCallback);					//기기값을 받아 넘겨줄 콜백객체 정의

		b_handler = new Handler() {
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case DEVICE_CONNECT:
					connectBT();
					solmi.connect(SensorBroker.DATA_ACC,SensorBroker.RUNTIME_CONTINUE,SensorBroker.ACC_DEGREE_8G);
					
					break;
				case DEVICE_DISCONNECT:
					solmi.disconnect();
					break;
				}
			}
		};
	}
    public static Object cppCall_logsth(){
    	  return activity;
 
    	 }
    
    protected void connectBT() {		
  		Intent intent = new Intent(getApplicationContext(),FindDeviceActivity.class);
  		startActivityForResult(intent,FindDeviceActivity.SET_DEVICE_ADDRESS);	//액티비티를 끝내면서 결과값을 얻어옴
  	}
      
      
      @Override
  	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
  		// TODO Auto-generated method stub
  		super.onActivityResult(requestCode, resultCode, data);
  		if(requestCode ==  FindDeviceActivity.SET_DEVICE_ADDRESS){
  			if (resultCode == FindDeviceActivity.RESULT_OK) {
  				//RESULT_OK
  				deviceName = data.getExtras().getString(FindDeviceActivity.EXTRA_DEVICE_ADDRESS);	//기기 값얻어오기
  				solmi.setDeviceName(deviceName);
  				
  			}	
  		}
  	}
      
    private native void getValue(SolmiBroker.SolmiEvent sevent);	//Navtive 함수 선언
  
    @Override
	protected void onDestroy() {
    	solmi.disconnect();
		super.onDestroy();
		}

	public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	return glSurfaceView;
    }
    
	SensorBroker.sensorcallback cocoCallback = new SensorBroker.sensorcallback()	//sensorbroker에 sensorcallback인터페이스를 재정의 후 인스턴스화
	{
		@Override
		public void rcvedData(m_SensorEvent paramsolmi) {
		SolmiBroker.SolmiEvent sevent = (SolmiBroker.SolmiEvent)paramsolmi;	//m_SensorEvent는 변수만 있고 이것을 상속받은 solmiEvent로 캐스팅하여 c++로
		getValue(sevent);	//native 함수 호출
		}

		@Override
		public void successConnected() {	//연결 성공시 호출 됨
			connected = true;
		}
		
	};
	
	public String getplayername()	//사용자 이름 String으로 C++로
	{
		return this.PlayerName;
	}
	
    public static void disconnect()	//연결끊기
    {
    	 b_handler.sendEmptyMessage(DEVICE_DISCONNECT);	//핸들러로 메세지 보냄
    }
	public static void connectDevice() {	//연결하기
		 b_handler.sendEmptyMessage(DEVICE_CONNECT);
		 
	}
	public static boolean getConnected()	//연결상태 Native call
	{
		return connected;
	}
	public static boolean getAxis(){	//현재 사용자가 잡고 있는 기기 축 판단 함수.
		if(solmi.getMainAxis() == 1){
			return true;
		}
		else 
			return false;
	}
    static {
        System.loadLibrary("cocos2dcpp");
    }       
}
