#include "Title.h"

Title::Title()
	:select(Select::None),
	speed(3),
	font(20),
	logo_sub_handler(L"sub" + ToString(Random<int>(1,5))),
	logo_sub_scale(1.0 + 0.25*(1 + sinf(0)))
{
	pos[0].set(0, 0);
	pos[1].set(1500, 0);

	text[0] = L"‚Í‚¶‚ß‚é";
	text[1] = L"‚¹‚Â‚ß‚¢";
	text[2] = L"‚¨‚í‚é";

	SoundAsset(L"Title").setLoop(true);
	SoundAsset(L"Title").play(0.3ms);
}

void Title::update()
{
	select = Select::None;
	for (int i = 0; i < 3; i++) {
		if (font(text[i]).regionCenter(Window::Center().movedBy(0, 150 + i * 50)).contains(Mouse::Pos()))
			select = static_cast<Select>(i);
	}

	/*int val = static_cast<int>(select) + Input::KeyDown.clicked - Input::KeyUp.clicked;
	select = static_cast<Select>((3 + val) % 3);*/
	if (Input::MouseL.clicked) {

		switch (select) {
		case Select::Start:
			changeScene(SceneName::Game);
			SoundAsset(L"Title").stop(0.5ms);
			break;
		case Select::End: System::Exit();
		}
	}

	//”wŒi
	for (int i = 0; i < 2; i++) {
		if (pos[i].x + 1500 < 0) pos[i].x = pos[(i + 1) % 2].x + 1500;
		pos[i].moveBy(-speed, 0);
	}

	static double t = 0.0;
	if ((t += 0.15) >= 2 * PiF) t = 0.0;
	logo_sub_scale = 1.0 + 0.2*(1+sinf(t));
}

void Title::draw() const {

	for (const auto& position : pos) {
		TextureAsset(L"titleBG").draw(position);
	}

	TextureAsset(L"logo").drawAt(Window::Center().movedBy(0, -210));
	TextureAsset(logo_sub_handler).scale(logo_sub_scale).drawAt(Window::Center().movedBy(0, -25));


	int selected = static_cast<int>(select);
	for (int i = 0; i < 3; i++) {
		font(text[i]).drawAt(Window::Center().movedBy(0, 150+i*50), i==selected?Palette::Red:Palette::Black);
	}

}