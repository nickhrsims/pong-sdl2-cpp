#pragma once

#include <memory>
#include <vector>

#include "entities/ball.h"
#include "entities/paddle.h"

class CollisionSystem {
    friend class Game;

  public:
    ~CollisionSystem();

    void resolve(Rect& field, Paddle& leftPaddle, Paddle& rightPaddle, Ball& ball);

  private:
    void initialize(std::function<void()> onLeftGoal,
                    std::function<void()> onRightGoal);
    void terminate();

    static CollisionSystem& get();

    std::function<void()> onLeftGoal;
    std::function<void()> onRightGoal;

    // --- Deleted Constructors
    CollisionSystem();
    CollisionSystem(const CollisionSystem&)             = delete;
    CollisionSystem(const CollisionSystem&&)            = delete;
    CollisionSystem& operator=(const CollisionSystem&)  = delete;
    CollisionSystem& operator=(const CollisionSystem&&) = delete;
};
