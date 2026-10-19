#ifndef DSA_H
#define DSA_H

#include <iostream>
#include "Models.h"

namespace CampusApp {

    class JoinRequest {
    public:
        Student student;
        int groupID;

        JoinRequest();
        JoinRequest(const Student& s, int gid);
    };

    class RequestQueue {
    private:
        JoinRequest arr[50];
        int front;
        int rear;

    public:
        RequestQueue();

        bool isEmpty() const;
        bool isFull() const;
        void enqueue(const JoinRequest& request);
        JoinRequest dequeue();
    };

} // namespace CampusApp

#endif