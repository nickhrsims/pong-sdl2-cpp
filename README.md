# Pong SDL2 C++

## Runtime Requirements

```text
SDL2
SDL2_TTF
```

## Controls

```text
Input | Action
------+--------------
A     | Player 1 UP
Z     | Player 1 DOWN
      |
K     | Player 2 UP
M     | Player 2 DOWN
      |
P     | PAUSE
      |
ENTER | START
```

## Building

- Requires `conan2`

### Using Scripts

```sh
# Debug build
./init
./compile

# Release build
./init-release
./compile-release
```

### Directly using `conan`

```sh
conan install -sbuild_type=Debug -of build --build=missing .
conan build -of build .
```
