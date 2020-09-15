package arqui1.grupo2b.smartent

import android.graphics.Color
import android.os.Bundle
import android.widget.TextView
import android.widget.Toast
import com.google.android.material.bottomnavigation.BottomNavigationView
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import ingenieria.jhr.bluetoothjhr.BluetoothJhr
import kotlin.concurrent.thread
import arqui1.grupo2b.smartent.Control
import arqui1.grupo2b.smartent.ui.temp.temp
import kotlinx.android.synthetic.main.temp_fragment.*
import kotlinx.coroutines.invoke
import kotlinx.coroutines.newFixedThreadPoolContext

class Control : AppCompatActivity() {

    lateinit var blue :BluetoothJhr
    var initHilo = false
    var hilo = true
    var mensaje = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_control)
        val navView: BottomNavigationView = findViewById(R.id.nav_view)


        val navController = findNavController(R.id.nav_host_fragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        val appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.navigation_home, R.id.navigation_dashboard, R.id.navigation_notifications
            )
        )
        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)
        blue = BluetoothJhr(this, MainActivity::class.java)
        //si se pierde conexion no sale sino que avisa con un mensaje  error
        blue.exitErrorOk(true)
        //mensaje conectado
        blue.mensajeConexion("conectado a dispositivo bluetooth\n")
        //mensaje de error
        blue.mensajeErrorTx("Error: Ocurrio un problema al conectar")


        thread(start = true){
            while (!initHilo){
                Thread.sleep(500)
            }
            while(hilo){
                blue.mTx("T")
                Thread.sleep(1000)
                mensaje = blue.mRx()
                if (mensaje != "") {
                    if (hilo) {
                        runOnUiThread {
                            //termometro.tempSet(mensaje.toFloat())
                            println("$mensaje")

                            if(mensaje.matches("Y+".toRegex())){
                                println("Cortar conexión")
                                Toast.makeText(this, "No tiene permiso para conectarse por la APP", Toast.LENGTH_SHORT).show()
                                blue.exitConexion()

                                finish()
                            }
                        }

                    } else {
                        break
                    }
                    blue.mensajeReset()
                }
                Thread.sleep(1000)
            }

        }
    }

    override fun onResume() {
        super.onResume()
        initHilo = blue.conectaBluetooth()
    }

    override fun onPause() {
        super.onPause()
        hilo = false
        initHilo = true
        blue.exitConexion()
        finish()
    }

    override fun onDestroy() {
        super.onDestroy()
        initHilo = true
        hilo = false
    }

}
