#include "Result.h"

void Result::update() {
	if (Input::KeyEnter.clicked) changeScene(SceneName::Title);
}

void Result::draw() const {
	PutText(L"Result�ł�").at(Window::Center());
}