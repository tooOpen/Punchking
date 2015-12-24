package com.solmi.refitgames.punchking;

import com.punchking.game.R;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class FindDeviceActivity extends Activity{
	
	public static final String EXTRA_DEVICE_ADDRESS = "device_address";				
	//result code
	public static final int RESULT_OK = 201;											
	public static final int RESULT_CANCEL = 200;
	public static final int REQUEST_ENABLE_BT = 202;
	
	public static final int SET_DEVICE_ADDRESS = 100;
	
	
	
	private BluetoothAdapter mBtAdapter;
//	private ArrayAdapter<String> mPairedDeviceArrayAdapter;	
	private ArrayAdapter<String> mNewDeviceArrayAdapter;
	
	private TextView g_tvDev;
	private TextView g_tvTitle;
	private ImageView g_ivDev;
	private ProgressBar g_progressSearch;
	


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_bluetooth_search);
		
		setResult(Activity.RESULT_CANCELED);
		
		g_tvDev = (TextView)findViewById(R.id.tv_selectedDev);
		g_ivDev = (ImageView)findViewById(R.id.iv_selectedDev);
		g_progressSearch = (ProgressBar)findViewById(R.id.progress_measure);
		g_tvTitle = (TextView)findViewById(R.id.title_new_devices);
		
		LinearLayout scanButton = (LinearLayout)findViewById(R.id.button_scan);
		scanButton.setOnClickListener(new View.OnClickListener() {		
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(mBtAdapter.isEnabled()){
					doDiscover();
					searchTimer(11);
				}
				else{
					Toast.makeText(getApplicationContext(),getResources().getString(R.string.tryBTActive),Toast.LENGTH_LONG).show();
				}
				
				
			}
		});
		
		
		
		//initial 
		mNewDeviceArrayAdapter = new ArrayAdapter<String>(this,R.layout.dev_name);
	
		//find and set up the Listview for new
		ListView newDeviceArrayAdapter = (ListView)findViewById(R.id.new_devices);
		newDeviceArrayAdapter.setAdapter(mNewDeviceArrayAdapter);
		newDeviceArrayAdapter.setOnItemClickListener(mDeviceClickListener);
		
		//Register for broadcast
		IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
		this.registerReceiver(mReceiver, filter);
		
		filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
		this.registerReceiver(mReceiver, filter);
		
		filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
		this.registerReceiver(mReceiver, filter);
				
		mBtAdapter = BluetoothAdapter.getDefaultAdapter();
		
		if(mBtAdapter.isEnabled()){		//블루투스가 켜져 있으면
			doDiscover();	//바로 장치를 검색하고			
		}
		else{	//블루투스가 꺼져있으면			
			mBtAdapter.enable();	//장치를 활성화 하고 기다리면 브로드캐스트 리시버에서 검색을 시작
			/*
			Handler handler = new Handler();
			handler.postDelayed(new Runnable(){
				public void run() {
					// TODO Auto-generated method stub
					//잠시 후에 장치 검색을 시작 한다.
					doDiscover();
				}					
			}, 500);
			*/
			
		}
		
		/**
		 * 2014.01.13 On/Off 스위치 사용 안함
		if(mBtAdapter.isEnabled()){
			gSWonoff.setChecked(true);
			doDiscover();
			searchTimer(11);
		}
		else{
			gSWonoff.setChecked(false);
		}
		**/
	}


	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		if(mBtAdapter != null){
			mBtAdapter.cancelDiscovery();
		}
		//Unregister
		this.unregisterReceiver(mReceiver);
	}
	

	public void doDiscover(){		
		g_tvTitle.setText(getResources().getString(R.string.searchDevice));
		if(mNewDeviceArrayAdapter.getCount() >= 0)
			mNewDeviceArrayAdapter.clear();
    	    	
    	if(mBtAdapter.isDiscovering()){
    		mBtAdapter.cancelDiscovery();
    	}
    	
    	mBtAdapter.startDiscovery();
		searchTimer(11);	//프로그래스바를 그린다
	}
	
	
	private OnItemClickListener mDeviceClickListener = new OnItemClickListener()
	{
		public void onItemClick(AdapterView<?> av, View v, int arg2,
				long arg3) {
			// TODO Auto-generated method stub
			mBtAdapter.cancelDiscovery();
				
			String info = ((TextView)v).getText().toString();
			
			if(info.length() != 0)
			{
				Intent intent = new Intent();
				String address = info.substring(info.length() - 17);
				String dev = info.substring(0,info.length() - 17);
				intent.putExtra(EXTRA_DEVICE_ADDRESS, address);			
				setResult(RESULT_OK, intent);
				g_tvDev.setText(dev);
			}
			else
				setResult(RESULT_CANCEL);
			
			finish();			
		}		
	};

	private void nofindDev(){
		setResult(RESULT_CANCEL);
	}
	private final BroadcastReceiver mReceiver = new BroadcastReceiver()		
	{
		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			String action = intent.getAction();
			
			if(BluetoothDevice.ACTION_FOUND.equals(action)){
				BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				if(device.getName() != null){
					mNewDeviceArrayAdapter.add(device.getName() + "\n" + device.getAddress());
				}
						
				
			}else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)){
				setProgressBarIndeterminateVisibility(false);
				g_tvTitle.setText(getResources().getString(R.string.searchedDevice));
				if(mNewDeviceArrayAdapter.getCount() == 0){						
					nofindDev();
				}
			}			
			else if(BluetoothAdapter.ACTION_STATE_CHANGED.equals(action)){
				int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.STATE_OFF);	//기본적으로 꺼져있다고 보고
				
				if(state == BluetoothAdapter.STATE_ON){	//턴 온 되었을때
					doDiscover();
				}
			}
		}
		
	};
	
	
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if(event.getKeyCode() == event.KEYCODE_BACK){
			setResult(RESULT_CANCEL);
		}
		
		return super.onKeyDown(keyCode, event);
	}
	
	
	private CountDownTimer g_timer;
	private int g_nVal = 0;
	protected void searchTimer(int sec) {
		g_nVal = 0;
		g_timer = new CountDownTimer(sec * 1000, 100) {
			@Override
			public void onFinish() {				
				g_progressSearch.setProgress(1000);
			}

			@Override
			public void onTick(long millisUntilFinished) {
				g_progressSearch.setProgress(g_nVal++);
			}

		};
		g_timer.start();
	}

}
