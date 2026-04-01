/*
4x4x4 LED CUBE (NO TRANSISTORS) - Arduino Uno
MODIFIED FOR:

Layers = ANODES (through 220 ohm resistors)
Columns = CATHODES (direct)

NEW LOGIC:
Layer HIGH = ON
Column LOW = ON

Notes:
- Serial Monitor NOT available (uses D0/D1)
- Avoid turning on too many LEDs at once
*/
const int snakePathX[12] = { 0, 1, 2, 3, 3, 3, 2, 1, 0, 0, 0, 1 };
const int snakePathY[12] = { 0, 0, 0, 0, 1, 2, 3, 3, 3, 2, 1, 1 };

const int buttonPin = 13;

const int columnPins[16] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3
};

const int layerPins[4] = {
  A4, A5, 0, 1  // L0 (bottom) to L3 (top)
};

bool cube[4][4][4];

inline int colIndex(int x, int y) {
  return (y * 4) + x;
}

// Turn all columns OFF (cathodes HIGH)
void allColumnsOff() {
  for (int i = 0; i < 16; i++)
    digitalWrite(columnPins[i], HIGH);
}

// Turn all layers OFF (anodes LOW)
void allLayersOff() {
  for (int i = 0; i < 4; i++)
    digitalWrite(layerPins[i], LOW);
}

void clearCube() {
  for (int z = 0; z < 4; z++)
    for (int y = 0; y < 4; y++)
      for (int x = 0; x < 4; x++)
        cube[z][y][x] = false;
}

void setVoxel(int x, int y, int z, bool on) {
  if (x < 0 || x > 3 || y < 0 || y > 3 || z < 0 || z > 3) return;
  cube[z][y][x] = on;
}

void setup() {

  pinMode(buttonPin, INPUT_PULLUP);


  for (int i = 0; i < 16; i++) {
    pinMode(columnPins[i], OUTPUT);
    digitalWrite(columnPins[i], HIGH);  // OFF
  }

  for (int i = 0; i < 4; i++) {
    pinMode(layerPins[i], OUTPUT);
    digitalWrite(layerPins[i], LOW);  // OFF
  }

  clearCube();
  randomSeed(analogRead(A6));
}

/*
Display refresh (multiplexing).
For each layer:
- enable layer (HIGH)
- set columns LOW where cube[z][y][x] is true
- short delay
- disable layer (LOW)
*/
void refreshCube(unsigned long durationMs) {

  unsigned long start = millis();

  while (millis() - start < durationMs) {

    for (int z = 0; z < 4; z++) {

      allLayersOff();
      allColumnsOff();

      // Enable this layer (ANODE HIGH)
      digitalWrite(layerPins[z], HIGH);

      // Turn on needed columns (CATHODE LOW)
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
          if (cube[z][y][x]) {
            digitalWrite(columnPins[colIndex(x, y)], LOW);
          }
        }
      }

      delay(2);
      digitalWrite(layerPins[z], LOW);
    }
  }
}

// 🎄 Christmas Twinkle Effect
void animChristmas(int cycles) {

  for (int c = 0; c < cycles; c++) {

    // Random sparkle phase
    for (int i = 0; i < 40; i++) {

      clearCube();

      // Turn on a few random LEDs (max 4 per layer safe)
      for (int k = 0; k < 8; k++) {
        int x = random(0, 4);
        int y = random(0, 4);
        int z = random(0, 4);
        setVoxel(x, y, z, true);
      }

      refreshCube(120);
    }

    // Soft full shimmer (checkerboard style, safe)
    clearCube();

    for (int z = 0; z < 4; z++) {
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {

          // Checker pattern to reduce current
          if ((x + y + z) % 2 == 0) {
            setVoxel(x, y, z, true);
          }
        }
      }
    }

    refreshCube(400);
  }
}

void animHeart() {

  clearCube();

  int z = 2;  // middle layer

  setVoxel(1, 0, z, true);
  setVoxel(2, 0, z, true);

  setVoxel(0, 1, z, true);
  setVoxel(3, 1, z, true);

  setVoxel(0, 2, z, true);
  setVoxel(3, 2, z, true);

  setVoxel(1, 3, z, true);
  setVoxel(2, 3, z, true);

  refreshCube(1000);
}
void animLetterA() {

  clearCube();
  int z = 1;

  setVoxel(1, 0, z, true);
  setVoxel(2, 0, z, true);

  setVoxel(0, 1, z, true);
  setVoxel(3, 1, z, true);

  setVoxel(0, 2, z, true);
  setVoxel(1, 2, z, true);
  setVoxel(2, 2, z, true);
  setVoxel(3, 2, z, true);

  setVoxel(0, 3, z, true);
  setVoxel(3, 3, z, true);

  refreshCube(1000);
}

void animWireCube() {

  clearCube();

  for (int z = 0; z < 4; z++) {
    for (int i = 0; i < 4; i++) {
      setVoxel(0, i, z, true);
      setVoxel(3, i, z, true);
      setVoxel(i, 0, z, true);
      setVoxel(i, 3, z, true);
    }
  }

  refreshCube(1200);
}
// Staircase Animation
void animStaircase(int repeats) {

  for (int r = 0; r < repeats; r++) {

    // --------------------------
    // GOING UP
    // --------------------------
    for (int z = 0; z < 4; z++) {
      clearCube();

      // Create a "stair" pattern: light up row by row
      for (int y = 0; y <= z; y++) {
        for (int x = 0; x <= y; x++) {
          setVoxel(x, y, z, true);  // stair step
        }
      }

      refreshCube(200);  // time each step
    }

    // --------------------------
    // GOING DOWN
    // --------------------------
    for (int z = 2; z >= 0; z--) {
      clearCube();

      for (int y = 0; y <= z; y++) {
        for (int x = 0; x <= y; x++) {
          setVoxel(x, y, z, true);
        }
      }

      refreshCube(200);
    }
  }
}
void animSnake(int repeats) {
  for (int r = 0; r < repeats; r++) {

    for (int z = 0; z < 4; z++) {
      clearCube();
      for (int i = 0; i < 12; i++) {
        setVoxel(snakePathX[i], snakePathY[i], z, true);
        refreshCube(150);
      }
    }

    for (int z = 2; z >= 0; z--) {
      clearCube();
      for (int i = 0; i < 12; i++) {
        setVoxel(snakePathX[i], snakePathY[i], z, true);
        refreshCube(150);
      }
    }
  }
}


/* --------------------------
SIMPLE ANIMATIONS
---------------------------*/
// Function definitions (every function goes here, outside loop)
void animSnakeUp() {
  for (int z = 0; z < 4; z++) {
    clearCube();
    for (int i = 0; i < 12; i++) {
      setVoxel(snakePathX[i], snakePathY[i], z, true);
      refreshCube(150);
    }
  }
}

void animSnakeDown() {
  for (int z = 2; z >= 0; z--) {
    clearCube();
    for (int i = 0; i < 12; i++) {
      setVoxel(snakePathX[i], snakePathY[i], z, true);
      refreshCube(150);
    }
  }
}
// 1) Single voxel bounce
void animBounceCenter(int repeats) {
  for (int r = 0; r < repeats; r++) {

    for (int z = 0; z < 4; z++) {
      clearCube();
      setVoxel(1, 1, z, true);
      refreshCube(180);
    }

    for (int z = 2; z >= 1; z--) {
      clearCube();
      setVoxel(1, 1, z, true);
      refreshCube(180);
    }
  }
}

// 2) Layer sweep (corners only)
void animLayerSweep(int repeats) {
  for (int r = 0; r < repeats; r++) {
    for (int z = 0; z < 4; z++) {
      clearCube();
      setVoxel(0, 0, z, true);
      setVoxel(3, 0, z, true);
      setVoxel(0, 3, z, true);
      setVoxel(3, 3, z, true);
      refreshCube(250);
    }
  }
}

// 3) Rain
int rand4() {
  return random(0, 4);
}

void animRain(int drops) {
  for (int d = 0; d < drops; d++) {
    int x = rand4();
    int y = rand4();
    for (int z = 3; z >= 0; z--) {
      clearCube();
      setVoxel(x, y, z, true);
      refreshCube(120);
    }
  }
}

void loop() {
  //animBounceCenter(3);
  //animLayerSweep(2);
  animRain(10);
  //  animChristmas(5);
  //animHeart();
  // animLetterA();
  //animWireCube();
  //  animStaircase(2);
    // animSnakeUp();
    // animSnakeDown();
}
