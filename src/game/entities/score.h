#pragma once

#include <vector>

#include "core/texture.h"
#include "game/entity.h"

// TODO: Tests
class Score : public Entity {
  public:
    /** Concrete primitive representing underlying score value */
    typedef uint8_t ValueType;

    /** Option Parameters */
    struct Params {
        const Font& font;
        ValueType max;
    };

    /** Destructor. */
    ~Score();

    /**
     * Create a new score widget.
     */
    Score(const Params params);

    // --- Delete Constructors & Operators
    Score(const Score&)            = delete;
    Score(Score&&)                 = delete;
    Score& operator=(const Score&) = delete;
    Score& operator=(Score&&)      = delete;

    // --- Virtual Implementations
    // TODO: Implement at max handler and notify here.
    void update(float delta) override { (void)delta; };
    void draw() const override;

    // --- Public API
    void increment();
    void reset();
    bool isAtMax();

  private:
    static std::vector<Texture> createIntegerTextureVector(const Font& font,
                                                           ValueType count);

    // NOTE: Candidate for string-texture caching example,
    //       such as an unordered map + LRU cache.

    /**
     * Maximum score, inclusive.
     *
     * Once this limit is reached, the game *should* be considered won.
     * (That is, the maximum score + 1 should not be displayed on screen).
     */
    ValueType max;

    /** Integer-texture vector reference used to draw the score */
    std::vector<Texture> textures;

    /**
     * Current score as an unsigned integer.
     */
    ValueType value{0};
};
