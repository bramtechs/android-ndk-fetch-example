package com.doomhowl.fetchexample

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.doomhowl.fetchexample.databinding.ActivityExampleJniBinding;

class FetchExampleActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val binding = ActivityExampleJniBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.helloTextview.text = stringFromJNI()
    }

    external fun stringFromJNI(): String?

    companion object {
        init {
            System.loadLibrary("fetchlib")
        }
    }
}

