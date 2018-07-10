#include "Explain.h"

void Explain::update() {
	if (Input::KeyEnter.clicked) changeScene(SceneName::Tutorial);
}

void Explain::draw() const {
	PutText(L"ê‡ñæÇ≈Ç∑").at(Window::Center());
}
