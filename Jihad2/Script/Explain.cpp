#include "Explain.h"

void Explain::update() {
	if (Input::KeyEnter.clicked) changeScene(SceneName::Title);
}

void Explain::draw() const {
	PutText(L"�����ł�").at(Window::Center());
}
