package com.doomhowl.fetchexample

import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.doomhowl.fetchexample.databinding.ActivityExampleJniBinding

class FetchExampleActivity : AppCompatActivity() {

    private var binding: ActivityExampleJniBinding? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityExampleJniBinding.inflate(layoutInflater)
        setContentView(binding!!.root)
    }

    fun onRefreshButtonClicked(view: View) {
        val data = downloadImage();
        when {
            data == null -> {
                showErrorToast()
            }
            data.isEmpty() -> {
                showErrorToast()
            }
            else -> {
                val bitmap = BitmapFactory.decodeByteArray(data, 0, data.size)
                binding!!.imageView.setImageBitmap(bitmap)
            }
        }
    }

    external fun downloadImage(): ByteArray?

    private fun showErrorToast(){
        Toast.makeText(this, "Error downloading image", Toast.LENGTH_SHORT).show()
    }

    companion object {
        init {
            System.loadLibrary("fetchlib")
        }
    }
}

