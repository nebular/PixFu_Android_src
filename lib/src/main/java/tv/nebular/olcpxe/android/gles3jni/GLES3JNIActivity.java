/*
 *
 * Generic Android OpenGL Native Application Launcher
 * Author: Rodolfo Lopez Pintor 2020.
 * License: Creative Commons Attribution (Latest version)
 *
 * This launcher is meant to be generic and packed into a shared library. Ideally no modifications
 * are needed for your differet projects, so you can concentrate on developing the C++ applications
 * and just link in the launcher and let the magic happen, forgetting about Java, JNI, toolchains, etc.
 *
 * Will copy all files and folders recursively from the assets/<appname>/ folder
 * to Android app's private directory, so everything is painlessly available on C++
 * with regular fopen() etc rather than exotic JNI functions.
 *
 * A progressbar will be displayed.
 *
 * After that we loading the shared library, that will instantiate the native application.
 * Finally, we instantiate the GLES3 surface view and connect its callbacks with their C++ counterparts.
 *
 */

package tv.nebular.olcpxe.android.gles3jni;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ProgressBar;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


public class GLES3JNIActivity extends Activity {

    public final static String OLCAPPNAME = "lonekart";
    GLES3JNIView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        ProgressBar p = new ProgressBar(this);
        p.setIndeterminate(true);
        addContentView(p, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, Gravity.CENTER));

        recursiveCopyAssets(new Runnable() {
            @Override
            public void run() {
                mView = new GLES3JNIView(GLES3JNIActivity.this, GLES3JNIView.SCALE_DP);
                setContentView(mView);
                mView.onResume();
            }
        });
    }

    @Override protected void onPause() {
        super.onPause();
        if (mView != null) mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        if (mView != null) mView.onResume();
    }


    private void recursiveCopyAssets(final Runnable finished) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                copyFileOrDir(GLES3JNIActivity.this.getFilesDir(), OLCAPPNAME);
                GLES3JNIActivity.this.runOnUiThread(finished);
            }
        }).start();
    }


    public void copyFileOrDir(File outdir, String path) {
        AssetManager assetManager = this.getAssets();
        String[] assets;

        try {
            assets = assetManager.list(path);

            if (assets == null || assets.length == 0) {

                copyFile(outdir, path);

            } else {

                File dir = new File(outdir, path);

                if (!dir.exists() && !dir.mkdir())
                    throw new IOException("Cannot write to storage");

                for (int i = 0; i < assets.length; ++i) {
                    copyFileOrDir(outdir, path + "/" + assets[i]);
                }

            }
        } catch (IOException ex) {
            Log.e("tag", "I/O Exception", ex);
        }
    }


    private void copyFile(File outDir, String filename) {
        AssetManager assetManager = this.getAssets();

        InputStream in;
        OutputStream out;
        try {
            in = assetManager.open(filename);
            out = new FileOutputStream(new File(outDir, filename));

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            out.flush();
            out.close();
        } catch (Exception e) {
            Log.e("tag", "error: "+e.getMessage());
        }

    }

}
