
# include "FileRegister.h"
# include "Fortress.h"
# include "Wall.h"
# include "Enemy.h"

void Main()
{
	Window::Resize(1500, 800);
	Graphics::SetBackground(Palette::White);

	TextureRegisterer().fileRegister(L"Files/").preload();
	Fortress fortress;
	Wall wall;
	std::list<std::unique_ptr<Enemy>> enemies;
	
	while (System::Update())
	{
		if (Input::KeySpace.clicked) enemies.emplace_back(new Thumb(wall));

		fortress.update();
		for (auto&& enemy : enemies) enemy->update(); 

		enemies.remove_if([](const std::unique_ptr<Enemy>& enemy) { return enemy->isDead(); });

		fortress.draw();
		wall.draw();
		for (const auto& enemy : enemies) enemy->draw();
	}
}
