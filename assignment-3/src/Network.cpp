#include "Network.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
    int number_of_hops = 0;
    for (const std::string& str : commands) {
        std::string tire(9+str.length(), '-');
        cout << tire << "\n";
        cout << "Command: " << str << "\n";
        cout << tire << "\n";
        std::istringstream iss(str);
        std::vector<std::string> new_str{
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>{}
        };
        if (new_str[0] == "MESSAGE"){
            bool in_message = false;
            //application layer
            std::string message;
            std::string sender_ID = new_str[1];
            std::string receiver_ID = new_str[2];

            //network layer
            std::string sender_IP_address;
            std::string receiver_IP_address;

            //physical layer
            std::string sender_MAC_address;
            std::string receiver_MAC_address;
            std::string receiver_MAC_address_receiver;

            for (const auto& word : new_str){
                if (word[0] == '#'){
                    in_message = true;
                    for (int i = 1; i<word.size(); i++){
                        message+=word[i];
                    }
                    message+= ' ';
                    continue;
                }
                if (word[word.size() - 1] == '#') {
                    for (int i = 0; i<word.size()-1; i++){
                        message+=word[i];
                    }
                    in_message = false;
                    continue;
                }
                if (in_message){
                    for (int i = 0; i<word.size(); i++){
                        message+=word[i];
                    }
                    message+= ' ';
                }
            }
            cout << "Message to be sent: " <<"\"" << message << "\" \n";

            if (message.length() <= message_limit){

                auto *application_layer = new ApplicationLayerPacket(0,sender_ID,receiver_ID,message);
                application_layer->middle_sender = sender_ID;
                auto *transport_layer = new TransportLayerPacket(1,sender_port,receiver_port);
                for (const auto& object : clients){
                    if (object.client_id == sender_ID){
                        sender_IP_address = object.client_ip;
                        sender_MAC_address = object.client_mac;
                        for(const auto& pair : object.routing_table){
                            if(pair.first == receiver_ID){
                                receiver_MAC_address_receiver = pair.second;
                            }
                        }
                    }
                    if(receiver_ID == object.client_id){
                        receiver_IP_address = object.client_ip;
                    }
                }
                for (const auto& object : clients){
                    if (object.client_id == receiver_MAC_address_receiver){
                        receiver_MAC_address = object.client_mac;
                    }
                }
                auto *network_layer = new NetworkLayerPacket(2,sender_IP_address,receiver_IP_address);
                auto *physical_layer = new PhysicalLayerPacket(3,sender_MAC_address,receiver_MAC_address);
                stack<Packet*> frame;
                frame.push(application_layer);
                frame.push(transport_layer);
                frame.push(network_layer);
                frame.push(physical_layer);
                stack<Packet*> temporay_frame = frame;
                cout << "\nFrame #1 \n";
                while (!temporay_frame.empty()){
                    temporay_frame.top()->print();
                    temporay_frame.pop();
                }
                cout << "Number of hops so far: "<<number_of_hops <<"\n";
                cout << "--------";
                for (auto& object : clients) {
                    if (object.client_id == sender_ID) {
                        object.outgoing_queue.push(frame);
                        break;
                    }
                }

            }
            else {
                int fragment_of_num = (message.length() + message_limit - 1) / message_limit;

                for (const auto& object : clients){
                    if (object.client_id == sender_ID){
                        sender_IP_address = object.client_ip;
                        sender_MAC_address = object.client_mac;
                        for(const auto& pair : object.routing_table){
                            if(pair.first == receiver_ID){
                                receiver_MAC_address_receiver = pair.second;
                            }
                        }
                    }
                    if(receiver_ID == object.client_id){
                        receiver_IP_address = object.client_ip;
                    }
                }
                for (const auto& object : clients){
                    if (object.client_id == receiver_MAC_address_receiver){
                        receiver_MAC_address = object.client_mac;
                    }
                }

                for (int fragment_index = 0; fragment_index < fragment_of_num; ++fragment_index) {
                    int start_index = fragment_index * message_limit;
                    int end_index = std::min((fragment_index + 1) * message_limit, static_cast<int>(message.length()));

                    std::string fragment = message.substr(start_index, end_index - start_index);

                    auto *application_layer = new ApplicationLayerPacket(0, sender_ID, receiver_ID, fragment);
                    auto *transport_layer = new TransportLayerPacket(1, sender_port, receiver_port);
                    auto *network_layer = new NetworkLayerPacket(2, sender_IP_address, receiver_IP_address);
                    auto *physical_layer = new PhysicalLayerPacket(3, sender_MAC_address, receiver_MAC_address);

                    stack<Packet*> frame;
                    frame.push(application_layer);
                    frame.push(transport_layer);
                    frame.push(network_layer);
                    frame.push(physical_layer);

                    stack<Packet*> temporary_frame = frame;

                    cout << "\nFrame #" << fragment_index + 1 << "\n";
                    while (!temporary_frame.empty()) {
                        temporary_frame.top()->print();
                        temporary_frame.pop();
                    }
                    cout << "Number of hops so far: "<<number_of_hops << "\n";
                    cout << "--------";
                    for (auto& object : clients) {
                        if (object.client_id == sender_ID) {
                            object.outgoing_queue.push(frame);
                            break;
                        }
                    }
                }
            }

        }
        else if (new_str[0] == "SHOW_FRAME_INFO"){
            if (new_str[2] == "out"){
                cout << "Current Frame #" << new_str[3] << " on the outgoing queue of client " << new_str[1] << endl;
                int number = 0;
                for (auto& object : clients) {
                    if (object.client_id == new_str[1]) {
                        queue<stack<Packet*>> temp_queue = object.outgoing_queue;
                        while (!temp_queue.empty()) {
                            stack<Packet*> frame = temp_queue.front();
                            number++;
                            if (number == std::stoi(new_str[3])) {
                                stack<Packet*> temp_stack;

                                while (!frame.empty()) {
                                    temp_stack.push(frame.top());
                                    frame.pop();
                                }

                                while (!temp_stack.empty()) {
                                    std::cout << "Layer "<< temp_stack.top()->layer_ID << " info: ";
                                    temp_stack.top()->print();
                                    temp_stack.pop();
                                }
                            }
                            temp_queue.pop();
                        }

                    }
                }
                cout << "Number of hops so far: "<<number_of_hops << endl;
            }
            else if (new_str[2] == "in"){

            }
        }
        else if (new_str[0] == "SHOW_Q_INFO"){
            int number = 0;
            if(new_str[2] == "out"){
                for (auto& object : clients) {
                    if(object.client_id == new_str[1]){
                        queue<stack<Packet*>> temp_queue = object.outgoing_queue;
                        while (!temp_queue.empty()) {
                            temp_queue.pop();
                            number++;
                        }

                    }
                }
                cout<<"Client C Outgoing Queue Status" <<endl;
            }
            else if(new_str[2] == "in"){
                for (auto& object : clients) {
                    if(object.client_id == new_str[1]){
                        queue<stack<Packet*>> temp_queue = object.incoming_queue;
                        while (!temp_queue.empty()) {
                            temp_queue.pop();
                            number++;
                        }

                    }
                }
                cout<<"Client C Incoming Queue Status" <<endl;
            }

            cout<<"Current total number of frames: " << number << endl;

        }
        else if (new_str[0] == "SEND"){
            number_of_hops++;
            for (auto& object : clients){
                queue<stack<Packet*>> temp_queue = object.outgoing_queue;
                int number_of_frame = 1;
                while (!temp_queue.empty()) {
                    stack<Packet*> frame = object.outgoing_queue.front();
                    stack<Packet*> temporary_frame = frame;
                    std::string rec_id;
                    std::string send_id;
                    while (!temporary_frame.empty()){
                        if (auto* appLayerPacket = dynamic_cast<ApplicationLayerPacket*>(temporary_frame.top())) {
                            rec_id = appLayerPacket->receiver_ID;
                            send_id = appLayerPacket->sender_ID;
                        }
                        temporary_frame.pop();
                    }
                    std::string next_client_id = object.routing_table[rec_id];
                    auto next_hop_client_it = clients.end();
                    for (auto it = clients.begin(); it != clients.end(); ++it) {
                        if (it->client_id == next_client_id) {
                            next_hop_client_it = it;
                            break;
                        }
                    }
                    next_hop_client_it->incoming_queue.push(frame);

                    cout << "Client "<<object.client_id  <<" sending frame #"<<number_of_frame << " to client " << next_hop_client_it->client_id<<endl;
                    stack<Packet*> temp_frame = temp_queue.front();
                    while(!temp_frame.empty()){
                        temp_frame.top()->print();
                        temp_frame.pop();
                    }
                    cout<<"Number of hops so far: " << number_of_hops << endl;
                    cout << "--------" << endl;
                    temp_queue.pop();
                    number_of_frame ++;
                }

            }
        }
        else if (new_str[0] == "RECEIVE"){
            bool first = true;
            int number_of_frame = 0;
            for (auto& object : clients){
                queue<stack<Packet*>> temp_queue = object.incoming_queue;
                while (!temp_queue.empty()) {
                    stack<Packet*> temp_packet = temp_queue.front();
                    std::string receiver_id;
                    std::string sender_id;
                    std::string sender_mac;
                    std::string receiver_mac;
                    while (!temp_packet.empty()) {
                        if (auto* appLayerPacket = dynamic_cast<ApplicationLayerPacket*>(temp_packet.top())) {
                            receiver_id = appLayerPacket->receiver_ID;
                            sender_id = appLayerPacket->sender_ID;
                        }
                        temp_packet.pop();
                    }
                    stack<Packet*> temp_packet2 = temp_queue.front();
                    while (!temp_packet2.empty()) {
                        if(auto* physicalLayerPacket = dynamic_cast<PhysicalLayerPacket*>(temp_packet2.top())){
                            physicalLayerPacket->setSenderMacAddress(object.client_mac);
                            sender_mac = physicalLayerPacket->getSenderMacAddress();
                            for(auto& new_object:clients){
                                if(new_object.client_id == object.routing_table[receiver_id]){
                                    physicalLayerPacket->setReceiverMacAddress(new_object.client_mac);
                                    receiver_mac = physicalLayerPacket->getReceiverMacAddress();
                                    break;

                                }
                            }
                        }
                        temp_packet2.pop();
                    }
                    if(sender_id != receiver_id){
                        if (first){
                            cout << "Client "<<object.client_id <<" receiving a message from client "<< sender_id <<", but intended for client "<<receiver_id<<". Forwarding... "<<endl;
                            first = false;
                        }
                        cout << "Frame #"<<number_of_frame <<" MAC address change: New sender MAC "<< sender_mac<<", new receiver MAC "<<receiver_mac << endl;
                    }
                    if(sender_id == receiver_id){
                        cout << "Client "<<receiver_id<<" receiving frame #1 from client "<<sender_id<<", originating from client "<<object.client_id << endl;
                        temp_packet.top()->print();
                        cout << " Number of hops so far: "<<number_of_hops<<endl;

                    }
                    temp_queue.pop();
                    number_of_frame++;
                }
            }

        }
        else if (new_str[0] == "PRINT_LOG"){
        }
        else {
            cout << "  Invalid command." << endl;
        }

    }
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    std::ifstream file(filename);
    std::string line;
    bool first_line = true;
    while (std::getline(file, line)) {
        if (first_line){
            first_line = false;
            continue;
        }
        std::istringstream iss(line);
        std::vector<std::string> new_line{
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>{}
        };
        Client client(new_line[0],new_line[1],new_line[2]);
        clients.push_back(client);
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    std::ifstream file(filename);
    std::string line;
    int counter = 0;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line == "-") {
            counter ++;
            continue;
        }
        std::istringstream iss(line);
        std::vector<std::string> new_line{
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>{}
        };

        if (new_line.size() == 2) {
            clients[counter].routing_table.insert(std::make_pair(new_line[0],new_line[1]));
        }
    }

}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.
    std::ifstream file(filename);
    std::string line;
    bool first_line = true;
    while (std::getline(file, line)) {
        if (first_line){
            first_line = false;
            continue;
        }
        commands.push_back(line);
    }
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}
