package com.doomhowl.fetchexample

import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.doomhowl.fetchexample.databinding.ActivityExampleJniBinding;

class FetchExampleActivity : AppCompatActivity() {

    private var binding: ActivityExampleJniBinding? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityExampleJniBinding.inflate(layoutInflater)
        setContentView(binding!!.root)
        refreshText()
    }

    private fun refreshText()  {
        binding?.let { binding ->
            binding.helloTextview.text = stringFromJNI()
        }
    }

    fun onRefreshButtonClicked(view: View) {
        refreshText()
    }

    external fun stringFromJNI(): String?

    companion object {
        init {
            System.loadLibrary("fetchlib")
        }
    }
}

