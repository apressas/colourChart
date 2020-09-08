#include "Display.h"

#include <cstdio>
#include <cstring>

namespace {
  constexpr uint16_t Width = 16;
  constexpr uint16_t Height = 9;
}

Display::Display(const std::string& name)
  : m_name(name)
  , m_frameBuffer(Width * Height)
{
}

Dimension Display::size() const
{
  return { Width, Height };
}

//Draws a line
void Display::draw(const Point point,
 const uint16_t *pixels,
 const uint16_t width)
{
  std::memcpy(&m_frameBuffer[point.y * Width + point.x],
  pixels,
  width * sizeof(uint16_t));
}

void Display::present()
{
  auto pixels = m_frameBuffer.data();
  for (int y = 0; y < Height; ++y) {
    for (int x = 0; x < Width; ++x) {

      printf("%04X ", *pixels++);
    }
    printf("\n");

  }
}
