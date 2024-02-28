#pragma once

#include <memory>
#include <vector>

#include "entities/paddle.h"

class CollisionSystem {
  public:
    static void resolve(Rect& field, Paddle& leftPaddle, Paddle& rightPaddle);

  private:
    CollisionSystem();
    ~CollisionSystem();
};
