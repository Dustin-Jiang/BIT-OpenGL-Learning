#pragma once
#include "stdafx.h"
#include "glut.h"

#include <map>

class Key {
public:
    int value;
    Key(int v): value(v) {};
    friend std::ostream& operator<<(std::ostream& os, const Key& key) {
        os << key.value;
        return os;
    }
    bool operator<(const Key& other) const {
        return value < other.value;
    }
    bool operator==(const Key& other) const {
        return value == other.value;
    }
};

inline Key KeyCode(unsigned char key) {
    return Key { (int)key };
}

inline Key KeyCode(char key) {
    return Key { (int)key };
}

inline Key KeyCode(int key) {
    return Key { key + 0x20050409 };
}

class Keyboard
{
private:
    Keyboard() : keyStates(), pressStates() {};
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;
    ~Keyboard() = default;

    std::map<Key, bool> keyStates, pressStates;
public:
    static Keyboard* GetInstance() {
        static Keyboard* instance = new Keyboard();
        return instance;
    }
    
    void OnKeyDown(Key key, int x, int y) {
        std::cout << "KeyDown: " << key << std::endl;
        keyStates[key] = true;
    }

    void OnKeyUp(Key key, int x, int y) {
        std::cout << "KeyUp: " << key << std::endl;
        keyStates[key] = false;
        pressStates[key] = true;
    }

    bool IsKeyDown(unsigned char key) {
        return IsKeyDown(KeyCode(key));
    }
    bool IsKeyDown(char key) {
        return IsKeyDown(KeyCode(key));
    }
    bool IsKeyDown(int key) {
        return IsKeyDown(KeyCode(key));
    }
    bool IsKeyDown(Key key) {
        return keyStates[key];
    }

    bool IsKeyPress(unsigned char key) {
        return IsKeyPress(KeyCode(key));
    }
    bool IsKeyPress(char key) {
        return IsKeyPress(KeyCode(key));
    }
    bool IsKeyPress(int key) {
        return IsKeyPress(KeyCode(key));
    }
    bool IsKeyPress(Key key) {
        if (pressStates[key]) {
            pressStates[key] = false;
            return true;
        }
        return false;
    }
};