#pragma once
#include "stdafx.h"
#include "GLApp.h"
#include <memory>

int main(int argc, char* argv[])
{
	auto app = GLApp::GetInstance(argc, argv);
	app->Init("图形作业模板", { 1024,768 }, { 100,100 });
	app->Run();
}