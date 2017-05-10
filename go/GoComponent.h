#ifndef GOCOMPONENT_H
#define GOCOMPONENT_H

#include <ren/SparseArray.h>

#include "GoAlloc.h"
#include "GoID.h"

class GameObject;

class GoComponent {
    GameObject *owner_;
public:
    GoComponent() : owner_(nullptr) {}
    virtual ~GoComponent() {}

    virtual GoID id() const = 0;

    virtual void Update() {}

    GameObject *owner() const { return owner_; }
    void set_owner(GameObject *go) { owner_ = go; }
};

#endif // GOCOMPONENT_H
