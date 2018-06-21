#include "Title.h"

Title::Title()
	:select(Select::Start)
{}

void Title::update() {
	int val = static_cast<int>(select) + Input::KeyDown.clicked - Input::KeyUp.clicked;
	select = static_cast<Select>((3 + val) % 3);
	if (Input::KeyEnter.clicked) {
		switch (select) {
		case Select::Start: changeScene(SceneName::Game); break;
		case Select::Explain: changeScene(SceneName::Explain); break;
		case Select::End: System::Exit();
		}
	}
}

void Title::draw() const {
	PutText(L"Title‚Å‚·").at(Window::Center());
	PutText(static_cast<int>(select)).from(0, 0);
}