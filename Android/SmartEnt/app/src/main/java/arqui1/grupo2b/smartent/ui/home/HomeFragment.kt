package arqui1.grupo2b.smartent.ui.home

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

class HomeFragment : Fragment() {

    private lateinit var homeViewModel: HomeViewModel

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        homeViewModel =
            ViewModelProviders.of(this).get(HomeViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_home, container, false)
        val btL1on: Button = root.findViewById(R.id.btnL1On)
        val btn1off: Button = root.findViewById(R.id.btnL1Off)
        val btn2on: Button = root.findViewById(R.id.btnL2On)
        val btn2off: Button = root.findViewById(R.id.btnL2Off)
        val btnscon: Button = root.findViewById(R.id.btnSCOn)
        val btnscoff: Button = root.findViewById(R.id.btnSCOff)
        val btneeon: Button = root.findViewById(R.id.btnEEOn)
        val btneeoff: Button = root.findViewById(R.id.btnEEOff)
        val btngenon: Button = root.findViewById(R.id.btnGenOn)
        val btngenoff: Button = root.findViewById(R.id.btnGenOff)
        btL1on.setOnClickListener {
            (activity as Control).blue.mTx("A")
            Toast.makeText(context, "Luces de laboratorio 1 encendidas", Toast.LENGTH_SHORT).show()
        }

        btn1off.setOnClickListener {
            (activity as Control).blue.mTx("B")
            Toast.makeText(context, "Luces de laboratorio 1 apagadas", Toast.LENGTH_SHORT).show()
        }

        btn2on.setOnClickListener {
            (activity as Control).blue.mTx("C")
            Toast.makeText(context, "Luces de laboratorio 2 encendidas", Toast.LENGTH_SHORT).show()
        }

        btn2off.setOnClickListener {
            (activity as Control).blue.mTx("D")
            Toast.makeText(context, "Luces de laboratorio 2 apagadas", Toast.LENGTH_SHORT).show()
        }

        btnscon.setOnClickListener{
            (activity as Control).blue.mTx("E")
            Toast.makeText(context, "Luces de salida de camiones encendidas", Toast.LENGTH_SHORT).show()
        }
        btnscoff.setOnClickListener{
            (activity as Control).blue.mTx("F")
            Toast.makeText(context, "Luces de salida de camiones apagadas", Toast.LENGTH_SHORT).show()
        }
        btneeon.setOnClickListener{
            (activity as Control).blue.mTx("G")
            Toast.makeText(context, "Luces de entrada de empleados encendidas", Toast.LENGTH_SHORT).show()
        }
        btneeoff.setOnClickListener{
            (activity as Control).blue.mTx("H")
            Toast.makeText(context, "Luces de entrada de empleados apagadas", Toast.LENGTH_SHORT).show()
        }
        btngenon.setOnClickListener{
            (activity as Control).blue.mTx("I")
            Toast.makeText(context, "Todas las luces encendidas", Toast.LENGTH_SHORT).show()
        }
        btngenoff.setOnClickListener{
            (activity as Control).blue.mTx("J")
            Toast.makeText(context, "Todas las luces apagadas", Toast.LENGTH_SHORT).show()
        }

        homeViewModel.text.observe(viewLifecycleOwner, Observer {

        })
        return root
    }
}
