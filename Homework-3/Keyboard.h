#pragma once
#include "stdafx.h"
#include "glut.h"

constexpr int KeyCode(int key) {
  return key + 0x20050409;
}

constexpr int KeyCode(unsigned char key) {
  return (int)key;
}