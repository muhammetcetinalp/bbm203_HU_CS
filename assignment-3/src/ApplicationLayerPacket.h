#ifndef APPLICATION_LAYER_PACKET_H
#define APPLICATION_LAYER_PACKET_H

#include "Packet.h"

// Extends Packet class. Have additional layer-specific member variables and overrides the virtual print function.
class ApplicationLayerPacket : public Packet {
public:
    ApplicationLayerPacket(int _layer_ID, const string& _sender_ID, const string& _receiver_ID, const string& _message_data);
    ~ApplicationLayerPacket() override;

    string sender_ID;
    string middle_sender;

    const string &getSenderId() const;

    void setSenderId(const string &senderId);

    string receiver_ID;

    const string &getReceiverId() const;

    void setReceiverId(const string &receiverId);

    string message_data;

    void print() override;

};

#endif // APPLICATION_LAYER_PACKET_H
