#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <unistd.h>

constexpr float cubeWidth = 20;
constexpr float K1 = 40;
constexpr int width = 160;
constexpr int height = 44;
constexpr float incrementSpeed = 0.6f;
constexpr int distanceFromCamera = 100;
constexpr char backgroundASCIICode = ' ';


struct float3 { float x, y, z; };

// Function to apply 3D rotation transformations
void rotatePoint(float3 &point, float3 rotations)
{
    float x = point.x, y = point.y, z = point.z;

    point.x = y * sin(rotations.x) * sin(rotations.y) * cos(rotations.z) - z * cos(rotations.x) * sin(rotations.y) * cos(rotations.z) + y * cos(rotations.x) * sin(rotations.z) + z * sin(rotations.x) * sin(rotations.z) + x * cos(rotations.y) * cos(rotations.z);
    point.y = y * cos(rotations.x) * cos(rotations.z) + z * sin(rotations.x) * cos(rotations.z) - y * sin(rotations.x) * sin(rotations.y) * sin(rotations.z) + z * cos(rotations.x) * sin(rotations.y) * sin(rotations.z) - x * cos(rotations.y) * sin(rotations.z);
    point.z = z * cos(rotations.x) * cos(rotations.y) - y * sin(rotations.x) * cos(rotations.y) + x * sin(rotations.y);
}

// Function to render a single point of the cube
void renderSurface(float3 point, float3 rotations, char* buf, float* zbuf, char ch) {
    rotatePoint(point, rotations);
    point.z += distanceFromCamera;

    float ooz = 1 / point.z; // One over z (inverse Z)
    int xp = static_cast<int>(width / 2  + K1 * ooz * point.x * 2);
    int yp = static_cast<int>(height / 2 + K1 * ooz * point.y);

    int idx = xp + yp * width;

    if (idx >= 0 && idx < width * height && ooz > zbuf[idx])
    {
        zbuf[idx] = ooz;
        buf[idx] = ch;
    }
}

void printSymbol(char symbol)
{
    switch(symbol)
    {
        case '@':
            std::cout << "\e[0;31m";
            break;
        case '$':
            std::cout << "\e[0;32m";
            break;
        case '~':
            std::cout << "\e[0;33m";
            break;
        case '#':
            std::cout << "\e[0;34m";
            break;
        case ';':
            std::cout << "\e[0;35m";
            break;
        case '+':
            std::cout << "\e[0;36m";
            break;
    }
    std::cout << symbol;
}

int main()
{
    float zBuffer[width * height];
    char buffer[width * height];
    float3 rotations = {0, 0, 0};
    std::cout << "\x1b[2J";  // Clear console

    while (true) {
        std::fill(buffer, buffer + width * height, backgroundASCIICode);
        std::fill(zBuffer, zBuffer + width * height, 0);

        // Draw the cube by iterating over its surface$$s
        for (float cubeX = -cubeWidth; cubeX <= cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY <= cubeWidth; cubeY += incrementSpeed) {
                renderSurface({cubeX, cubeY, -cubeWidth},  rotations, buffer, zBuffer, '@'); // Front face
                renderSurface({cubeWidth, cubeY, cubeX},   rotations, buffer, zBuffer, '$'); // Right face
                renderSurface({-cubeWidth, cubeY, -cubeX}, rotations, buffer, zBuffer, '~'); // Left face
                renderSurface({-cubeX, cubeY, cubeWidth},  rotations, buffer, zBuffer, '#'); // Back face
                renderSurface({cubeX, -cubeWidth, -cubeY}, rotations, buffer, zBuffer, ';'); // Bottom face
                renderSurface({cubeX, cubeWidth, cubeY},   rotations, buffer, zBuffer, '+'); // Top face
            }
        }

        std::cout << "\x1b[H";  // Move cursor to top-left
        for (int k = 0; k < width * height; k++) {
            if (k % width)
                printSymbol(buffer[k]);
            else
                std::cout << std::endl;
        }

        // Update rotation angles
        rotations.x += 0.05;
        rotations.y += 0.05;

        usleep(16000);  // Sleep for smooth animation
    }

    return EXIT_SUCCESS;
}
