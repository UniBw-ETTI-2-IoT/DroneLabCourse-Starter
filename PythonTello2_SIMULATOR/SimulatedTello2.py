#Last Change: KS 2022 03 15
import paho.mqtt.client as mqtt
import time

strRealAircraftID = "UniBwTello1"
strSimulatedAircraftID = "UniBwTello2"
mqttBroker = "127.0.0.1"

def on_message(client, userdat,  message):
    strMessage = str(message.payload.decode("utf-8"))
    strTopic = message.topic
    if strMessage == "Cleared for Takeoff" and strTopic == strSimulatedAircraftID+"/command":
        print("Received messages: \"", strMessage, "\" from Topic\"", strTopic ,"\"")
        print("taking off...")
        time.sleep(2)
        print("flying to Pad 2")
        time.sleep(2)
        client.publish(strRealAircraftID+"/command","Cleared to Land", qos=2)
        print("published: "+strRealAircraftID+"/command"+" = Cleared to Land")

    if strMessage == "Cleared to Land" and strTopic == strRealAircraftID+"/command":
        print("Received messages: \"", strMessage, "\" from Topic\"", strTopic ,"\"")
        print("flying around the course...")
        time.sleep(20)
        print("holding on pad 4")
        time.sleep(1)
        client.publish(strRealAircraftID+"/command","Cleared for Takeoff", qos=2)
        print("published: "+strRealAircraftID+"/command"+" = Cleared for Takeoff")
        print("cycle finished, standing in Idle On Mission Pad1...\n")


client = mqtt.Client(client_id=strSimulatedAircraftID, callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
client.connect(mqttBroker)
time.sleep(1)
client.publish(strSimulatedAircraftID+"/command","I'm listening to commands on this topic. :)", qos=2)
client.subscribe(strSimulatedAircraftID+"/command", qos=2)
client.subscribe(strRealAircraftID+"/command", qos=2) #cause paho mqtt is blocking and I can't just continue after publishing cleared to land and I'm proly too dumb to thread it
client.on_message = on_message
print("started successfully")
client.loop_forever()