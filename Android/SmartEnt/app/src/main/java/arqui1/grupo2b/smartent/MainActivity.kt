package arqui1.grupo2b.smartent

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import ingenieria.jhr.bluetoothjhr.BluetoothJhr
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val bluetooth = BluetoothJhr(this,lstBluetooth,Control::class.java)
        bluetooth.onBluetooth()
        lstBluetooth.setOnItemClickListener { adapterView, view, i, l ->
            bluetooth.bluetoothSeleccion(i)
        }
    }
}
