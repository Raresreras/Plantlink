from PyQt6.QtWidgets import *
from PyQt6.QtCore import *
from PyQt6.QtGui import *
import sys
import serial

class SerialReaderThread(QThread):
    data_received = pyqtSignal(str)

    def __init__(self, port, baudrate=9600):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.running = True

    def run(self):
        try:
            with serial.Serial(self.port, self.baudrate, timeout = 1) as ser:
                while self.running:
                    if ser.in_waiting > 0:
                        data = ser.readline()
                        self.data_received.emit(str(data)[:-5][2:])
        except serial.SerialException as e:
            self.data_received.emit(f"error: {e}")

    def stop(self):
        self.running = False
        self.wait()

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Plantlink")
        self.start_serial_reader()
        self.setWindowTitle("Plantlink")

        #Main UI

        mainWidget = QWidget()
        self.setCentralWidget(mainWidget)
        layout = QVBoxLayout()
        mainWidget.setLayout(layout)

        layout.addWidget(QLabel("Water level 1"))
        self.waterLevel1Label = QLabel()
        self.waterLevel1Label.setText("waiting data")
        layout.addWidget(self.waterLevel1Label)

        layout.addWidget(QLabel("Water level 2"))
        self.waterLevel2Label = QLabel()
        self.waterLevel2Label.setText("waiting data")
        layout.addWidget(self.waterLevel2Label)

        #Sensor UI

        self.comboBox1 = QComboBox()
        self.comboBox1.addItems(['101', '102', '103'])
        self.comboBox1.activated.connect(self.boardChange)
        layout.addWidget(self.comboBox1)

        self.boardCode = QLabel()
        self.boardCode.setText("Board version: 101")
        self.sensorTemp = QLabel()
        self.sensorTemp.setText("waiting data")
        self.sensorHum = QLabel()
        self.sensorHum.setText("waiting data")
        self.sensorSoilHum = QLabel()
        self.sensorSoilHum.setText("waiting data")

        button1 = QPushButton()
        button1.setText("Prime pump1")
        button1.setFixedSize(100, 40)
        button1.released.connect(lambda: self.send_data(1))

        button2 = QPushButton()
        button2.setText("Prime pump2")
        button2.setFixedSize(100, 40)
        button2.released.connect(lambda: self.send_data(2))

        button3 = QPushButton()
        button3.setText("Prime pump3")
        button3.setFixedSize(100, 40)
        button3.released.connect(lambda: self.send_data(3))

        layout.addWidget(self.boardCode)
        layout.addWidget(self.sensorTemp)
        layout.addWidget(self.sensorHum)
        layout.addWidget(self.sensorSoilHum)
        layout.addWidget(button1)
        layout.addWidget(button2)
        layout.addWidget(button3)

    def boardChange(self):
        code = str(self.comboBox1.currentText())
        self.boardCode.setText("Board version: "+code)
        self.sensorHum.setText("waiting data")
        self.sensorTemp.setText("waiting data")
        self.sensorSoilHum.setText("waiting data")

    def start_serial_reader(self):
        port = 'COM10'
        self.serial_thread = SerialReaderThread(port)
        self.serial_thread.data_received.connect(self.update_label)
        self.serial_thread.start()

    def update_label(self, data):
        code = int(self.comboBox1.currentText())

        if data[:3] == 'WL1':
            self.waterLevel1Label.setText(data[4:])
        if data[:3] == 'WL2':
            self.waterLevel2Label.setText(data[4:])

    def send_data(self, data):
        self.serial_thread.stop()
        try:
                serial.Serial('COM10', 9600, timeout = 1).write(data)
                print(data)
        except serial.SerialException as e:
            print(f"SerialException while sending data: {e}")
        self.start_serial_reader()

    def closeEvent(self, event):
        self.serial_thread.stop()
        event.accept()

app = QApplication(sys.argv)
window = MainWindow()
window.show()

app.exec()
