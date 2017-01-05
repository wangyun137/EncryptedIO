package com.singuloid.encrypt;

import android.app.Application;
import android.os.Build;
import android.util.Log;

/**
 * Author: wangyun
 * Date: 17-1-5
 * Email: yun.wang@vmplay.com
 * LastUpdateTime: 17-1-5
 * LastUpdateBy: wangyun
 */
public class IOApplication extends Application{

	private static final String TAG = "IOApplication";
	private static final String LIB_IO = "io";

	/**
	 * The key used by crypto, we can fixed it or generate it dynamic
	 */
	private static String sEncryptKey = "1234567890abcdef";


	static {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
			try {
				System.loadLibrary(LIB_IO);
			} catch (Exception e) {
				Log.e(TAG, "load libio.so occur error and error is: ");
				Log.e(TAG, Log.getStackTraceString(e));
			}
		}
	}

	/**
	 * Switch a new key to crypto
	 * @param newKey
	 */
	private native void switchKey(String newKey);
}
