package com.unidata.pet_feeder_app

import android.os.Bundle
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.RadioGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.FragmentActivity
import com.google.firebase.database.*


class MainActivity : AppCompatActivity(), View.OnClickListener {

    private lateinit var mEditStatus1: EditText
    private lateinit var mEditStatus2: EditText
    private lateinit var mButtonSend: Button
    private lateinit var mRadioCommand: RadioGroup
    private lateinit var mDatabase: FirebaseDatabase
    private lateinit var mCommandRef: DatabaseReference
    private lateinit var mStatusRef: DatabaseReference


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mEditStatus1 = findViewById(R.id.status_command_edit)
        mEditStatus2 = findViewById(R.id.status_read_edit)
        mButtonSend = findViewById(R.id.button_send)
        mRadioCommand = findViewById(R.id.radio_group_commands)
        mButtonSend.setOnClickListener(this)
        mDatabase = FirebaseDatabase.getInstance()
        mCommandRef = mDatabase.getReference("petFeeder/command")
        mStatusRef = mDatabase.getReference("petFeeder/read_confirmation")
        // Read from the database
        // Read from the database
        mCommandRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                val value = dataSnapshot.getValue(String::class.java)
                var message = getString(R.string.invalid_command)
                when(value){
                    "a" ->{
                        message = getString(R.string.feed_now_status)
                    }
                    "b" ->{
                        message = getString(R.string.feed_4_status)
                    }
                    "c" ->{
                        message = getString(R.string.feed_6_status)
                    }
                    "d" -> {
                        message = getString(R.string.feed_8_status)
                    }
                }
                mEditStatus1.hint = message
            }

            override fun onCancelled(error: DatabaseError) {
                // Failed to read value
                mEditStatus1.hint = getString(R.string.fail_to_read_status)
            }
        })
        mStatusRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                val value = dataSnapshot.getValue(String::class.java)
                var message = getString(R.string.fail_to_read_status)
                when(value){
                    "sent" ->{
                        message = getString(R.string.sent)
                    }
                    "received" ->{
                        message = getString(R.string.received)
                    }
                }
                mEditStatus2.hint = message
            }

            override fun onCancelled(error: DatabaseError) {
                // Failed to read value
                mEditStatus2.hint = getString(R.string.fail_to_read_status)
            }
        })


    }

    override fun onClick(v: View) {
        if(v.id == R.id.button_send){
            var command = "s"
            when (mRadioCommand.checkedRadioButtonId) {
                R.id.radio_feed_now -> {
                    command = "a"
                }
                R.id.radio_feed_in_four -> {
                    command = "b"
                }
                R.id.radio_feed_in_six -> {
                    command = "c"
                }
                R.id.radio_feed_in_eight -> {
                    command = "d"
                }
            }
            if (command != "s"){
                mCommandRef.setValue(command)
                mStatusRef.setValue("sent")
            }
        }
    }


}

