#ifndef BLE_COMMUNICATOR_H
#define BLE_COMMUNICATOR_H

#include "../geometry/Point.h"

class BLE_communicator {
    public:
        BLE_communicator();
        void setUpCommunication();
        void announceDirection(Point position, Point destination);
};

#endif