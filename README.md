# super-bowser-bros-advance

## Notes

Lint with ClangFormat:

```shell
# install clang-format
brew install clang-format

# run from root of project
find . -name "*.cpp" -o -name "*.h" | xargs clang-format -i -style=file
```

---

## Getting Started

This project structure assumes the [Butano](https://github.com/GValiente/butano) GitHub repository exists in a sibling folder.

1. follow the [Butano "Getting Started"](https://gvaliente.github.io/butano/getting_started.html) documentation to install all other dependencies
    - `devkitPro`
    - `devkitARM`
    - The devkitARM `Game Boy Advance Development` component
    - A Game Boy Advance emulator (mGBA is great)
    - Python

2. Clone Butano locally

```shell
git clone https://github.com/GValiente/butano.git
```

3. Clone this project

```shell
git clone git@github.com:thegreatsunra/super-bowser-bros-advance.git
```

## Usage

1. Build the project

```shell
cd super-bowser-bros-advance
# build everything
make -j8 # where `8` is the number of CPU cores your build machine has
```

2. Run the `.gba` file in your Game Boy Advance emulator

## Linting with Artistic Style

### Installing

```shell
brew install astyle
```

### Using

```shell
cd super-bowser-collision
# lint with astyle
astyle --recursive --project -n "*.cpp,*.h"
```
