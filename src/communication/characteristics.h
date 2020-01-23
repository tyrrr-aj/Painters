#ifndef CHARACTERISTICS_H
#define CHARACTERISTICS_H

#include "../geometry/Point.h"
#include "protocol.h"

#define NOTIFICATION_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define COURSE_CHARACTERISTIC_UUID "29d58832-a0e4-425f-85e6-8040f7a5bf23"
#define PROPOSAL_CHARACTERISTIC_UUID ""
#define RESPONSE_TO_PROPOSAL_CHARACTERISTIC_UUID ""

enum NotificationCode {NONE, NEW_COURSE, COLLISION_SPOTTED, PROPOSAL, RESPONSE_TO_PROPOSAL, FREE_WAY};

struct Course {
    Point position;
    Point destination;
};

class NotificationCharacteristic : public BLECharacteristic {
  public:
    NotificationCharacteristic() : BLECharacteristic(BLEUUID(CHARACTERISTIC_UUID_2)) {
      this->setNotifyProperty(true);
      pCharacteristic->addDescriptor(new BLE2902());
    }

    void setNotificationCode(int code) {
      setValue(&code);
    }
};

class CourseCharacteristic : public BLECharacteristic {
  public:
    CourseCharacteristic() : BLECharacteristic {
      this->setReadProperty(true);
      pCharacteristic->addDescriptor(new BLE2902());
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
    ProposalCharacteristic : BLECharacteristic {
      this->setReadProperty(true);
      pCharacteristic->addDescriptor(new BLE2902());
    }

    void setProposal(int numberOfSteps) {
      setValue(&numberOfSteps);
    }
};

class ResponseToProposalCharacteristic : public BLECharacteristic {
  public:
    ResponseToProposalCharacteristic : BLECharacteristic {
      this->setReadProperty(true);
      pCharacteristic->addDescriptor(new BLE2902());
    }

    void setResponse(protocol::ResponseToProposal response) {
      setValue(&response);
    }
};

#endif