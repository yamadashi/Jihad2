#include "Explain.h"

void Explain::update() {
	if (Input::KeyEnter.clicked) changeScene(SceneName::Tutorial);
}

void Explain::draw() const {
	PutText(L"�����ł�").at(Window::Center());
}
