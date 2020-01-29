/**
 *
 * Generic Android OpenGL Native Application Launcher
 * @author Rodolfo Lopez Pintor 2020.
 * @license Creative Commons CC-BY 4.0
 *
 * These are the native hooks that connect with the C++ side of PgeRunner
 *
 */

package tv.nebular.olcpge.android.pgerunner;

// Wrapper for native library

import android.view.MotionEvent;

public class PgeNativeLib {

     static {
          System.loadLibrary("olcapp");
     }

     public static native void init(String internalFilesPath);
     public static native void resize(int width, int height);
     public static native void step();
     public static native void onTouch(MotionEvent event,  int decodedAction, int pointerIndex, float screenDensity);
     public static native void onPause(boolean status);
}
