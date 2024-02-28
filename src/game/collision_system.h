#pragma once

#include <memory>
#include <vector>

#include "entities/ball.h"
#include "entities/paddle.h"

class CollisionSystem {
  public:
    static void resolve(Rect& field, Paddle& leftPaddle, Paddle& rightPaddle,
                        Ball& ball);

  private:
    CollisionSystem();
    ~CollisionSystem();
};
