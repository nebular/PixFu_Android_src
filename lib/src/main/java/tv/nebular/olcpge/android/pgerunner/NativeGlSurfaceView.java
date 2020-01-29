/**
 * Generic Android OpenGL Native Application Launcher
 * @author Rodolfo Lopez Pintor 2020.
 * @license Creative Commons CC-BY 4.0
 *
 * GLSurfaceView that is rendered on the JNI side.
 * Based on GLES3JNIView, (C) AOSP.
 *
 */

package tv.nebular.olcpge.android.pgerunner;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static tv.nebular.olcpge.android.pgerunner.PgeRunner.OLCAPPNAME;
import static tv.nebular.olcpge.android.pgerunner.PgeRunner.SCALE;

@SuppressLint("ViewConstructor")
class NativeGlSurfaceView extends GLSurfaceView {

    public static float SCALE_NATIVE = 1;       // native resolution (full res)
    public static float SCALE_DP = 0;           // dp resolution using system density

    private static String PATH = null;

    int nWidth, nHeight;

    public NativeGlSurfaceView(Context context, float scale) {
        super(context);

        PATH = context.getFilesDir().getAbsolutePath()+"/"+OLCAPPNAME;

        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.

        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);

        DisplayMetrics displayMetrics = new DisplayMetrics();
        ((Activity)context).getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

        if (scale == 0) scale = displayMetrics.density;

        nHeight = (int) (displayMetrics.heightPixels / scale);
        nWidth = (int) (displayMetrics.widthPixels / scale);

        getHolder().setFixedSize(nWidth,nHeight);
        setRenderer(new Renderer());

    }

    @Override
    public void onPause() {
        super.onPause();
        PgeNativeLib.onPause(true);

    }

    @Override
    public void onResume() {
        super.onResume();
        PgeNativeLib.onPause(false);
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // calculate things that are easy here and complicated in the native side
        int action = event.getActionMasked();
        int index = event.getActionIndex();
        int pointerId = event.getPointerId(index);
        PgeNativeLib.onTouch(event, action, pointerId, SCALE);
        return true;
    }


    private static class Renderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
            PgeNativeLib.step();
        }
        public void onSurfaceChanged(GL10 gl, int width, int height) { PgeNativeLib.resize(width, height); }
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            PgeNativeLib.init(PATH);
        }
    }

}
