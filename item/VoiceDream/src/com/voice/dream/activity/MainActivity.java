package com.voice.dream.activity;

import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.schedulers.Schedulers;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import com.example.voicedream.R;
import com.voice.dream.api.CityService;
import com.voice.dream.model.UserInfo;
import com.voice.dream.net.AppClient;
import com.voice.dream.util.LogUtil;


public class MainActivity extends Activity {

	public static final String TAG = MainActivity.class.getSimpleName();
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
    public void requestBySubscribe() {
    	AppClient.create(CityService.class).queryGeonames(44.1, -9.9, -22.4, 55.2, "de")
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Subscriber<UserInfo>() {
                    @Override
                    public void onStart() {
                    	LogUtil.d(TAG, Thread.currentThread().getName() + "---onStart......");
                        //显示loading
//                        LoadingBar.show(rlContent);
                    }

                    @Override
                    public void onCompleted() {
                    	LogUtil.d(TAG, Thread.currentThread().getName() + "---onCompleted......");
                        //取消loading
//                        LoadingBar.cancel(rlContent);
                    }

                    @Override
                    public void onError(Throwable e) {
                    	LogUtil.d(TAG, Thread.currentThread().getName() + "---onError......" + e);
                        //取消loading
//                        LoadingBar.cancel(rlContent);
                    }

                    @Override
                    public void onNext(UserInfo hotMovieModel) {
                    	LogUtil.d(TAG, Thread.currentThread().getName() + "---onNext......" + hotMovieModel);
                        //绑定数据
                        //bindData(hotMovieModel.getSubjects());
                    }
                });
    }
}
