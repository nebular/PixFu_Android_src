/**
 *
 * Generic Android OpenGL Native Application Launcher
 *
 * @author Rodolfo Lopez Pintor 2020.
 * @license Creative Commons CC-BY 4.0
 *
 * This launcher is meant to be generic and packed into a shared library. Ideally no modifications
 * are needed for your different projects, so you can concentrate on developing the C++ applications
 * and just link in the launcher and let the magic happen, forgetting about Java, JNI, toolchains, etc.
 *
 * Will copy all files and folders recursively from the assets/<appname>/ folder
 * to Android app's private directory, so everything is painlessly available on C++
 * with regular fopen() etc rather than exotic JNI functions.
 *
 * A progressbar will be displayed while the assets are copied.
 *
 * After that we load the shared library, that will instantiate the native application.
 * Finally, we instantiate the GLES3 surface view and connect its callbacks with their C++ counterparts.
 *
 */

package tv.nebular.pixengine.android.launcher;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.DisplayMetrics;
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

public class Runner extends Activity {

	public final static String APPNAME = "pix";
	public static float SCALE;

	private final static String TAG = "Runner";

	NativeGlSurfaceView mView;

	/**
	 * Concenience constructor for DP resoution
	 */

	public Runner() {
		this(NativeGlSurfaceView.SCALE_DP);
	}

	/**
	 * Creates a PgeRunner Activity with a custom resolution.
	 *
	 * @param densityPixelSize The PGS Resolution. 1 = native phone resolution, 0 = dp logical resolution or custom
	 */

	public Runner(float densityPixelSize) {
		SCALE = densityPixelSize;
	}


	@Override
	protected void onCreate(Bundle icicle) {

		super.onCreate(icicle);

		ProgressBar p = new ProgressBar(this);
		p.setIndeterminate(true);
		addContentView(p,
				new FrameLayout.LayoutParams(
						ViewGroup.LayoutParams.WRAP_CONTENT,
						ViewGroup.LayoutParams.WRAP_CONTENT,
						Gravity.CENTER
				)
		);


		if (SCALE == 0) {
			DisplayMetrics displayMetrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
			SCALE = displayMetrics.density;
		}

		recursiveCopyAssets(new Runnable() {
			@Override
			public void run() {
				mView = new NativeGlSurfaceView(Runner.this, SCALE);
				setContentView(mView);
				mView.onResume();
			}
		});
	}

	@Override
	protected void onPause() {
		super.onPause();
		if (mView != null) mView.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		if (mView != null) mView.onResume();
	}


	/**
	 * Copies all assets into the internal storge
	 *
	 * @param finished Runnable to execute when finished (in the UI thread)
	 */

	private void recursiveCopyAssets(final Runnable finished) {
		new Thread(new Runnable() {
			@Override
			public void run() {
				copyFileOrDir(new File(Runner.this.getFilesDir(), Runner.APPNAME), "");
				Runner.this.runOnUiThread(finished);
			}
		}).start();
	}


	public void copyFileOrDir(File outdir, String path) {

		AssetManager assetManager = this.getAssets();
		String[] assets;
		outdir.mkdirs();

		try {
			assets = assetManager.list(path);

			if (assets == null || assets.length == 0) {

				copyFile(outdir, path);

			} else {

				if (path.length() > 0) {
					File dir = new File(outdir, path);

					if (!dir.exists() && !dir.mkdir())
						throw new IOException("Cannot write to storage");
				}

				for (String asset : assets) {
					copyFileOrDir(outdir, (path.length() > 0 ? path + "/" : "") + asset);
				}

			}
		} catch (IOException ex) {
			Log.e(TAG, "I/O Exception", ex);
		}
	}


	private void copyFile(File outDir, String filename) {
		AssetManager assetManager = this.getAssets();

		InputStream in;
		OutputStream out;
		try {
			Log.v(TAG, "Copy " + filename + " to " + outDir.getAbsolutePath());
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
			Log.e(TAG, "error: " + e.getMessage());
		}

	}

}
