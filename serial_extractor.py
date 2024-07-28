import csv
import serial
import time
from datetime import date
from datetime import datetime

arduino = serial.Serial(port="COM4",baudrate=115200)

def read_serial(): 
    data = arduino.readline() 
    return data 

def create_data_field(data):
    now = date.today()  
    splitted_current_datetime = datetime.now().strftime('%Y-%m-%d %H:%M:%S').split()
    
    current_date = splitted_current_datetime[0]
    current_time = splitted_current_datetime[1] 
    splitted_data = data.split(",")
    temperature = splitted_data[0]
    humidity = splitted_data[1]

    return f"{temperature},{humidity},{current_date},{current_time}\n"

csv_file = open('datas.txt', "a")
counter = 0
writer = csv_file

while True:
    with open("datas.txt", "a") as txt_file:
        for i in range(0, 10):
            counter += 1 
            text = str(read_serial())[2:-5]
            try:
                data = create_data_field(text)
            except:
                data = "***" + text.upper() + "***"
            txt_file.write(data)
            print(str(data) + " => " + str(counter))
        txt_file.close()
        print("data saved. ")
        time.sleep(1)

