# Contributing to Pekka Kana 2

![pekka](res/gfx/manual/chick.png?raw=true)
Thank you for your interest in contributing to Pekka Kana 2.
All contributions are welcome, whether they are code, bug fixes,
documentation, tooling, or other improvements.

## Licensing of contributions

- By submitting a pull request, patch, or other contribution to this repository,
  you agree that your contribution is licensed under the terms of the project's
  licence.txt file. This ensures that contributions can be legally redistributed and
  maintained as part of the official project.
- Contributions are provided voluntarily, without expectation of payment,
  compensation, royalties, or additional licensing terms.


## General guidelines

- Version numbers (`v0.xxx`) are assigned only to commits on the `main` branch.
  Contributors should not include version numbers (`vx.xxx`) in commit messages or pull
  requests. The version is assigned when changes are merged into `main`.

- By submitting a Pull Request, you confirm that your changes are stable,
  complete, and ready to be merged into the main branch.
  Please do not submit Work-In-Progress (WIP) Pull Requests.
  Pull Requests are not a discussion forum — if you want to discuss ideas
  or unfinished work, please use Issues or Discord instead.

### Game content guidelines

- The episodes "Rooster Island 1" and "Rooster Island 2", as well as the original
  vanilla sprites and textures, are not intended to be modified.
  They are preserved in their original form for historical and cultural reasons.
- Please do not submit custom episodes through pull requests to this repository.
  Third-party episodes should be distributed via external stores
  (such as Makyuni's mapstore) as downloadable content.
- If necessary, a separate repository may be created for sharing
  custom episodes and other community-created content.


### Coding guidelines

- Avoid any change that can destroy backwards compatibility with existing levels.
- Avoid introducing third-party dependencies unless necessary.
- Prefer modern C++ containers and types such as `std::string`, `std::vector`,
  `std::array`, and `std::filesystem::path`.
  Avoid obsolete C-style arrays in new code.
  Legacy C arrays are acceptable only in backwards compatibility code.
- Initialize all class fields explicitly.
  Use in-class initializers where possible and initialize pointers to `nullptr`.

  Example:
    ```cpp
    class Sample {
        int i = 0;
        bool b = false;
        SDL_Surface* surface = nullptr;

        std::string s;
        std::vector<int> vi;
    };
    ```

- The preferred coding style for new and refactored code is illustrated below.
    Explicit use of `this->` is preferred for improved code readability.

    ```cpp
    class MyClass {
    public:
        MyClass();
        ~MyClass();
        void sampleMethod();

    private:
        int posX = 0;
    };

    MyClass::MyClass() {
        // constructor implementation
    }

    MyClass::~MyClass() {
        // destructor implementation
    }

    void MyClass::sampleMethod() {
        if (this->posX < 0) {
            this->posX = 0;
        } else {
            ++this->posX;
        }
    }
    ```



