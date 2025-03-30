#pragma once
#include "stdafx.h"
#include "glut.h"

constexpr int KeyCode(unsigned char key) {
    return (int)key;
}

constexpr int KeyCode(char key) {
    return (int)key;
}

constexpr int KeyCode(int key) {
  return key + 0x20050409;
}