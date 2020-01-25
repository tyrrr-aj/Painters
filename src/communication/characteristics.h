#ifndef CHARACTERISTICS_H
#define CHARACTERISTICS_H

#include "../geometry/Point.h"
#include "protocol.h"

#define NOTIFICATION_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define COURSE_CHARACTERISTIC_UUID "29d58832-a0e4-425f-85e6-8040f7a5bf23"
#define PROPOSAL_CHARACTERISTIC_UUID "29d56782-a0e4-425f-85e6-8040f7a5bf23"
#define RESPONSE_TO_PROPOSAL_CHARACTERISTIC_UUID "29d12342-a0e4-425f-85e6-8040f7a5bf23"

enum NotificationCode {NONE, NEW_COURSE, COLLISION_SPOTTED, PROPOSAL, RESPONSE_TO_PROPOSAL, FREE_WAY};

struct Course {
    Point position;
    Point destination;
};

class NotificationCharacteristic : public BLECharacteristic {
  public:
    NotificationCharacteristic() : BLECharacteristic(BLEUUID(NOTIFICATION_CHARACTERISTIC_UUID)) {
      this->setNotifyProperty(true);
      this->addDescriptor(new BLE2902());
    }

    void setNotificationCode(int code) {
      setValue((uint8_t*) &code, sizeof code);
    }
};

class CourseCharacteristic : public BLECharacteristic {
  public:
    CourseCharacteristic() : BLECharacteristic(BLEUUID(COURSE_CHARACTERISTIC_UUID)) {
      this->setReadProperty(true);
      this->addDescriptor(new BLE2902());
    }

    void setCourse(Point position, Point destination) {
      Course course;
      course.position = position;
      course.destination = destination;
      setValue((uint8_t*) &course, sizeof course);
    }
};

class ProposalCharacteristic : public BLECharacteristic {
  public:
    ProposalCharacteristic() : BLECharacteristic(BLEUUID(PROPOSAL_CHARACTERISTIC_UUID)) {
      this->setReadProperty(true);
      this->addDescriptor(new BLE2902());
    }

    void setProposal(int numberOfSteps) {
      setValue((uint8_t*) &numberOfSteps, sizeof numberOfSteps);
    }
};

class ResponseToProposalCharacteristic : public BLECharacteristic {
  public:
    ResponseToProposalCharacteristic() : BLECharacteristic(BLEUUID(RESPONSE_TO_PROPOSAL_CHARACTERISTIC_UUID)) {
      this->setReadProperty(true);
      this->addDescriptor(new BLE2902());
    }

    void setResponse(protocol::ResponseToProposal response) {
      setValue((uint8_t*) &response, sizeof response);
    }
};

#endif