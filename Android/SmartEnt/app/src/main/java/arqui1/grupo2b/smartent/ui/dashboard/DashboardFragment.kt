package arqui1.grupo2b.smartent.ui.dashboard

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

class DashboardFragment : Fragment() {

    private lateinit var dashboardViewModel: DashboardViewModel

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        dashboardViewModel =
            ViewModelProviders.of(this).get(DashboardViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_dashboard, container, false)
        val btnB1: Button = root.findViewById(R.id.btnBL1)
        val btnB2: Button = root.findViewById(R.id.btnBL2)

        btnB1.setOnClickListener{
            (activity as Control).blue.mTx("K")
            Toast.makeText(context, "Banda del laboratorio 1 encendida", Toast.LENGTH_SHORT).show()
        }

        btnB2.setOnClickListener{
            (activity as Control).blue.mTx("L")
            Toast.makeText(context, "Banda del laboratorio 2 encendida", Toast.LENGTH_SHORT).show()
        }
        dashboardViewModel.text.observe(viewLifecycleOwner, Observer {

        })
        return root
    }
}
