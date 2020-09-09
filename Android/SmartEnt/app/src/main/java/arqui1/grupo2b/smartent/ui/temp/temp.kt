package arqui1.grupo2b.smartent.ui.temp

import android.graphics.Color
import androidx.lifecycle.ViewModelProviders
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import arqui1.grupo2b.smartent.Control

import arqui1.grupo2b.smartent.R
import kotlinx.android.synthetic.main.temp_fragment.*
import kotlinx.coroutines.*
import kotlin.concurrent.thread

class temp : Fragment() {

    companion object {
        fun newInstance() = temp()
    }

    private lateinit var viewModel: TempViewModel
    var mensaje = ""
    var initHilo = false
    var hilo = true


    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.temp_fragment, container, false)
        val txttemp: TextView = root.findViewById(R.id.lblTemp)


        thread(start = true) {
            while (!initHilo){
                Thread.sleep(500)
            }
            while(hilo) {
                mensaje = (activity as Control).mensaje
                if (mensaje != "") {
                    //termometro.tempSet(mensaje.toFloat())
                    println(mensaje)

                    if (mensaje.toFloat() >= 25) {
                        txttemp.setTextColor(Color.parseColor("#fc2c03"))
                    } else if (mensaje.toFloat() >= 20) {
                        txttemp.setTextColor(Color.parseColor("#ed9e00"))

                    } else if (mensaje.toFloat() < 20) {
                        txttemp.setTextColor(Color.parseColor("#00a6ed"))
                    }
                    (activity as Control).runOnUiThread {
                        txttemp.setText("$mensaje °C")
                    }

//                    Toast.makeText(context, "Prueba temperatura", Toast.LENGTH_SHORT).show()
                }
            }
        }



        //(activity as Control).blue.mTx("T")
        //val prueba = (activity as Control).blue.mRx()
        //Log.d("Preuba temperatura", prueba)

        return root
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        viewModel = ViewModelProviders.of(this).get(TempViewModel::class.java)



        // TODO: Use the ViewModel
    }

    override fun onResume() {
        super.onResume()
        initHilo = true
    }

    override fun onPause() {
        super.onPause()
        hilo = false
        initHilo = true
    }

    override fun onDestroy() {
        super.onDestroy()
        initHilo = true
        hilo = false
    }
}
