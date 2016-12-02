package au.ivj.lasertagrobot;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.ToggleButton;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity implements SensorEventListener {
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    ;

    private SensorManager sensorManager;
    private Sensor accelerometer;
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothDevice bluetoothDevice;
    private ConnectThread connectThread;
    private ConnectedThread connectedThread;
    private Handler bluetoothHandler;

    private SeekBar tilt;
    private SeekBar speed;
    private ToggleButton robotSwitch;
    private ToggleButton bluetoothSwitch;
    private TextView log;

    private int tiltX;
    private String lastPacket = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        /** Controls **/
        tilt = (SeekBar) findViewById(R.id.tilt);
        speed = (SeekBar) findViewById(R.id.speed);
        speed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                sendPacket();
            }
        });
        robotSwitch = (ToggleButton) findViewById(R.id.robotSwitch);
        robotSwitch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendPacket();
            }
        });

        log = (TextView) findViewById(R.id.log);

        bluetoothSwitch = (ToggleButton) findViewById(R.id.bluetoothSwitch);
        bluetoothSwitch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (bluetoothSwitch.isChecked()) {
                    connectThread = new ConnectThread(bluetoothDevice);
                    connectThread.start();
                } else {
                    connectThread.cancel();
                }
            }
        });

        Button shootButton = (Button) findViewById(R.id.shoot);
        shootButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                connectedThread.write("t".getBytes());
            }
        });

        /** Sensor **/
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        if (sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) != null) {
            accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
            sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
        } else {
            throw new IllegalStateException("No accelerometer");
        }

        /** Bluetooth **/
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            throw new IllegalStateException("No bluetooth");
        }
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, 1);
        }

        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            Log.i("MainActivity", "Bluetooth devices:");
            for (BluetoothDevice device : pairedDevices) {
                Log.i("MainActivity", device.getName() + ": " + device.getAddress());
                if (device.getName().equals("HC-05")) {
                    bluetoothDevice = device;
                }
            }
        }
        if (bluetoothDevice == null) {
            throw new IllegalArgumentException("Couldn't find device");
        }

        bluetoothHandler = new Handler(new Handler.Callback() {
            @Override
            public boolean handleMessage(Message msg) {
                log.setText(msg.getData().getString("value"));
                Log.i("MainActivity", "Received: " + msg.getData().getString("value"));
                return true;
            }
        });
    }

    protected void onResume() {
        super.onResume();
        sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
    }

    protected void onPause() {
        super.onPause();
        sensorManager.unregisterListener(this);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        float x = event.values[0];
        float y = event.values[1];
        float z = event.values[2];

        tiltX = (int) (Math.abs(x) * 10);
        tiltX = tiltX < 15 ? 0 : tiltX - (tiltX % 5);

        int progress = tiltX;
        if (x < 0) {
            progress += 100;
        } else {
            progress = 100 - progress;
        }

        if (tilt != null) {
            tilt.setProgress(progress);
        }

        if (x < 0) {
            tiltX *= -1;
        }

        sendPacket();
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    @Override
    public void onStart() {
        super.onStart();
    }

    @Override
    public void onStop() {
        super.onStop();
    }

    /**
     * Thread to connect to the bluetooth device
     */
    private class ConnectThread extends Thread {
        private final BluetoothSocket socket;
        private final BluetoothDevice device;

        public ConnectThread(BluetoothDevice device) {
            BluetoothSocket tmp = null;
            this.device = device;

            try {
                tmp = device.createInsecureRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
                Log.i("MainActivity", e.getMessage());
            }
            socket = tmp;
        }

        @Override
        public synchronized void start() {
            super.start();
        }

        public void run() {
            try {
                socket.connect();
            } catch (IOException connectException) {
                try {
                    socket.close();
                } catch (IOException closeException) {
                    Log.i("MainActivity", closeException.getMessage());
                }
                Log.i("MainActivity", connectException.getMessage());
            }

            if (connectedThread != null) {
                connectedThread.cancel();
            }

            connectedThread = new ConnectedThread(socket);
            connectedThread.start();
        }

        public void cancel() {
            try {
                socket.close();
            } catch (IOException e) {
                throw new IllegalStateException(e);
            }
        }
    }

    /**
     * Loop thread to send/receive data in the socket
     */
    private class ConnectedThread extends Thread {
        private final BluetoothSocket socket;
        private final InputStream inStream;
        private final OutputStream outStream;

        public ConnectedThread(BluetoothSocket socket) {
            this.socket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
                throw new IllegalStateException(e);
            }

            inStream = tmpIn;
            outStream = tmpOut;
            write("p;".getBytes());
        }

        public void run() {
            byte[] buffer = new byte[1024];
            int bytes;
            StringBuilder packet = new StringBuilder();

            while (true) {
                try {
                    BufferedInputStream bis = new BufferedInputStream(inStream);
                    bytes = inStream.read(buffer);
                    Message message = new Message();
                    message.what = 1;
                    Bundle bundle = new Bundle();
                    String value = new String(buffer, "UTF-8");
                    value = value.substring(0, bytes);
                    if (value.indexOf(";") < 0) {
                        packet.append(value);
                        continue;
                    } else if (packet.length() > 50) {
                        value = "Error parsing: " + packet;
                    } else {
                        value = packet.append(value.substring(0, value.indexOf(";"))).toString();
                        packet = new StringBuilder();
                    }
                    bundle.putString("value", value);
                    message.setData(bundle);
                    bluetoothHandler.sendMessage(message);
                } catch (IOException e) {
                    Log.i("MainActivity", e.getMessage());
                    break;
                }
            }
        }

        public void write(byte[] bytes) {
            try {
                outStream.write(bytes);
            } catch (IOException e) {
                Log.i("MainActivity", e.getMessage());
            }
        }

        public void cancel() {
            try {
                socket.close();
            } catch (IOException e) {
                Log.i("MainActivity", e.getMessage());
            }
        }
    }

    private void sendPacket() {
        String packet = getPacket();
        if (connectedThread != null && !lastPacket.equals(packet)) {
            connectedThread.write(packet.getBytes());
            Log.i("MainActivity", "Sending packet: " + packet);
            lastPacket = packet;
        }
    }

    /**
     * Puts together the relevant info for speed and direction
     */
    private String getPacket() {
        StringBuffer result = new StringBuffer();
        if (!robotSwitch.isChecked()) {
            result.append("s;");
        } else {
            result.append(String.format("f%3d;", speed.getProgress()));
            if (tiltX < 0) {
                result.append(String.format("sr%2d;", Math.abs(tiltX)));
            } else {
                result.append(String.format("sl%2d;", tiltX));
            }
        }
        return result.toString();
    }
}
