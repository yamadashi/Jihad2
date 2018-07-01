#include "Result.h"


Result::Result()
	:font(25)
{}

void Result::update() {
	if (Input::KeyEnter.clicked) changeScene(SceneName::Title);
}

void Result::draw() const {
	font(m_data->cleared ? L"�������I" : L"�������I").drawAt(Window::Center());
}