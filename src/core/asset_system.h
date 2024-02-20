#pragma once

class AssetSystem {
    friend class App;

  public:
    struct Config {};

    ~AssetSystem();

    static const AssetSystem& get();

    void initialize(const Config& config);
    void terminate();

  private:
    static AssetSystem& getMutable();

    AssetSystem();
    AssetSystem(const AssetSystem&)            = delete;
    AssetSystem(const AssetSystem&&)           = delete;
    AssetSystem& operator=(const AssetSystem&) = delete;
};
