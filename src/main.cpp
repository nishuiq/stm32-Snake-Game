#include <Arduino.h>
#include <SPI.h>
#include <TFT.h>
#include <iostream>
#include "Controller.h"
#include "Game.h"
#include "Renderer.h"
#include "snake.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(kLED_PIN0, OUTPUT);
  pinMode(kLED_PIN1, OUTPUT);
  pinMode(kButton_K2, INPUT);
  pinMode(kButton_K3, INPUT);

  attachInterrupt(digitalPinToInterrupt(kButton_K2), Controller::HandleK2Press, FALLING);
  attachInterrupt(digitalPinToInterrupt(kButton_K3), Controller::HandleK3Press, FALLING);
}

void loop() {
  constexpr std::size_t kScreenWidth{160};
  constexpr std::size_t kScreenHeigth{128};
  constexpr std::size_t kGridWidth{16};
  constexpr std::size_t kGridHeigth{16};
  constexpr std::size_t kFramesPerSecond{5};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};

  Renderer renderer(kScreenWidth, kScreenHeigth, kGridWidth, kGridHeigth);
  Controller controller;
  Game game(kGridWidth, kGridHeigth);
  game.Run(controller, renderer, kMsPerFrame);
  // Serial.println("Game Over");
  delay(2000);
}
