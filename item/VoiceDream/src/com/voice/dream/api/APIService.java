package com.voice.dream.api;



import io.reactivex.Observable;
import retrofit2.http.GET;
import retrofit2.http.Query;
import com.voice.dream.model.UserInfo;

public interface APIService {

    
    @GET("movie/search")
    Observable<UserInfo> getSearchMovie(@Query("q") String q);
	
}
