package arqui1.grupo2b.smartent.ui.notifications

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import arqui1.grupo2b.smartent.Control
import arqui1.grupo2b.smartent.R

class NotificationsFragment : Fragment() {

    private lateinit var notificationsViewModel: NotificationsViewModel

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {


        notificationsViewModel =
            ViewModelProviders.of(this).get(NotificationsViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_notifications, container, false)

        val btnPOn: Button = root.findViewById(R.id.btnPOn)
        val btnPOff: Button = root.findViewById(R.id.btnPOff)

        btnPOn.setOnClickListener{
            (activity as Control).blue.mTx("M")
            Toast.makeText(context, "Abriendo portón", Toast.LENGTH_SHORT).show()
        }
        btnPOff.setOnClickListener{
            (activity as Control).blue.mTx("N")
            Toast.makeText(context, "Cerrando portón", Toast.LENGTH_SHORT).show()
        }
        notificationsViewModel.text.observe(viewLifecycleOwner, Observer {

        })
        return root
    }
}
