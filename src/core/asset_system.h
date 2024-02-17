#pragma once

class AssetSystem {
  public:
    struct Config {};

    ~AssetSystem();

    static AssetSystem& get();

    void initialize(const Config& config);
    void terminate();

  private:
    AssetSystem();
    AssetSystem(const AssetSystem&)            = delete;
    AssetSystem(const AssetSystem&&)           = delete;
    AssetSystem& operator=(const AssetSystem&) = delete;
};
