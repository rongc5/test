package com.littleblackcat.api;

import com.littleblackcat.model.CityResponse;

import retrofit2.http.GET;
import retrofit2.http.Query;
import rx.Observable;

/**
 * Created by Administrator on 2017/11/14.
 */

public interface APIService {
    @GET("search")
    Observable<CityResponse> getSearch(@Query("q") String q);
}
