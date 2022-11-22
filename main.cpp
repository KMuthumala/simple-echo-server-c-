//
// Created by krishan on 11/22/22.
//

#include <iostream>
#include <utility>
#include "queue"
#include "App.h"

struct PerSocketData {
    /* Fill with user data */
};

struct messageItem{
    uWS::OpCode opCode;
    std::string_view message;
    uWS::WebSocket<false,true,PerSocketData> *ws;
};


void showq(std::queue<messageItem> gq) {
    std::queue<messageItem> g = std::move(gq);
    while (!g.empty()) {
        std::cout << '\t' << g.front().message;
        g.pop();
    }
    std::cout << '\n';
}

void messageCollector(std::queue<messageItem> *messageQueue) {



    uWS::App().ws<PerSocketData>(
            "/*",
            {
                    /* Settings */
                    .compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR_4KB | uWS::DEDICATED_DECOMPRESSOR),
                    .maxPayloadLength = 100 * 1024 * 1024,
                    .idleTimeout = 16,
                    .maxBackpressure = 100 * 1024 * 1024,
                    .closeOnBackpressureLimit = false,
                    .resetIdleTimeoutOnSend = false,
                    .sendPingsAutomatically = true,
                    /* Handlers */
                    .upgrade = nullptr,
                    .open = [](auto */*ws*/) {
                        /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
                        std::cout << "open" << std::endl;
                    },
                    .message = [&messageQueue](auto *ws, std::string_view message, uWS::OpCode opCode) {

                        auto temp = messageItem{
                                .opCode =  opCode,
                                .message =  message,
                                .ws = ws
                        };
                        messageQueue->push(temp);
                        std::cout << "Message Received : " << message << "  BY Thread : "<< std::this_thread::get_id() << std::endl;

                    },
                    .drain = [](auto */*ws*/) {
                        /* Check ws->getBufferedAmount() here */
                    },
                    .ping = [](auto */*ws*/, std::string_view) {
                        /* Not implemented yet */
                    },
                    .pong = [](auto */*ws*/, std::string_view) {
                        /* Not implemented yet */
                    },
                    .close = [](auto */*ws*/, int /*code*/, std::string_view /*message*/) {
                        /* You may access ws->getUserData() here */
                    }
            }
    ).listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    }).run();
}

void messageSender(std::queue<messageItem> *messageQueue) {
    while(true){
        if(!messageQueue->empty()){
            messageQueue->front().ws->send(messageQueue->front().message,messageQueue->front().opCode,true);
            std::cout << "Message Sent : "<< messageQueue->front().message << "  BY Thread : "<< std::this_thread::get_id() << std::endl;
            messageQueue->pop();
        }
    }

}

int main() {

    std::queue<messageItem> messageQueue;

    std::thread receivingThread(messageCollector, &messageQueue);
    std::thread sendingThread(messageSender, &messageQueue);

    receivingThread.join();
    sendingThread.join();
    return 0;
}