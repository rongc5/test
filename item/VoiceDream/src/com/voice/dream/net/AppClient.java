package com.voice.dream.net;

import java.io.IOException;

import okhttp3.HttpUrl;
import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Retrofit;
import retrofit2.adapter.rxjava.RxJavaCallAdapterFactory;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.converter.scalars.ScalarsConverterFactory;

import com.voice.dream.util.Constant;

public class AppClient {
	private static final String TAG = AppClient.class.getSimpleName();
	
	
	private Retrofit mRetrofit;
	
	private static AppClient mInstance;

	
    private AppClient(){
        this.mRetrofit=createRetrofit();
    }
	
	public static <T> T create(final Class<T> service) {
		if (mInstance==null){
			mInstance = new AppClient();
        }
        return mInstance.mRetrofit.create(service);		
	}
	
	private Retrofit createRetrofit() {
        Retrofit.Builder builder = new Retrofit.Builder();
        
        builder.baseUrl(Constant.DebugConfig.SERVER_URL);
        builder.addConverterFactory(ScalarsConverterFactory.create());
        builder.addConverterFactory(GsonConverterFactory.create());
        builder.addCallAdapterFactory(RxJavaCallAdapterFactory.create());
        

        //…Ë÷√okhttp
        OkHttpClient httpClient = createOkHttpClient();
        builder.client(httpClient);
        return builder.build();
    }
	
	 private OkHttpClient createOkHttpClient() {
		 final OkHttpClient.Builder httpClient =
	                new OkHttpClient.Builder();
	        httpClient.addInterceptor(new Interceptor() {
	            @Override
	            public Response intercept(Chain chain) throws IOException {
	                final Request original = chain.request();
	                final HttpUrl originalHttpUrl = original.url();

	                final HttpUrl url = originalHttpUrl.newBuilder()
	                        .addQueryParameter("username", "demo")
	                        .build();

	                // Request customization: add request headers
	                final Request.Builder requestBuilder = original.newBuilder()
	                        .url(url);

	                final Request request = requestBuilder.build();
	                return chain.proceed(request);
	            }
	        });
	        
	        if (Constant.DebugConfig.SHOW_NET_INFO) {
	        	httpClient.addInterceptor(new HttpLoggingInterceptor().
	        			setLevel(HttpLoggingInterceptor.Level.BASIC));
	        }
	        return httpClient.build();
	    }
	
}
