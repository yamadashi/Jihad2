#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>


struct SharedData {


};


enum class SceneName : int {
	Title = 0,
	Explain,
	Game,
	Result
};


using MySceneManager = SceneManager<SceneName, SharedData>;
using MyScene = SceneManager<SceneName, SharedData>::Scene;
