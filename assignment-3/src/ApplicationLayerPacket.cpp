#include "ApplicationLayerPacket.h"

ApplicationLayerPacket::ApplicationLayerPacket(int _layer_ID, const string &_sender_ID, const string &_receiver_ID, const string& _message_data) : Packet(_layer_ID) {
    sender_ID = _sender_ID;
    receiver_ID = _receiver_ID;
    message_data = _message_data;
}

void ApplicationLayerPacket::print() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.
    Packet::print();
    cout << "Sender ID: " << sender_ID <<", Receiver ID: " <<receiver_ID << endl;
    cout << "Message chunk carried: \"" << message_data << "\"" << endl;
}


ApplicationLayerPacket::~ApplicationLayerPacket() {
    // TODO: Free any dynamically allocated memory if necessary.
}

const string &ApplicationLayerPacket::getReceiverId() const {
    return receiver_ID;
}

void ApplicationLayerPacket::setReceiverId(const string &receiverId) {
    receiver_ID = receiverId;
}

const string &ApplicationLayerPacket::getSenderId() const {
    return sender_ID;
}

void ApplicationLayerPacket::setSenderId(const string &senderId) {
    sender_ID = senderId;
}
